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
#include <iostream>
#include <thread>
#include <future>
#include <KeyConfig.hpp>

#include <gtkmm.h>

#include "PicnicWindow.hpp"
#include "PicnicApp.hpp"



PicnicWindow::PicnicWindow(PicnicApp *picnicApp)
: Gtk::ApplicationWindow()
, m_config{std::make_shared<KeyConfig>("picnic.conf")}
, m_appSupport{m_config}
{
    auto pix = Gdk::Pixbuf::create_from_resource(picnicApp->get_resource_base_path() + "/picnic.png");
    if (pix) {
        set_icon(pix);
    }
    m_appSupport.setApplication(picnicApp);
    m_appSupport.addWindow(this, CONF_PREFIX, 512, 512, CONF_GROUP);
    Gtk::Label* text = nullptr;
    m_picnicView = new PicnicView(m_appSupport, text);
    auto naviArea = Gtk::make_managed<NaviGlArea>(m_picnicView);
    add(*naviArea);
    //set_decorated(FALSE);
    add_action("open", sigc::mem_fun(*this, &PicnicWindow::on_action_open));
    add_action("view", sigc::mem_fun(*m_picnicView, &PicnicView::on_action_view));
    add_action("about", sigc::mem_fun(*this, &PicnicWindow::on_action_about));

    //set_default_size(512, 512);
    show_all_children();
}


PicnicWindow::~PicnicWindow()
{
    if (m_picnicView)
        delete m_picnicView;
    m_picnicView = nullptr;
}

ApplicationSupport&
PicnicWindow::getAppSupport()
{
	return m_appSupport;
}

void
PicnicWindow::on_hide()
{
    m_appSupport.removeWindow(this, CONF_PREFIX, CONF_GROUP);
}

void
PicnicWindow::on_action_preferences()
{
    //Gtk::Dialog *dlg = m_monglView.monitors_config();
    //dlg->set_transient_for(*this);
    //dlg->run();
}

void
PicnicWindow::on_action_open()
{
    Glib::RefPtr<Gio::File> openDir = m_picnicView->getOpenDir();
    auto dialog = new Gtk::FileChooserDialog("Please choose a directory",
          Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog->set_modal(true);
    dialog->set_current_folder(openDir->get_path());
    dialog->signal_response().connect(sigc::bind(
        sigc::mem_fun(m_picnicView, &PicnicView::on_file_dialog_response), dialog));

    m_appSupport.addDialogYesNo(*dialog);

	//Show the dialog and wait for a user response:
	dialog->show();
}

void
PicnicWindow::on_action_about()
{
    auto refBuilder = Gtk::Builder::create();
    try {
		auto appl = m_appSupport.getApplication();
        refBuilder->add_from_resource(
            appl->get_resource_base_path() + "/abt-dlg.ui");
        auto object = refBuilder->get_object("abt-dlg");
        auto abtdlg = Glib::RefPtr<Gtk::AboutDialog>::cast_dynamic(object);
        if (abtdlg) {
            Glib::RefPtr<Gdk::Pixbuf> pix = Gdk::Pixbuf::create_from_resource(
                    appl->get_resource_base_path() +"/picnic.png");
            abtdlg->set_logo(pix);
            abtdlg->set_transient_for(*this);
            abtdlg->run();
            abtdlg->hide();
        }
		else {
            std::cerr << "PicnicWindow::on_action_about(): No \"abt-dlg\" object in abt-dlg.ui"
                << std::endl;
		}
    }
	catch (const Glib::Error& ex) {
        std::cerr << "PicnicWindow::on_action_about(): " << ex.what() << std::endl;
    }

}
