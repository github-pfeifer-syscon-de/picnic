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

#pragma once

#include <vector>
#include <gtkmm.h>

#include "Pict.hpp"
#include "ImageArea.hpp"
#include "ApplicationSupport.hpp"
#include "ImageList.hpp"

class ImageFilter : public Gtk::FileFilter
{
public:
    ImageFilter(Gdk::PixbufFormat &format);
    ~ImageFilter();
    Gdk::PixbufFormat getFormat();

    static void addFormats(Gtk::FileChooserDialog &dialog, Glib::ustring &prefered);
    static Glib::RefPtr<ImageFilter> create(Gdk::PixbufFormat &format);

private:
    Gdk::PixbufFormat m_format;
};

class BinView;
class DisplayImage;

class ImageView : public Gtk::Window
{
public:
    ImageView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, int32_t front, std::vector<Glib::RefPtr<Gio::File>>& picts, ApplicationSupport& appSupport);
    virtual ~ImageView();

    void on_hide() override;
    void showFront();
    void setPixbuf(Glib::RefPtr<DisplayImage>& pixbuf);

    static void showView(int32_t front, std::vector<Glib::RefPtr<Gio::File>>& picts, ApplicationSupport& m_appSupport);
    static bool is_mime_gdk_readable(const Glib::ustring& mime);

protected:
    bool on_button_press_event(GdkEventButton* event) override;
    bool on_scroll_event(GdkEventScroll* scroll_event) override;
    bool on_motion_notify_event(GdkEventMotion* event) override;

    Gtk::Menu* build_popup();
    void on_menu_save();
    void save_image(Glib::ustring filename, Gdk::PixbufFormat& format);
    void on_menu_next();
    void on_menu_prev();
    void on_menu_n(gint n);
    void on_menu_view(ViewMode viewMode);
    void on_select();
private:
    ImageArea* m_content;
    int32_t m_front;
    std::vector<Glib::RefPtr<Gio::File>> m_picts;
    ApplicationSupport m_appSupport;
    Glib::RefPtr<Gtk::ScrolledWindow> m_scroll;
    Glib::RefPtr<Gtk::Paned> m_paned;
    Glib::RefPtr<Gtk::TreeView> m_table;
    Glib::RefPtr<ImageList> m_listStore;
    double m_dragStartX,m_dragStartY;
    Glib::RefPtr<Gtk::RadioButton> m_fitRadio;
    Glib::RefPtr<Gtk::RadioButton> m_nativRadio;
    BinView* m_binView;
    bool m_select;
    const char *CONF_GROUP = "view";
    const char *CONF_PREFIX = "view";
    const char *CONF_PATH = "path";
    const char *CONF_FILTER = "filter";
    const char *CONF_VIEW = "view";
    const char *CONF_PANED = "paned";
};

