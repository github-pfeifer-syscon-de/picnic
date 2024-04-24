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

#pragma once

#include <glibmm.h>
#include <gtkmm.h>
#include <active_ptr.hpp>


class Pict;

class Worker {
public:
    Worker(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher& _readyDispatcher);
    virtual ~Worker();

    virtual void queue(const psc::mem::active_ptr<Pict>& pict) = 0;
    virtual bool isMimeSupported(const std::string mime) = 0;
    static Worker* create(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher &_readyDispatcher);

protected:
    Glib::RefPtr<Gdk::Pixbuf> removeTransparency(Glib::RefPtr<Gdk::Pixbuf>& pixbuf, int dest_width, int dest_height);
    void freeBuffer(const guint8* pixels);

    Glib::Dispatcher &m_Dispatcher;
    Glib::Dispatcher &m_readyDispatcher;

private:

};

