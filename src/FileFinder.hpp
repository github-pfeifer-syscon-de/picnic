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

#include "PicnicView.hpp"
#include "ConcurrentCollections.hpp"

class PicnicView;

class FileEntry {
public:
    FileEntry(const std::string& m_mime, const Glib::RefPtr<Gio::FileInfo>& m_fileInfo, const Glib::RefPtr<Gio::File> m_file);
    ~FileEntry();

    std::string getMime() const;
    Glib::RefPtr<Gio::FileInfo> getFileInfo() const;
    Glib::RefPtr<Gio::File> getFile() const;

private:
    const std::string m_mime;
    const Glib::RefPtr<Gio::FileInfo> m_fileInfo;
    const Glib::RefPtr<Gio::File> m_file;

};

class FileFinder {
public:
    FileFinder(PicnicView  *picnicView);
    virtual ~FileFinder();

    void add_file(Glib::RefPtr<Gio::AsyncResult>& result);
    void load(Glib::RefPtr<Gio::File> fPics);
    FileEntry* pop_front();
private:
    Glib::RefPtr<Gio::FileEnumerator> m_enum;
    Glib::RefPtr<Gio::Cancellable> m_cancellable;
    static const int FILE_COUNT = 4;    // keep this low if we load from a slow medium...
    PicnicView  *m_picnicView;
    TListConcurrent<FileEntry *> m_entires;
};

