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
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <iostream>
#include <GL/glu.h>
#include <cmath>
#include <memory>
#include <StringUtils.hpp>
#include <KeyConfig.hpp>

#include "PicnicView.hpp"
#include "DbusWorker.hpp"
#include "RectLayout.hpp"
#include "CubeLayout.hpp"
#include "RollLayout.hpp"
#include "ImageView.hpp"
#include "PicnicWindow.hpp"
#include "FileFinder.hpp"
#include "config.h"

Gray::Gray(uint32_t width, uint32_t height)
: psc::gl::Tex2()
{
    auto size = width * height * 3u;
	auto buf = new uint8_t[size];
    if (buf) {
        std::fill(buf, buf+size, 0x40);
        create(width, height, buf, GL_RGB);
        delete[] buf;
    }
}

GLint
Gray::getTexWrap()
{
	return GL_CLAMP;
}

PicnicView::PicnicView(ApplicationSupport& appSupport, Gtk::Label* entry)
: Scene()
, pictContext{nullptr}
, textContext{nullptr}
, m_glArea{nullptr}
, m_pictures()
, m_Dispatcher()
, m_readyDispatcher()
, m_loadDispatcher()
, m_appSupport{appSupport}
, m_fileFinder{nullptr}
, m_log{psc::log::Log::create("picnic")}
{
    m_Dispatcher.connect(sigc::mem_fun(*this, &PicnicView::on_notification_from_worker_thread));
    m_readyDispatcher.connect(sigc::mem_fun(*this, &PicnicView::on_ready_from_worker_thread));
    m_loadDispatcher.connect(sigc::mem_fun(*this, &PicnicView::on_load_complete));
}


PicnicView::~PicnicView()
{
    // destroy is done with unrealize as it has the right context for gl resouces
}

Position
PicnicView::getIntialPosition()
{
    return Position(0.0f,7.0f,24.0f);
}

Rotational
PicnicView::getInitalAngleDegree()
{
    return Rotational(-17.0f,0.0f,0.0f);
}

Gdk::EventMask
PicnicView::getAddEventMask()
{
    return Gdk::EventMask::POINTER_MOTION_MASK;
}

void
PicnicView::on_ready_from_worker_thread() {
    Glib::RefPtr<Gio::File> fPics = getOpenDir();
    m_fileFinder->load(fPics);
}

void
PicnicView::on_notification_from_worker_thread()
{
    if (m_glArea != nullptr
	 && pictContext != nullptr) {	// this is important !!! otherwise this will crash in dri-lib !!!
        m_glArea->make_current();
        for (auto& p : m_pictures) {
            if (auto lp = p.lease()) {
                lp->create(*pictContext);  // test them all, internally checked if creation is needed
            }
        }
        m_glArea->queue_draw();
        m_log->debug("PicnicView::on_notification_from_worker_thread");
    }
 }

gboolean
PicnicView::init_shaders(Glib::Error &error)
{
    gboolean ret = true;
    try {
        pictContext = new TextContext(GL_QUADS);    // hack: use Text context that has the right setup to display pciture as texture
        ret = pictContext->createProgram(error);
        if (ret) {
            textContext = new MarkContext();
            ret = textContext->createProgram(error);
        }
    }
    catch (const Glib::Error &err) {
        error = err;
        ret = false;
    }
    m_log->info(Glib::ustring::sprintf("PicnicView::init_shaders %s", (ret ? "ok" : "fail")));
    return ret;
}


guint32
PicnicView::getAnimationMs()
{
    return 1000/30;
}

bool
PicnicView::needsAnimation()
{
    //gint64 start = g_get_monotonic_time();
    bool ret = false;
    for (auto& pict : m_pictures) {
        if (auto lpict = pict.lease()) {
            ret |= lpict->advance();
        }
    }
    //gint64 end = g_get_monotonic_time();
    //double dt = ((double)(end - start)) / 1.0e6;
    //std::cout << "PicnicView::needsAnimation " << dt << std::endl;
    return ret;
}

