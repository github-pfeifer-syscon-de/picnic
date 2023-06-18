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

// Asynchronous thumbnail generator for cases where no dbus service is available

#pragma once

#include <glibmm.h>
#include <ConcurrentCollections.hpp>

#include "Worker.hpp"


class Pict;

class ThreadWorker : public Worker
{
public:
    ThreadWorker(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher &_readyDispatcher);
    virtual ~ThreadWorker();

    void queue(Pict * pict) override;
    bool isMimeSupported(const std::string mime) override;
protected:
    void loadScaled(Pict* pict);
    void work();
private:
    std::thread *m_workerThread;
    TQueueConcurrent<Pict *> m_files;
};