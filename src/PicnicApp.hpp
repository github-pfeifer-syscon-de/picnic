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

#include "PicnicListWin.hpp"
#include "PicnicWindow.hpp"

class PicnicApp : public Gtk::Application {
public:
    PicnicApp(int arc, char **argv);
    PicnicApp(const PicnicApp& orig);
    virtual ~PicnicApp();

    Glib::ustring getFilename();

    void on_activate() override;
    void on_startup() override;
protected:
    int on_cmd(const Glib::RefPtr<Gio::ApplicationCommandLine> &cmd);
private:
    //PicnicWindow *m_picnicAppWindow;
    PicnicListWin *m_picnicListWin;
    PicnicWindow *m_picnicWindow;
    bool m_show_gtkPictureManager;
    Glib::ustring m_fileName;
    void on_action_quit();
};

