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

#include "FileTree.hpp"

class FileTreeView : public Gtk::TreeView, public RowReadyNotify {
public:
    FileTreeView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~FileTreeView();
private:

    void notifyRowReady(const Gtk::TreePath& path) override;
    void onTreeRowExpanded(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);

};