void
PicnicView::init(Gtk::GLArea *glArea)
{
    m_glArea = (NaviGlArea *)glArea;

    m_font = std::make_shared<psc::gl::Font2>("sans-serif");    //
    // this solves our text garbling, but by increasing startup time
    if (textContext) {
        m_font->createDefault(textContext, GL_TRIANGLES);
    }
    m_layout = std::make_shared<RollLayout>(m_glArea);
    m_gray = psc::mem::make_active<Gray>();
	m_fileFinder = new FileFinder(this);
    m_log->info("PicnicView::init Worker::create");
    m_worker = Worker::create(m_Dispatcher, m_readyDispatcher);
}


Glib::RefPtr<Gio::File>
PicnicView::getOpenDir()
{
    auto config = m_appSupport.getConfig();
    if (config->hasKey(PicnicWindow::CONF_GROUP, CONF_PATH)) {
        Glib::ustring pics = config->getString(PicnicWindow::CONF_GROUP, CONF_PATH);
        Glib::RefPtr<Gio::File> fPics = Gio::File::create_for_path(pics);
        if (fPics
         && fPics->query_exists()) {
            return fPics;
        }
    }
    Glib::ustring pics = Glib::get_home_dir();        // just start from somewhere
    Glib::RefPtr<Gio::File> fPics = Gio::File::create_for_path(pics);
    config->setString(PicnicWindow::CONF_GROUP, CONF_PATH, pics);
    m_appSupport.saveConfig();
    return fPics;
}

uint32_t
PicnicView::getMaxFiles()
{
    auto config = m_appSupport.getConfig();
    uint32_t maxFiles = 100;
    if (config->hasKey(PicnicWindow::CONF_GROUP, MAX_FILES)) {
         maxFiles = config->getInteger(PicnicWindow::CONF_GROUP, MAX_FILES);
    }
    else {
        config->setInteger(PicnicWindow::CONF_GROUP, MAX_FILES, maxFiles);
        m_appSupport.saveConfig();
    }
    return maxFiles;
}


// see ImageList for attributes
bool
PicnicView::add_file(const std::string& mime, Glib::RefPtr<Gio::FileInfo>& fileInfo, Glib::RefPtr<Gio::File>& file) {
	if (m_worker->isMimeSupported(mime)) {
		Glib::ustring uName = fileInfo->get_name();   // glib might use utf-8 by default
		auto pict = psc::mem::make_active<Pict>(file, uName, mime, pictContext);
        if (auto lpict = pict.lease()) {
            lpict->setGray(m_gray);
            guint64 modified = fileInfo->get_attribute_uint64(G_FILE_ATTRIBUTE_TIME_MODIFIED);
            lpict->setModified(modified);
#           ifdef DBUS_WORKER_DEBUG
            std::cout << "adding " << lpict->getName() << std::endl;
#           endif
        }
		m_layout->add(pict);
		m_pictures.push_back(pict);
		//getIcon(fileInfo);
		pictContext->addGeometry(pict);
		m_worker->queue(pict);
	}
	else {
        m_log->warn(Glib::ustring::sprintf("rejected mime %s for %s", mime, file->get_basename()));
	}
    uint32_t maxFiles = getMaxFiles();
	return m_pictures.size() < maxFiles;
}

void
PicnicView::loadDispatcherEmit()
{
    m_glArea->make_current();	// this is a game changer!!!
	while (true) {
		FileEntry* entry = m_fileFinder->pop_front();
		if (entry != nullptr) {
			uint32_t maxFiles = getMaxFiles();
			if (m_pictures.size() < maxFiles) {	// limit loading but keep freeing
				std::string mime = entry->getMime();
				Glib::RefPtr<Gio::FileInfo> fileInfo = entry->getFileInfo();
				Glib::RefPtr<Gio::File> file = entry->getFile();
				add_file(mime, fileInfo, file);
			}
			delete entry;
		}
		else {
			break;
		}
	}
	m_log->info("PicnicView::loadDispatcherEmit 2");

	m_layout->position(true, m_glArea->get_width(), m_glArea->get_height());     // position incrementally, even if retrival is faster than our screen at least for a limited number
	// need if icons shoud be used on_notification_from_worker_thread();
	m_glArea->queue_draw();
	m_log->info(Glib::ustring::sprintf("Files %d", m_pictures.size() ));
    m_loadDispatcher.emit();
}

