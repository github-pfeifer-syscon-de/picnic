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

#include "PicnicView.hpp"
#include "Pict.hpp"

class PicnicApp;
class PicnicView;
class KeyConfig;

class PicnicWindow : public Gtk::ApplicationWindow {
public:
    PicnicWindow(PicnicApp *picnicApp);
    virtual ~PicnicWindow();

    void on_action_preferences();
    void on_action_open();

    ApplicationSupport& getAppSupport();
    static constexpr auto CONF_GROUP{"picnic"};

protected:
    void on_hide();
    void on_action_about();
private:
    PicnicView *m_picnicView;
    std::shared_ptr<KeyConfig> m_config;
    ApplicationSupport m_appSupport;

    static constexpr auto CONF_PREFIX{"win"};
};
