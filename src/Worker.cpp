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

#include <iostream>

#include "Worker.hpp"
#include "DbusWorker.hpp"
#include "ThreadWorker.hpp"

Worker::Worker(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher & _readyDispatcher)
: m_Dispatcher(_Dispatcher)
, m_readyDispatcher(_readyDispatcher)
{
}


Worker::~Worker()
{
}

Worker*
Worker::create(Glib::Dispatcher &_Dispatcher, Glib::Dispatcher &_readyDispatcher)
{
    // have to find system w/o thumbnailer to test this
    if (Gio::DBus::is_interface_name(DbusWorker::THUMBNAIL1_NAME)) {
        //std::cout << "DbusWorker" << std::endl;
        return new DbusWorker(_Dispatcher, _readyDispatcher);
    }
    else {
        //std::cout << "ThreadWorker" << std::endl;
        return new ThreadWorker(_Dispatcher, _readyDispatcher);
	}
}

Glib::RefPtr<Gdk::Pixbuf>
Worker::removeTransparency(Glib::RefPtr<Gdk::Pixbuf>& pixbuf, int dest_width, int dest_height)
{
	if (pixbuf->get_has_alpha()) {
		auto width = pixbuf->get_width();
		auto height = pixbuf->get_height();
		auto rowstride = pixbuf->get_rowstride();
		const guint8* orig_pixels = pixbuf->get_pixels();
		auto dest_rowstride = dest_width * 3;
		auto pixels = new guint8[dest_rowstride * dest_height];
		for (gint32 row = 0; row < height; row++) {
			auto orig_ptr = orig_pixels + rowstride * row;
			auto ptr = pixels + dest_rowstride * row;
			for (gint32 col = 0; col < width; col++) {
				if (row < height && col < width) {
					*ptr++ = *orig_ptr++;
					*ptr++ = *orig_ptr++;
					*ptr++ = *orig_ptr++;
					++orig_ptr;	// skip transparency
				}
				else {
					*ptr++ = 0;
					*ptr++ = 0;
					*ptr++ = 0;
				}
			}
		}

		pixbuf = Gdk::Pixbuf::create_from_data(pixels,
											pixbuf->get_colorspace(),
											false, 8,
											dest_width, dest_height,
											dest_rowstride,
											sigc::mem_fun(*this, &Worker::freeBuffer));
	}
	return pixbuf;
}

void
Worker::freeBuffer(const guint8* pixels)
{
	if (pixels) {
		delete pixels;
	}
}