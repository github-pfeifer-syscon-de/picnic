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



#include "FileFinder.hpp"
#include "PicnicView.hpp"


FileEntry::FileEntry(const std::string& mime, const Glib::RefPtr<Gio::FileInfo>& fileInfo, const Glib::RefPtr<Gio::File> file)
: m_mime{mime}
, m_fileInfo{fileInfo}
, m_file{file}
{
}

FileEntry::~FileEntry()
{
}

std::string
FileEntry::getMime() const
{
	return m_mime;
}

Glib::RefPtr<Gio::FileInfo>
FileEntry::getFileInfo() const
{
	return m_fileInfo;
}

Glib::RefPtr<Gio::File>
FileEntry::getFile() const
{
	return m_file;
}



FileFinder::FileFinder(PicnicView  *picnicView)
: m_enum()
, m_cancellable()
, m_picnicView(picnicView)
, m_entires()
{
}

FileFinder::~FileFinder()
{
}

// see ImageList for attributes
void
FileFinder::add_file(Glib::RefPtr<Gio::AsyncResult>& result) {
    std::vector<Glib::RefPtr<Gio::FileInfo>> fileInfos = m_enum->next_files_finish(result);
	bool loadNext = false;
    for (auto fileInfo : fileInfos) {
        // may check  fileInfo->get_file_type() == Gio::FileType::FILE_TYPE_SYMBOLIC_LINK points to file
        if (fileInfo->get_file_type() == Gio::FileType::FILE_TYPE_REGULAR) {
            Glib::RefPtr<Gio::File> file = m_enum->get_child(fileInfo);
            //std::cout << "FileFinder::add_file " << fileInfo->get_name() << std::endl;
            //for (auto attr : info->list_attributes()) {
            //    auto type = info->get_attribute_type(attr);
            //    std::cout << "   " << attr << "=" << info->get_attribute_as_string(attr) << std::endl;
            //}

            std::string mime;   // the glib infos might be more accurate than ours, coud query incrementally
            if (fileInfo->has_attribute(G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE)) {  // we asked for it so we can use it
                mime = fileInfo->get_attribute_string(G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE);
            }
            else if (fileInfo->has_attribute(G_FILE_ATTRIBUTE_STANDARD_FAST_CONTENT_TYPE)) {
                mime = fileInfo->get_attribute_string(G_FILE_ATTRIBUTE_STANDARD_FAST_CONTENT_TYPE);
            }
            //std::cout << "File " << info->get_name() << " type " << info->get_file_type() << " mime " << mime << std::endl;
            if (!mime.empty()) {
				FileEntry* entry = new FileEntry(mime, fileInfo, file);
				m_entires.push_back(entry);
				//loadNext = m_picnicView->add_file(mime, fileInfo, file);
            }
			loadNext = true;
        }
		// moving these to dispatch improves stability
        //m_layout->position(TRUE, m_glArea->get_width(), m_glArea->get_height());     // position incrementally, even if retrival is faster than our screen at least for a limited number
        // need if icons shoud be used on_notification_from_worker_thread();
        //m_glArea->queue_draw();
        //std::cout << "files " << m_pictures.size() << std::endl;
    }
    if (fileInfos.size() > 0
     && loadNext) {
        m_enum->next_files_async(sigc::mem_fun(*this, &FileFinder::add_file), m_cancellable, FILE_COUNT);
    }
    else {
        m_enum->close();
        m_picnicView->loadDispatcherEmit();
    }
}

void
FileFinder::load(Glib::RefPtr<Gio::File> fPics)
{
    if (fPics->query_exists()) {    // for the first start we may have no clue
		if (!m_cancellable) {
			m_cancellable = Gio::Cancellable::create();
		}
        m_enum = fPics->enumerate_children(m_cancellable, "standard::*,time::*");   // these shoud be fast and the mime-type we need
        m_enum->next_files_async(sigc::mem_fun(*this, &FileFinder::add_file), m_cancellable, FILE_COUNT);
    }
}

FileEntry* FileFinder::pop_front()
{
	return m_entires.pop_front();
}