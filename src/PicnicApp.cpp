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
: Gtk::Application(argc, argv, "de.pfeifer_syscon.picnic", Gio::ApplicationFlags::APPLICATION_HANDLES_COMMAND_LINE )	// , Gio::APPLICATION_HANDLES_COMMAND_LINE kill default handling
, m_picnicListWin{nullptr}
, m_picnicWindow{nullptr}
, m_show_gtkPictureManager{false}
, m_fileName()
{
    signal_command_line().connect(sigc::mem_fun(*this, &PicnicApp::on_cmd), false);
}


PicnicApp::~PicnicApp()
{
}


void
PicnicApp::on_activate()
{
	if (m_show_gtkPictureManager) {
		auto builder = Gtk::Builder::create();
		try {
			builder->add_from_resource(get_resource_base_path() + "/win.ui");
			builder->get_widget_derived("IconView", m_picnicListWin, this);
			add_window(*m_picnicListWin);
			m_picnicListWin->show();
		}
		catch (const Glib::Error &ex) {
			std::cerr << "Unable to load win.ui: " << ex.what() << std::endl;
		}
	}
	else {
		m_picnicWindow = new PicnicWindow(this);
		add_window(*m_picnicWindow);
		m_picnicWindow->show();

		if (!m_fileName.empty()) {
			const int32_t front = 0;
			std::vector<Glib::RefPtr<Gio::File>>  picts;
			Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(m_fileName);
			picts.push_back(file);

			ImageView::showView(front, picts, m_picnicWindow->getAppSupport());
		}
	}
}


void
PicnicApp::on_action_quit()
{
	if (m_picnicListWin) 
		m_picnicListWin->hide();	
	if (m_picnicWindow)
		m_picnicWindow->hide();

    // Not really necessary, when Gtk::Widget::hide() is called, unless
    // Gio::Application::hold() has been called without a corresponding call
    // to Gio::Application::release().
    quit();
}

Glib::ustring
PicnicApp::getFilename()
{
	return m_fileName;
}


int
PicnicApp::on_cmd(const Glib::RefPtr<Gio::ApplicationCommandLine> &cmd) {
	Glib::OptionContext ctx;
	Glib::OptionGroup group("options", "main options");
	Glib::OptionEntry entry;
	entry.set_long_name("nogl");
	entry.set_description("show gtk-interface.");
	group.add_entry(entry, m_show_gtkPictureManager);
	ctx.add_group(group);
	// add GTK options, --help-gtk, etc
	Glib::OptionGroup gtkgroup(gtk_get_option_group(true));
	ctx.add_group(gtkgroup);
	int argc;
	char **argv = cmd->get_arguments(argc);
	ctx.parse(argc, argv);
	if (argc > 1) {
		char *fileName = argv[1];
		if (fileName != nullptr) {
			m_fileName = Glib::ustring(fileName);
		}
	}
	activate();
    return 0;
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