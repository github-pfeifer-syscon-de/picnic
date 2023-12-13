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

#include "FileSupport.hpp"


FileSupport::FileSupport(FileModel &fileModel)
: m_fileModel{fileModel}
, m_queue()
{
}

void
FileSupport::load(Glib::RefPtr<Gio::File>& fPics)
{
	if (fPics->query_exists()) {    // for the first start we may have no clue
		if (!m_enum || m_enum->is_closed()) {
			if (!m_cancellable)
				m_cancellable = Gio::Cancellable::create();
		    m_enum = fPics->enumerate_children(m_cancellable, m_fileModel.getAttributes(), Gio::FileQueryInfoFlags::FILE_QUERY_INFO_NOFOLLOW_SYMLINKS);
			m_enum->next_files_async(sigc::mem_fun(*this, &FileSupport::add_file), m_cancellable, FILE_COUNT);
		}
		else {
			m_queue.push_back(fPics);
		}
    }
}

// see ImageList for attributes
void
FileSupport::add_file(Glib::RefPtr<Gio::AsyncResult>& result) {
    std::vector<Glib::RefPtr<Gio::FileInfo>> fileInfos = m_enum->next_files_finish(result);
    for (auto fileInfo : fileInfos) {
		//std::cout <<  "FileSupport::add_file info " << fileInfo->get_display_name() << std::endl;
        Glib::RefPtr<Gio::File> file = m_enum->get_child(fileInfo);
		//std::cout <<  "FileSupport::add_file file " << file->get_basename() << std::endl;
		m_fileModel.addRow(fileInfo, file);
    }
    if (fileInfos.size() > 0) {
        m_enum->next_files_async(sigc::mem_fun(*this, &FileSupport::add_file), m_cancellable, FILE_COUNT);
    }
    else {
        m_enum->close();
		if (!m_queue.empty()) {
			Glib::RefPtr<Gio::File> dir = m_queue.front();
			m_queue.pop_front();
			load(dir);
		}
        //m_loadDispatcher.emit();
    }
}
