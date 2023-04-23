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



#include "FileTreeView.hpp"

FileTreeView::FileTreeView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::TreeView(cobject)
{
	signal_row_expanded().connect(
		sigc::mem_fun(*this, &FileTreeView::onTreeRowExpanded));

}

FileTreeView::~FileTreeView()
{
}



void
FileTreeView::onTreeRowExpanded(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path)
{
	auto fileTree = Glib::RefPtr<FileTree>::cast_dynamic(get_model());
	auto row = *(fileTree->get_iter(path));
	auto node = row->get_value(FileTree::m_treeColumns.m_node);
	if (!node->isLoaded()) {
		//std::cout << "PicnicListWin::onTreeRowExpanded " << path.to_string() << " file " << fPath->get_path() << std::endl;
		fileTree->loadChildren(path);
	}
	else {
		// load children of children
		for (auto chld : row->children()) {
			node = chld->get_value(FileTree::m_treeColumns.m_node);
			if (!node->isLoaded()) {
				fileTree->loadChildren(fileTree->get_path(chld));
			}
		}
	}
}

void
FileTreeView::notifyRowReady(const Gtk::TreePath& path)
{
	auto fileTree = Glib::RefPtr<FileTree>::cast_dynamic(get_model());
	auto parent = path;
	parent.up();
	if (parent && row_expanded(parent)) {
		fileTree->loadChildren(path);
	}
}

