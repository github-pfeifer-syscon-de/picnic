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
#include <iomanip>
#include <sys/stat.h>
#include <stdlib.h>

#include "ImageIconList.hpp"

ImageListColumns ImageIconList::m_imageListColumns;


ImageIconList::ImageIconList()
: Gtk::ListStore(m_imageListColumns)
, m_fileSupport(*this)
{
}

ImageIconList::~ImageIconList()
{
}

Glib::RefPtr<ImageIconList>
ImageIconList::create()
{
    return Glib::RefPtr<ImageIconList>(new ImageIconList());
}

const char *
ImageIconList::getAttributes()
{
	return "standard::*,time::*";  // these shoud be fast and the mime-type we need
}

void
ImageIconList::addRow(Glib::RefPtr<Gio::FileInfo>& fileInfo, Glib::RefPtr<Gio::File>& file)
{
    // may check  fileInfo->get_file_type() == Gio::FileType::FILE_TYPE_SYMBOLIC_LINK points to file
    if (fileInfo->get_file_type() == Gio::FileType::FILE_TYPE_REGULAR) {
		//std::cout << "PicnicListWin::add_file " << fileInfo->get_name() << std::endl;

		//for (auto attr : info->list_attributes()) {
		//    auto type = info->get_attribute_type(attr);
		//    std::cout << "   " << attr << "=" << info->get_attribute_as_string(attr) << std::endl;
		//}

		//std::string mime;   // the glib infos might be more accurate than ours, coud query incrementally
		//if (fileInfo->has_attribute(G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE)) {  // we asked for it so we can use it
		//    mime = fileInfo->get_attribute_string(G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE);
		//}
		//else if (fileInfo->has_attribute(G_FILE_ATTRIBUTE_STANDARD_FAST_CONTENT_TYPE)) {
		//    mime = fileInfo->get_attribute_string(G_FILE_ATTRIBUTE_STANDARD_FAST_CONTENT_TYPE);
		//}
		//std::cout << "File " << info->get_name() << " type " << info->get_file_type() << " mime " << mime << std::endl;
		//if (!mime.empty()) {
		//}

		auto i = append();
		auto row = *i;
		Glib::ustring cName(fileInfo->get_display_name());
		row.set_value(m_imageListColumns.m_name, cName);
		if (cName.length() > 16) {
			cName = cName.substr(0, 16);
			cName += u8"…";
		}
		row.set_value(m_imageListColumns.m_shortName, cName);
		auto buf = getIcon(fileInfo);
		if (buf) {
			row.set_value(m_imageListColumns.m_icon, buf);
		}
		row.set_value(m_imageListColumns.m_file, file);
    }
}

std::vector<Glib::RefPtr<Gio::File>>
ImageIconList::getFiles()
{
	std::vector<Glib::RefPtr<Gio::File>> files;
	for (auto row : children()) {
		files.push_back(row.get_value(m_imageListColumns.m_file));
	}
	return files;
}

void
ImageIconList::setIconTheme(Glib::RefPtr<Gtk::IconTheme>& iconTheme)
{
	m_iconTheme = iconTheme;
}

void
ImageIconList::load(Glib::RefPtr<Gio::File>& dir)
{
    clear();
    m_fileSupport.load(dir);
}

Glib::RefPtr<Gdk::Pixbuf>
ImageIconList::getIcon(Glib::RefPtr<Gio::FileInfo>& fileInfo) {
    //auto sp = iconTheme->get_search_path();
    //for (auto s = sp.begin(); s != sp.end(); ++s) {
    //    std::cout << " theme " << *s << std::endl;
    //}

    if (m_iconTheme && fileInfo->has_attribute(G_FILE_ATTRIBUTE_STANDARD_ICON)) {
        Glib::RefPtr<Gio::Icon> icon = fileInfo->get_icon();    // symbolic_icon wont work, icon is not so informative
        Glib::RefPtr<Gio::ThemedIcon> ticon = Glib::RefPtr<Gio::ThemedIcon>::cast_dynamic(icon);
        if (ticon) {
            Glib::StringArrayHandle names = ticon->get_names();
            if (!names.empty()) {
                auto i = names.begin();
                auto iconInfo = m_iconTheme->lookup_icon(ticon, 32/*, Gtk::IconLookupFlags::ICON_LOOKUP_USE_BUILTIN*/);
                if (iconInfo) {
                    auto buf = iconInfo.load_icon();
                    if (buf) {
                        //std::cout << "Names using " << *i  << " w " << buf->get_width() << " h " << buf->get_height() << " ch " << buf->get_n_channels() << " bits " << buf->get_bits_per_sample() <<  " for " << fileInfo->get_name() << std::endl;
						return buf;
                        //guint8 *pix = buf->get_pixels();
                        //int min = 255, max = 0;
                        //for (int x = 0; x < buf->get_rowstride(); ++x) {
                        //    for (int y = 0; y < buf->get_height(); ++y) {
                        //        int n = x + y * buf->get_rowstride();
                        //        min = std::min(min, pix[n] & 0xff);
                        //        max = std::max(max, pix[n] & 0xff);
                        //    }
                        //}
                        //std::cout << "Names using " << *i << " min " << min << " max " << max << std::endl;
                        //pict->setPixbuf(buf);
                    }
                    else {
                        std::cout << "Names no pixbuf " << *i << " name " << iconInfo.get_display_name() << " for " << fileInfo->get_name() << std::endl;
                    }
                }
                else {
                    std::cout << "No icon " << *i << " info using " << m_iconTheme->get_example_icon_name() << std::endl;
                }
            }
            if (names.empty()) {
                std::cout << "Names empty " << fileInfo->get_name() << std::endl;
            }
        }
        else {
            std::cout << "Not themed icon " << fileInfo->get_name() << std::endl;
        }
    }
	return Glib::RefPtr<Gdk::Pixbuf>();
}