void
PicnicView::on_load_complete()
{
    uint32_t maxFiles = getMaxFiles();
    if (m_pictures.size() >= maxFiles) {
        Glib::ustring msg = Glib::ustring::sprintf("Loading files reached the limit %d. If you are sure your system can handle more you can modify in config and set the value %d.",
                maxFiles, MAX_FILES);
        m_appSupport.showError(msg, Gtk::MESSAGE_WARNING);
    }
}

void
PicnicView::unrealize()
{
    //m_appSupport.saveConfig();
    if (m_worker != nullptr) {
        delete m_worker;
        m_worker = nullptr;
    }
	if (m_fileFinder) {
		delete m_fileFinder;
		m_fileFinder = nullptr;
	}
    clear();
    if (pictContext)
        delete pictContext;
    if (textContext)
        delete textContext;

    m_font.reset();
    m_gray.resetAll();
}

void
PicnicView::draw(Gtk::GLArea *glArea, Matrix &proj, Matrix &view)
{
    //gint64 start = g_get_monotonic_time();
    if (m_layout->sort()) {   // try to keep entries sorted, will check internally if needed and use gui thread
        // this may garble image display, try to position as it seems that finally works
        m_layout->position(false, glArea->get_width(), glArea->get_height());
    }

    pictContext->use();
    psc::gl::checkError("useProgram");
    Color c(1.0f, 1.0f, 1.0f);
    pictContext->setColor(c);

    pictContext->setTexture(0);

    Matrix projView = proj * view;

    pictContext->display(projView);
    psc::gl::checkError("display pict");
    pictContext->unuse();

    textContext->use();
    psc::gl::checkError("initDraw text");
    for (auto& p : m_pictures) {
        if (auto lp = p.lease()) {
            lp->displayText(projView, textContext, m_font, m_layout);
        }
    }
    textContext->unuse();
    //gint64 end = g_get_monotonic_time();
    //double dt = ((double)(end - start)) / 1.0e6;
    //std::cout << "PicnicView::draw " << dt << std::endl;
}


bool
PicnicView::on_motion_notify_event(GdkEventMotion* event, float mx, float my)
{
    bool btn = (event->state & (Gdk::ModifierType::BUTTON1_MASK
                              | Gdk::ModifierType::BUTTON2_MASK
                              | Gdk::ModifierType::BUTTON3_MASK)) != 0;
    if (!btn)  {
        //std::cout << "x: " << mx << " y: " << my << std::endl;
        m_layout->hover(mx, my);
        return true;
    }

    return false;
}

bool
PicnicView::scroll(GdkEventScroll* event)
{
    bool ret = m_layout->scroll(event);
    if (ret) {
        m_glArea->queue_draw();
    }
    return ret;
}

void
PicnicView::on_resize(int width, int height)
{
    m_layout->position(false, width, height);
}


psc::gl::aptrGeom2
PicnicView::on_click_select(GdkEventButton* event, float mx, float my)
{
    auto selected = pictContext->hit2(mx, my);
    return selected;
}

void
PicnicView::on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog)
{
    switch (response_id)
    {
    case Gtk::ResponseType::RESPONSE_OK:
        auto filename = dialog->get_file()->get_path();
        auto fPics = Gio::File::create_for_path(filename);
        if (fPics->query_exists()) {
            clear();
            m_fileFinder->load(fPics);
            auto config = m_appSupport.getConfig();
            config->setString(PicnicWindow::CONF_GROUP, CONF_PATH, filename);
            m_appSupport.saveConfig();
        }
        break;
    //case Gtk::ResponseType::RESPONSE_CANCEL:
    //    break;
    }
    delete dialog;
}

void
PicnicView::clear()
{
    m_layout->clear();
    m_pictures.clear();
}

void
PicnicView::on_action_view()
{
    const int32_t front = m_layout->getFront();
    auto& tiles = m_layout->getTiles();  // these are sorted
    std::vector<Glib::RefPtr<Gio::File>>  picts;
    for (auto i = tiles.begin(); i != tiles.end(); ++i) {
        auto tile = *i;
        auto pict = psc::mem::dynamic_pointer_cast<Pict>(tile);
        if (auto lpict = pict.lease()) {
            picts.push_back(lpict->getPath());
        }
        else {
        	m_log->error(Glib::ustring::sprintf("Failed to cast tile to pict %s", typeid(tile).name()));
        }
    }
	ImageView<Gtk::Window,GtkWindow>::showView(front, picts, m_appSupport);
}
