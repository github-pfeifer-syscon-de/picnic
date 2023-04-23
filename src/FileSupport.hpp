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
#include <giomm.h>
#include <list>

class FileModel {
public:
    ~FileModel();
    virtual void addRow(Glib::RefPtr<Gio::FileInfo>& fileInfo, Glib::RefPtr<Gio::File>& file) = 0;
    virtual const char* getAttributes() = 0;
protected:
    FileModel();
};

class FileSupport {
public:
    FileSupport(FileModel& fileModel);
    virtual ~FileSupport();

    void load(Glib::RefPtr<Gio::File>& dir);
    void add_file(Glib::RefPtr<Gio::AsyncResult>& result);


private:
    Glib::RefPtr<Gio::FileEnumerator> m_enum;
    Glib::RefPtr<Gio::Cancellable> m_cancellable;
    FileModel& m_fileModel;
    std::list<Glib::RefPtr<Gio::File>> m_queue;
    
    static const int FILE_COUNT = 10;    // keep this low in case we load from a slow medium..., 100 has no visible effect on local fs (raspi)
};

