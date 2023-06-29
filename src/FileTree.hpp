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
#include <memory>

#include "FileSupport.hpp"
#include "DirNode.hpp"

class RowReadyNotify
{
public:
    virtual ~RowReadyNotify();
    virtual void notifyRowReady(const Gtk::TreePath& path) = 0;
protected:
    RowReadyNotify();
};

/*
 * model of tree display
 */
class TreeColumns : public Gtk::TreeModel::ColumnRecord
{
public:
    Gtk::TreeModelColumn<Glib::ustring> m_name;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_icon;
    Gtk::TreeModelColumn<std::shared_ptr<DirNode>> m_node;
    TreeColumns()
    {
        add(m_icon);
        add(m_name);
        add(m_node);
    }
};

class FileTree : public Gtk::TreeStore, FileModel {
public:
    virtual ~FileTree() = default;

    static Glib::RefPtr<FileTree> create(RowReadyNotify& rowReadyNotify);
    static TreeColumns m_treeColumns;

    const char *getAttributes() override;
    void addRow(Glib::RefPtr<Gio::FileInfo>& fileInfo, Glib::RefPtr<Gio::File>& file) override;
    Gtk::TreePath load(Glib::RefPtr<Gio::File>& dir);
    void loadChildren(const Gtk::TreePath& treePath);
    void loadChildren(Glib::RefPtr<Gio::File>& dir);
    bool isReadable(Glib::RefPtr<Gio::File>& dir);
    void setValues(const Gtk::TreeRow& row
	, Glib::ustring& dir
	, std::shared_ptr<DirNode>& node);
    void unref();

protected:
    Gtk::TreePath addPath(Glib::RefPtr<Gio::File>& dir);
    bool isDescendant(Glib::RefPtr<Gio::File>& dir);
    void setModelRef(Glib::RefPtr<FileTree>& modelRef);

private:
    FileTree(RowReadyNotify& rowReadyNotify);
    FileSupport m_fileSupport;
    RowReadyNotify& m_rowReadyNotify;
    Glib::RefPtr<Gio::File> m_selected;
    std::shared_ptr<DirNode> m_root;
    Glib::RefPtr<FileTree> m_modelRef;    // as this is exact our model it will either live forever or may get invalid ...
};

