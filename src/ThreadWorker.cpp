/*
 * Copyright (C) 2021 rpf
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

#include <gtkmm.h>
#include <iostream>

#include "ThreadWorker.hpp"
#include "Pict.hpp"

ThreadWorker::ThreadWorker(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher &_readyDispatcher)
: Worker(_Dispatcher, _readyDispatcher)
, m_workerThread{nullptr}
, m_files()
{
    m_readyDispatcher.emit();   // here we wont need a special setup
}

ThreadWorker::~ThreadWorker()
{
    if (m_workerThread) {
        if (m_files.isActive()) {
            m_files.finish();
            if (m_workerThread->joinable()) {
                m_workerThread->join();
            }
            delete m_workerThread;
        }
        m_workerThread = nullptr;
    }
}

void
ThreadWorker::loadScaled(const psc::mem::active_ptr<Pict>& pict)
{
    if (auto lpict = pict.lease()) {
        Glib::RefPtr<Gio::File> path = lpict->getPath();
        try {
            //std::cout << "Reading " << path->get_path() << std::endl;
            auto pix = Gdk::Pixbuf::create_from_file(path->get_path(), 256, 256, true);
            lpict->setPixbuf(pix);
            m_Dispatcher.emit();    // keep updating view
        }
        catch (const Glib::FileError &exc) {
            std::cerr << "File exc " << path->get_path() << " " << exc.what() << std::endl;
        }
        catch (const Gdk::PixbufError &exc) {
            std::cerr << "Pixbuf exc " << path->get_path() << " " << exc.what() << std::endl;
        }
    }
}

void
ThreadWorker::work()
{
     uint32_t read = 0;
     while (m_files.isActive()) {
        auto pict = m_files.pop_front();
        if (pict
         && m_files.isActive()) {
            loadScaled(pict);
            ++read;
            //std::cout << "read image scaled " << read << std::endl;
        }
    }
}

void
ThreadWorker::queue(const psc::mem::active_ptr<Pict>& pict)
{
    m_files.emplace_back(pict);
    if (m_workerThread == nullptr) {    // std:async woud be easier to handle...
        m_workerThread = new std::thread([this]
        {
            work();
        });
    }
}

bool
ThreadWorker::isMimeSupported(const std::string mime)
{
    std::vector<Gdk::PixbufFormat> formats = Gdk::Pixbuf::get_formats();
    for (auto format : formats) {
		if (!format.is_disabled()) {	// scalable is false for pixel formats
			for (auto mimeType : format.get_mime_types()) {
				if (mimeType == mime) {
					//std::cout << format.get_name()
					//		  << " match " << mimeType
					//		  << " scale " << (format.is_scalable() ? "yes" : "no")
					//		  << " disabled " << (format.is_disabled() ? "yes" : "no")
					//		  << std::endl;
					return true;
				}
			}
        }
    }
    return false;
}
