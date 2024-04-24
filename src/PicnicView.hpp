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

#include <gtkmm.h>
#include <string>
#include <list>
#include <TextContext.hpp>
#include <MarkContext.hpp>
#include <Font2.hpp>
#include <Geom2.hpp>
#include <NaviGlArea.hpp>
#include <Scene.hpp>
#include <ApplicationSupport.hpp>
#include <memory>
#include <Log.hpp>


#include "Pict.hpp"
#include "Worker.hpp"

class Layout;
class FileFinder;

class Gray
: public psc::gl::Tex2
{
public:
    Gray(uint32_t width = 16, uint32_t height = 16);
    GLint getTexWrap() override;
};

class PicnicView
: public Scene
{
public:
    PicnicView(ApplicationSupport& appSupport, Gtk::Label *entry);
    virtual ~PicnicView();
    Position getIntialPosition() override;
    Rotational getInitalAngleDegree() override;
    void unrealize() override;
    void on_ready_from_worker_thread();
    void on_notification_from_worker_thread();
    gboolean init_shaders(Glib::Error &error) override;
    void init(Gtk::GLArea *glArea) override;
    void draw(Gtk::GLArea *glArea, Matrix &proj, Matrix &view) override;
    guint32 getAnimationMs() override;
    bool needsAnimation() override;
    Gdk::EventMask getAddEventMask() override;
    bool on_motion_notify_event(GdkEventMotion* event, float mx, float my) override;
    void on_resize(int width, int height) override;
    psc::gl::aptrGeom2 on_click_select(GdkEventButton* event, float mx, float my) override;
    void on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog);
    bool scroll(GdkEventScroll* event) override;
    Glib::RefPtr<Gio::File> getOpenDir();
    void on_load_complete();
    void on_action_view();

    void loadDispatcherEmit();
    bool add_file(const std::string& mime, Glib::RefPtr<Gio::FileInfo>& fileInfo, Glib::RefPtr<Gio::File>& file);

protected:

private:
    uint32_t getMaxFiles();
    void clear();

    TextContext* pictContext;
    MarkContext* textContext;
    NaviGlArea* m_glArea;
    psc::gl::ptrFont2 m_font;
    gboolean view_update();
    std::list<psc::mem::active_ptr<Pict>> m_pictures;
    Worker* m_worker;
    psc::mem::active_ptr<Gray> m_gray;
    Glib::Dispatcher m_Dispatcher;  // used for thread notification
    Glib::Dispatcher m_readyDispatcher;     // used on worker ready
    Glib::Dispatcher m_loadDispatcher;      // used on load completed
    std::shared_ptr<Layout> m_layout;
    ApplicationSupport& m_appSupport;
    FileFinder* m_fileFinder;
    std::shared_ptr<psc::log::Log> m_log;
    const char *CONF_PATH = "path";
    const char *MAX_FILES = "maxFiles";
};

