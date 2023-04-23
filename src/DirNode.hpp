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
#include <map>
#include <memory>

class FileTree;

class DirNode {
public:
    DirNode(DirNode *parent, Gtk::TreeRowReference& treeRow, const Glib::RefPtr<Gio::File>& file);
    virtual ~DirNode();

    std::shared_ptr<DirNode> find(Glib::RefPtr<FileTree>& model, const Glib::RefPtr<Gio::File>& fDir);
    void setLoaded(bool loaded);
    bool isLoaded();
    Glib::RefPtr<Gio::File> getFile();
    Gtk::TreeRowReference& getRow();
private:
    Gtk::TreeRowReference m_treeRow;
    DirNode *m_parent;
    std::map<Glib::ustring, std::shared_ptr<DirNode>> m_children;
    Glib::RefPtr<Gio::File> m_file;
    bool m_loaded;
};
