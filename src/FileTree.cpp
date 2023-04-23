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

#include "FileTree.hpp"

TreeColumns FileTree::m_treeColumns;

RowReadyNotify::RowReadyNotify()
{
}


RowReadyNotify::~RowReadyNotify()
{
}

FileTree::FileTree(RowReadyNotify& rowReadyNotify)
: Gtk::TreeStore(m_treeColumns)
, m_fileSupport(*this)
, m_rowReadyNotify(rowReadyNotify)
{
}


FileTree::~FileTree()
{
}

void
FileTree::setModelRef(Glib::RefPtr<FileTree>& modelRef)
{
	m_modelRef = modelRef;
	// now we are prepared and can create root
	auto i = append();
	auto row = *i;
	Glib::ustring root("/");		// not win compat
	Glib::RefPtr<Gio::File> fRoot = Gio::File::create_for_path(root);
	//std::cout << "Creating root " << get_path(i) << std::endl;
	Gtk::TreeRowReference rowRef(m_modelRef, get_path(i));
	m_root = std::make_shared<DirNode>(nullptr, rowRef, fRoot);
	setValues(row, root, m_root);
	//m_fileSupport.load(fRoot);
}

void
FileTree::setValues(const Gtk::TreeRow& row
	, Glib::ustring& dir
	, std::shared_ptr<DirNode>& node)
{
	row.set_value(m_treeColumns.m_name, dir);
	row.set_value(m_treeColumns.m_node, node);
	m_rowReadyNotify.notifyRowReady(get_path(row));
}

Glib::RefPtr<FileTree>
FileTree::create(RowReadyNotify& rowReadyNotify)
{
	auto ref = Glib::RefPtr<FileTree>(new FileTree(rowReadyNotify));
	ref->setModelRef(ref);
    return ref;
}

void
FileTree::unref()
{
	m_modelRef.clear();	// allow clean up
}

const char *
FileTree::getAttributes()
{
	return "standard::*,time::*";  // these shoud be fast and the mime-type we need
}

void
FileTree::addRow(Glib::RefPtr<Gio::FileInfo>& fileInfo, Glib::RefPtr<Gio::File>& file)
{
	//std::cout << "FileTree::addRow " << file->get_path() << std::endl;
    if (fileInfo->get_file_type() == Gio::FileType::FILE_TYPE_DIRECTORY) {
		addPath(file);
	}
}

Gtk::TreePath 
FileTree::load(Glib::RefPtr<Gio::File>& dir)
{
	m_selected = dir;
	return addPath(dir);
}

void
FileTree::loadChildren(const Gtk::TreePath& treePath)
{
	auto iter = get_iter(treePath);
	auto row = *iter;
	std::shared_ptr<DirNode> node = row.get_value(m_treeColumns.m_node);
	node->setLoaded(true);
	// exclude no permission e.g. lost+found ...
	//std::cout << "FileTree::loadChildren " << dir->get_path() << " read " << (isReadable(dir) ? "yes" : "no") << std::endl;
	auto dir = node->getFile();
	if (isReadable(dir)) {
		m_fileSupport.load(dir);
	}
}

bool
FileTree::isReadable(Glib::RefPtr<Gio::File>& dir)
{
	Glib::RefPtr<Gio::FileInfo> info = dir->query_info("access::*", Gio::FileQueryInfoFlags::FILE_QUERY_INFO_NONE);
	bool read = info->get_attribute_boolean("access::can-read");
	return read;
}

// check if dir is a descendant of selected
bool
FileTree::isDescendant(Glib::RefPtr<Gio::File>& dir)
{
	Glib::RefPtr<Gio::File> up = m_selected;
	while(true) {
		if (dir->has_parent(up)) {
			//std::cout << "FileTree::isDescendant dir " << dir->get_path() << " up " << up->get_path() << std::endl;
			return true;
		}
		up = up->get_parent();
		if (!up) {
			break;
		}
	}
	return false;
}

// add a path by using existing nodes as far as possible
Gtk::TreePath 
FileTree::addPath(Glib::RefPtr<Gio::File>& dir)
{
	Glib::RefPtr<Gio::File> up = dir;
	std::vector<Glib::RefPtr<Gio::File>> paths;
	while(true) {
		paths.push_back(up);
		up = up->get_parent();
		if (!up) {
			break;
		}
	}
	//std::cout << "path " << dir->get_path() <<  " parts " << path.size() << std::endl;
	auto node = m_root;
	for (int i = paths.size() - 2; i >= 0; --i) {	// -2 to leave out root , as we start with this (and again this is linx only)
		Glib::RefPtr<Gio::File> fPath = paths[i];
		node = node->find(m_modelRef, fPath);
	}
	return node->getRow().get_path();
	//std::cout << "completed "  << std::endl;
}

