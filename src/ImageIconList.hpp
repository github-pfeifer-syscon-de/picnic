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

#include <gtkmm.h>
#include <FileSupport.hpp>

/*
 * model of image icon display
 */
class ImageListColumns : public Gtk::TreeModel::ColumnRecord
{
public:
    Gtk::TreeModelColumn<Glib::ustring> m_name; // referenced by index in PicnicListWin
    Gtk::TreeModelColumn<Glib::ustring> m_shortName;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_icon;
    Gtk::TreeModelColumn<Glib::RefPtr<Gio::File>> m_file;
    ImageListColumns()
    {
        add(m_name);
        add(m_shortName);
        add(m_icon);
        add(m_file);
    }
};

class ImageIconList : public Gtk::ListStore, FileModel {
public:
    virtual ~ImageIconList();

    static Glib::RefPtr<ImageIconList> create();
    static ImageListColumns m_imageListColumns;

    const char* getAttributes() override;
    void addRow(Glib::RefPtr<Gio::FileInfo>& fileInfo, Glib::RefPtr<Gio::File>& file) override;
    Glib::RefPtr<Gdk::Pixbuf> getIcon(Glib::RefPtr<Gio::FileInfo>& fileInfo);
    void setIconTheme(Glib::RefPtr<Gtk::IconTheme>& iconTheme);
    void load(Glib::RefPtr<Gio::File>& dir);
    std::vector<Glib::RefPtr<Gio::File>> getFiles();
private:
    ImageIconList();
    Glib::RefPtr<Gtk::IconTheme> m_iconTheme;
    FileSupport m_fileSupport;
};

