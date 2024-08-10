/*
 * Copyright (C) 2018 rpf
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

#include "PicnicWindow.hpp"

class PicnicApp : public Gtk::Application {
public:
    PicnicApp(int arc, char **argv);
    PicnicApp(const PicnicApp& orig);
    virtual ~PicnicApp() = default;


    void on_activate() override;
    void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;
    void on_startup() override;
protected:
    void get_or_create_picnic();
private:
    PicnicWindow *m_picnicWindow;
    void on_action_quit();
};

