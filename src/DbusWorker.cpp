/*
 * Copyright (C) 2018 rpf
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <openssl/evp.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <Log.hpp>

#include "DbusWorker.hpp"
#include "Pict.hpp"

const char* DbusWorker::THUMBNAIL1_NAME = "org.freedesktop.thumbnails.Thumbnailer1";

// as it looks event are delivered on the ui-thread

DbusWorker::DbusWorker(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher &_readyDispatcher)
: Worker(_Dispatcher, _readyDispatcher)
, m_pictsQueue()
, m_pictsInqueue()
, m_handle{0}
, m_MutexBus()
, m_thumbnailReaderQueue()
, m_queue{0}
, m_ready{0}
, m_mimeComplete{false}
, m_flavorComplete{false}
, m_scheduleComplete{false}
, m_readyNotified{false}
, m_queueDispatcher()
{
    m_queueDispatcher.connect(sigc::mem_fun(*this, &DbusWorker::on_queue_check));

    org::freedesktop::thumbnails::Thumbnailer1::createForBus(
                                Gio::DBus::BUS_TYPE_SESSION,
                                Gio::DBus::PROXY_FLAGS_NONE,
                                THUMBNAIL1_NAME,
                                "/org/freedesktop/thumbnails/Thumbnailer1",
                                sigc::mem_fun(*this, &DbusWorker::proxy_created));

}


DbusWorker::~DbusWorker()
{
    m_thumbnailReaderQueue.finish();
}

void
DbusWorker::getSchedulers(const Glib::RefPtr<Gio::AsyncResult> &result)
{
    std::vector<Glib::ustring> scheds;
    proxy->GetSchedulers_finish(scheds, result);
    //std::cout << "GetSchedulers_finish scheds: " << scheds.size() << std::endl;
    for (auto sched : scheds) {
        //std::cout << "  sched " << sched << std::endl;
        if (sched == "background") {  // this might have some issue
            m_scheduler = sched;
        }
    }
    if (m_scheduler.length() == 0) {
        m_scheduler = "default";    // default shoud at least be supported
    }
    m_scheduleComplete = true;
    queue();
}


void
DbusWorker::getSupported(const Glib::RefPtr<Gio::AsyncResult> &result)
{
    std::vector<Glib::ustring> uris;
    std::vector<Glib::ustring> mimes;
    proxy->GetSupported_finish(uris, mimes, result);
    //std::cout << "GetSupported_finish uri: " << uris.size() << " mime: " << mimes.size() << std::endl;
    for (guint i = 0; i< uris.size(); ++i) {
        if (uris[i] == "file") {    //unsure how to handle resource,... ???
            //std::cout << "uri[" << i << "] " << uris[i] << " " << mimes[i] << std::endl;
            m_mimes[mimes[i]] = uris[i];
        }
    }
    m_mimeComplete = true;
    queue();
}

void
DbusWorker::getFlavors(const Glib::RefPtr<Gio::AsyncResult> &result)
{
    std::vector<Glib::ustring> flavors;
    proxy->GetFlavors_finish(flavors, result);
    //std::cout << "GetFlavors_finish flavors: " << flavors.size() << std::endl;
    for (auto flv : flavors) {
        //std::cout << "  flavor " << flv << std::endl;
        if (flv == "large") {
            m_flavor = flv;
        }
    }
    if (m_flavor.length() == 0) {
        m_flavor = "normal";    // normal shoud at least be supported
    }
    m_flavorComplete = true;
    queue();
}


void
DbusWorker::queue_ret(Glib::RefPtr<Gio::AsyncResult>& result)
{
    guint32 ret;
    proxy->Queue_finish(ret, result);
    //std::cout << "DbusWorker::queue_ret " << ret << std::endl;
    //queue();    // queue next if previous block was accepted, otherwise we wont get them all back
}

void
DbusWorker::enqueue(const std::vector<Glib::ustring> &uris, const std::vector<Glib::ustring> &mimes)
{
    if (uris.size() > 0) {
        proxy->Queue(uris, mimes, m_flavor, m_scheduler, m_handle, sigc::mem_fun(*this, &DbusWorker::queue_ret));
        //std::cout << "DbusWorker::enqueue " << m_handle
		//	      << " cnt: " << uris.size()
		//	      << " flavor " << m_flavor
		//	      << " scheduler " << m_scheduler
		//	      << std::endl;
        ++m_handle;
    }
}

void
DbusWorker::queue()
{
    //std::cout << "DbusWorker::queue try flav " << m_flavor
    //          << " sched " << m_scheduler
    //          << " mimes " << m_mimes.size()
    //          <<  std::endl;
    if (!m_scheduleComplete
     || !m_flavorComplete
     || !m_mimeComplete) {    // start if all relevant infos are known
        return;
    }
    if (!m_readyNotified) {   // and notify view
        m_readyDispatcher.emit();
        m_readyNotified = true;
    }
    if (m_pictsInqueue.empty()) {
        std::vector<Glib::ustring> uris;
        std::vector<Glib::ustring> mimes;
        std::lock_guard<std::mutex> lock(m_MutexBus);
        while (!m_pictsQueue.empty()) {
            auto pict = m_pictsQueue.front();//std::move() will destry entry with pop ???
            m_pictsQueue.pop_front();
            if (auto lpict = pict.lease()) {
                std::string mime = lpict->getMime();
                if (!lpict->hasThumbnail()
                  && !mime.empty()) {   // isMimeSupported shoud have been checked before
                    std::string uri = lpict->getUri();
                    //std::cout << "DbusWorker::queue uri " << uri
                    //	      << " mime " << mime
                    //	      << std::endl;
                    m_pictsInqueue.insert(std::pair(uri, pict));
                    uris.push_back(uri);
                    mimes.push_back(mime);
                    if (mimes.size() >= QUEUE_COUNT) {
                        break;
                    }
                }
            }
        }
        if (!uris.empty()) {
            m_queue += uris.size();
            //std::cout << "DbusWorker::queue " << m_queue << std::endl;
        }
        enqueue(uris, mimes);
    }
}

void
DbusWorker::on_finish(guint32 handle)
{
    //std::cout << "Finish " << handle << std::endl;
    m_pictsInqueue.clear();     // for not blocking further handling
	m_queueDispatcher.emit();
}

void
DbusWorker::on_ready(guint32 handle, std::vector<Glib::ustring> ready)
{
    // handle is not the one we submitted, but increases on invocations
    std::lock_guard<std::mutex> lock(m_MutexBus);   // ensure we wont add new entries while working + m_pictsInqueue.empty ...
    m_ready += ready.size();
    //std::cout << "Ready " << m_ready
	//	      << " handle " << handle
	//	      << std::endl;
    for (auto rdy : ready) {
        //std::cout << "Ready[" << i << "] " << ready[i] << std::endl;
        auto p = m_pictsInqueue.find(rdy);
        if (p != m_pictsInqueue.end()) {
            auto pict = p->second;
            Glib::RefPtr<Gio::File> thumbnail = getCache(rdy);
            setThumbnail(pict, thumbnail);
            m_pictsInqueue.erase(rdy);
        }
        else {
            std::cerr << "Coud not assign pict " << rdy << std::endl;
        }
    }
}


Glib::RefPtr<Gio::File>
DbusWorker::getCache(Glib::ustring ready)
{
    // as described by:
    // https://specifications.freedesktop.org/thumbnail-spec/thumbnail-spec-latest.html
    std::string cache = Glib::get_user_cache_dir();

    std::string hash = getMd5Hash(ready);
    std::string thumbnail = Glib::build_filename(cache, "thumbnails", m_flavor, hash + ".png");

    return Gio::File::create_for_path(thumbnail);
}

void
DbusWorker::on_error(guint32 handle, std::vector<Glib::ustring> error, gint32 err, Glib::ustring msg)
{
    std::cout << "Error " << handle << " err " << err << " msg " << msg << std::endl;
    for (auto err : error) {
        std::cout << "Err  " << err << std::endl;
    }
}

void
DbusWorker::proxy_created(const Glib::RefPtr<Gio::AsyncResult> result)
{
    proxy = org::freedesktop::thumbnails::Thumbnailer1::createForBusFinish(result);

    proxy->Finished_signal.connect(sigc::mem_fun(*this, &DbusWorker::on_finish));
    proxy->Ready_signal.connect(sigc::mem_fun(*this, &DbusWorker::on_ready));
    proxy->Error_signal.connect(sigc::mem_fun(*this, &DbusWorker::on_error));

    proxy->GetSupported(sigc::mem_fun(*this, &DbusWorker::getSupported));
    proxy->GetSchedulers(sigc::mem_fun(*this, &DbusWorker::getSchedulers));
    proxy->GetFlavors(sigc::mem_fun(*this, &DbusWorker::getFlavors));
}

bool
DbusWorker::isMimeSupported(const std::string mime)
{
    return m_mimes.find(mime) != m_mimes.end();  // accept only supported
}


void
DbusWorker::queue(const psc::mem::active_ptr<Pict>& pict)
{
    m_pictsQueue.push_back(pict);
	m_queueDispatcher.emit();
}

// requires openssl
std::string
DbusWorker::getMd5Hash(const Glib::ustring &uri)
{
    const EVP_MD *mdType = EVP_md5();
    unsigned int mdLen = EVP_MD_size(mdType);
    auto md{new unsigned char[mdLen]};

    EVP_MD_CTX *ctx = EVP_MD_CTX_create();
    EVP_MD_CTX_init(ctx);
    EVP_DigestInit_ex(ctx, mdType, nullptr);
    unsigned int dataLen = strlen(uri.c_str());
    EVP_DigestUpdate(ctx, uri.c_str(), dataLen);
    EVP_DigestFinal_ex(ctx, md, nullptr);
    EVP_MD_CTX_destroy(ctx);

    std::ostringstream oss1;
    for (unsigned int i = 0; i < mdLen; ++i) {
        oss1 << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)(md[i] & 0xff);
    }
    delete[] md;
    std::string ret = oss1.str();
    return ret;
}

void
DbusWorker::setThumbnail(const psc::mem::active_ptr<Pict>& pict, Glib::RefPtr<Gio::File> thumbnail)
{
    // try to offload the costly file access from dbus event handling
    psc::log::Log::logAdd(psc::log::Level::Info, Glib::ustring::sprintf("DbusWorker::setThumbnail path %s", thumbnail->get_path()));
    if (auto lpict = pict.lease()) {
        lpict->setThumbnail(thumbnail);
        if (!m_thumbnailReader.valid()) {
            //std::cout << "thumbs not valid creating async" << std::endl;
            m_thumbnailReader = std::async(std::launch::async, &DbusWorker::readThumbnails, this);
        }
    }
    m_thumbnailReaderQueue.push_back(pict); // as we want to keep previous reference push is the way to go
}

void
DbusWorker::readThumbnails()
{
    uint32_t read = 0;
    //std::cout << "DbusWorker::readThumbnails" << std::endl;
    while (m_thumbnailReaderQueue.isActive()) {
        auto pict = m_thumbnailReaderQueue.pop_front();
        if (!pict
         || !m_thumbnailReaderQueue.isActive()) {
            break;
        }
        //std::cout << "DbusWorker::readThumbnails got " << pict << std::endl;
        if (auto lpict = pict.lease()) {
            Glib::RefPtr<Gio::File> thumbnail = lpict->getThumbnail();
            if (thumbnail->query_exists()) {
                //std::cout << "read thumbnail " << pict->getFileName() << std::endl;
                try {
                    auto pixbuf = Gdk::Pixbuf::create_from_file(thumbnail->get_path());
                    // create w/o transparency
                    //auto pixbuf2 = removeTransparency(pixbuf, 256, 256);
                    lpict->setPixbuf(pixbuf);
                    m_Dispatcher.emit();
                    ++read;
                    //std::cout << "read thumbnail " << read << std::endl;
                }
                catch (const Glib::FileError &exc) {
                    std::cerr << "File exc " << thumbnail->get_path() << " " << exc.what() << std::endl;
                }
                catch (const Gdk::PixbufError &exc) {
                    std::cerr << "Pixbuf exc " << thumbnail->get_path() << " " << exc.what() << std::endl;
                }
            }
            else {
                std::cerr << "Coud not find pict " << thumbnail->get_path() << std::endl;
            }
        }
    }
}

void
DbusWorker::on_queue_check()
{
	if (m_pictsInqueue.empty()) {   // important dbus-worker can handle only one request list a a time
        queue();
    }

}