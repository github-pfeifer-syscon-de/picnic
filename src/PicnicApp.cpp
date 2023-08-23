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

#include <gtkmm.h>
#include <iostream>
#include <exception>
#include <thread>
#include <future>

#include "PicnicApp.hpp"
#include "ImageView.hpp"


PicnicApp::PicnicApp(int argc, char **argv)
: Gtk::Application(argc, argv, "de.pfeifer_syscon.picnic", Gio::ApplicationFlags::APPLICATION_HANDLES_OPEN )	// , Gio::APPLICATION_HANDLES_COMMAND_LINE kill default handling
, m_picnicWindow{nullptr}
{
}


void
PicnicApp::get_or_create_picnic()
{
    // The application has been asked to open some files,
    // so let's open a new view for each one.
    PicnicWindow* appwindow = nullptr;
    auto windows = get_windows();
    if (windows.size() > 0) {
        appwindow = dynamic_cast<PicnicWindow*>(windows[0]);
    }

    if (!appwindow) {
        m_picnicWindow = new PicnicWindow(this);
        add_window(*m_picnicWindow);
        m_picnicWindow->show();
    }
    else {
        m_picnicWindow = appwindow;
    }
}

void
PicnicApp::on_activate()
{
    // either on_activate is called (no args)
    get_or_create_picnic(); // on instance shoud be sufficent
}

void
PicnicApp::on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint)
{
    // or on_open is called (some args)
    try {
        get_or_create_picnic();

        if (!files.empty()) {
            std::vector<Glib::RefPtr<Gio::File>>  picts;
            for (auto file : files) {
                picts.push_back(file);
            }
            const int32_t front = 0;
            // this opens a single dialog with paging ...
            ImageView<Gtk::Window,GtkWindow>::showView(front, picts, m_picnicWindow->getAppSupport());
        }
    }
    catch (const Glib::Error& ex) {
        std::cerr << "PicnicApp::on_open(): " << ex.what() << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "PicnicApp::on_open(): " << ex.what() << std::endl;
    }
}

void
PicnicApp::on_action_quit()
{
	if (m_picnicWindow)
		m_picnicWindow->hide();

    // Not really necessary, when Gtk::Widget::hide() is called, unless
    // Gio::Application::hold() has been called without a corresponding call
    // to Gio::Application::release().
    quit();
}

void
PicnicApp::on_startup()
{
    // Call the base class's implementation.
    Gtk::Application::on_startup();

    // Add actions and keyboard accelerators for the application menu.
    add_action("quit", sigc::mem_fun(*this, &PicnicApp::on_action_quit));
    set_accel_for_action("app.quit", "<Ctrl>Q");

    auto refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_resource(get_resource_base_path() + "/app-menu.ui");
        auto object = refBuilder->get_object("appmenu");
        auto app_menu = Glib::RefPtr<Gio::MenuModel>::cast_dynamic(object);
        if (app_menu)
            set_app_menu(app_menu);
        else
            std::cerr << "GlSceneApp::on_startup(): No \"appmenu\" object in app_menu.ui"
                << std::endl;
    }
    catch (const Glib::Error& ex) {
        std::cerr << "GlSceneApp::on_startup(): " << ex.what() << std::endl;
        return;
    }

}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");      // use locale formating
    Glib::init();
    auto app = PicnicApp(argc, argv);

    return app.run();
}