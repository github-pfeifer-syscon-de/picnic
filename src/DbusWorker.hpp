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

// Dbus thumbnail adapter

#pragma once

#include <string>
#include <map>
#include <list>
#include <future>
#include <ConcurrentCollections.hpp>

#include "Worker.hpp"
#include "../gen/thumbnail1_proxy.h"
#include "Pict.hpp"

#undef DBUS_WORKER_DEBUG

class DbusWorker : public Worker
{
public:
    DbusWorker(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher &_readyDispatcher);
    virtual ~DbusWorker();

    void proxy_created(const Glib::RefPtr<Gio::AsyncResult> result);
    void getSupported(const Glib::RefPtr<Gio::AsyncResult> &result);
    void getFlavors(const Glib::RefPtr<Gio::AsyncResult> &result);
    void getSchedulers(const Glib::RefPtr<Gio::AsyncResult> &result);
    void queue_ret(Glib::RefPtr<Gio::AsyncResult> &result);
    void on_finish(guint32 handle);
    void on_ready(guint32 handle, std::vector<Glib::ustring> ready);
    void on_error(guint32 handle, std::vector<Glib::ustring> error, gint32 err, Glib::ustring msg);
    Glib::RefPtr<Gio::File> getCache(Glib::ustring ready);

    void queue(const psc::mem::active_ptr<Pict>&  pict);
    bool isMimeSupported(const std::string mime) override;
    void on_queue_check();
    const static constexpr auto THUMBNAIL1_NAME = "org.freedesktop.thumbnails.Thumbnailer1";
private:
    void enqueue(const std::vector<Glib::ustring> &uris, const std::vector<Glib::ustring> &mimes);
    void queue();
    std::string getMd5Hash(const Glib::ustring &uri);
    void readThumbnails();
    void setThumbnail(const psc::mem::active_ptr<Pict>& pict, Glib::RefPtr<Gio::File> thumbnail);

    Glib::RefPtr<org::freedesktop::thumbnails::Thumbnailer1Proxy> proxy;
    std::list<psc::mem::active_ptr<Pict>> m_pictsQueue;      // pics queued list  (using queue woud be a bad idea)
    std::map<std::string, psc::mem::active_ptr<Pict>> m_pictsInqueue;   // pics momentary in dbus processing
    guint32 m_handle;
    Glib::ustring m_flavor;
    Glib::ustring m_scheduler;
    std::map<std::string, std::string> m_mimes;
    const uint32_t QUEUE_COUNT = 4; // do request in blocks to update incrementally
    mutable std::mutex  m_MutexBus;
    TQueueConcurrent<psc::mem::active_ptr<Pict>> m_thumbnailReaderQueue;
    std::future<void> m_thumbnailReader;          // do file reading asychronously
    uint32_t m_queue;
    uint32_t m_ready;
    bool m_mimeComplete;
    bool m_flavorComplete;
    bool m_scheduleComplete;
    bool m_readyNotified;
    Glib::Dispatcher m_queueDispatcher;  // used for queue activation
};
