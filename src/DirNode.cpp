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

#include "DirNode.hpp"
#include "FileTree.hpp"

DirNode::DirNode(DirNode *parent, Gtk::TreeRowReference& treeRow, const Glib::RefPtr<Gio::File>& file)
: m_treeRow{treeRow}
, m_parent{parent}
, m_children()
, m_file{file}
, m_loaded{false}
{
}

// this shoud be called for the right parent in tree
std::shared_ptr<DirNode>
DirNode::find(Glib::RefPtr<FileTree>& model, const Glib::RefPtr<Gio::File>& fDir)
{
	Glib::ustring dir = fDir->get_basename();
	//std::cout << "DirNode::find " << fDir->get_path() << std::endl;
	auto node = m_children.find(dir);
	if (node == m_children.end()) {
		Gtk::TreeIter titer = model->get_iter(m_treeRow.get_path())->children().begin();
		while (titer) {
			auto row = *titer;
			Glib::ustring name = row.get_value(FileTree::m_treeColumns.m_name);
			if (name.compare(dir) >= 0) {	// as entries should be sorted
				//std::cout << name << " is larger " << dir << std::endl;
				break;
			}
			++titer;
		}
		// insert will handle insert before end well
		titer = model->insert(titer);
		auto childRow = *titer;
		Gtk::TreeRowReference rowRef(model, model->get_path(titer));
		auto dirNode = std::make_shared<DirNode>(this, rowRef, fDir);
		model->setValues(childRow, dir, dirNode);
		auto newNode = std::pair<Glib::ustring, std::shared_ptr<DirNode>>(dir, dirNode);
		auto ins = m_children.insert(newNode);
		node = ins.first;
	}
	return node->second;
}

void
DirNode::setLoaded(bool loaded)
{
	m_loaded = loaded;
}

bool
DirNode::isLoaded()
{
	return m_loaded;
}

Glib::RefPtr<Gio::File>
DirNode::getFile()
{
	return m_file;
}

Gtk::TreeRowReference&
DirNode::getRow()
{
	return m_treeRow;
}