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
#include <iomanip>

#include "BinView.hpp"
#include "ImageView.hpp"
#include "Pict.hpp"
#include "PicnicView.hpp"
#include "ImageOptionDialog.hpp"
#include "DisplayImage.hpp"

ImageFilter::ImageFilter(Gdk::PixbufFormat &format)
: Gtk::FileFilter()
, m_format{format}
{
}

ImageFilter::~ImageFilter()
{

}

Glib::RefPtr<ImageFilter>
ImageFilter::create(Gdk::PixbufFormat &format)
{
    return Glib::RefPtr<ImageFilter>(new ImageFilter(format));
}

Gdk::PixbufFormat
ImageFilter::getFormat()
{
    return m_format;
}

void
ImageFilter::addFormats(Gtk::FileChooserDialog &dialog, Glib::ustring &prefFormat)
{
    for (auto format : Gdk::Pixbuf::get_formats()) {
        if (format.is_writable()) {
            auto filter_image = ImageFilter::create(format);
            filter_image->set_name(format.get_description());
            for (Glib::ustring mime : format.get_mime_types()) {
                filter_image->add_mime_type(mime);
            }
            for (Glib::ustring ext : format.get_extensions()) {
                Glib::ustring pattern("*.");
                pattern += ext;
                filter_image->add_pattern(pattern);
            }
            dialog.add_filter(filter_image);
            if (format.get_name() == prefFormat) {
                dialog.set_filter(filter_image);
            }
        }
    }
}


ImageView::ImageView(
    BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
    int32_t front, std::vector<Glib::RefPtr<Gio::File>>& picts, ApplicationSupport& appSupport)
: Gtk::Window(cobject)       //Calls the base class constructor
, m_content(nullptr)
, m_front(front)
, m_picts(picts)
, m_appSupport(&appSupport)
, m_scroll()
, m_paned()
, m_table()
, m_listStore()
, m_dragStartX(0.0)
, m_dragStartY(0.0)
, m_fitRadio()
, m_nativRadio()
, m_binView(nullptr)
, m_select(false)
{
    m_appSupport.addWindow(this, CONF_PREFIX, 640, 480, CONF_GROUP);

    auto scrollObj = builder->get_object("imageScroll");
    m_scroll = Glib::RefPtr<Gtk::ScrolledWindow>::cast_dynamic(scrollObj);
    auto panedObj = builder->get_object("paned");
    m_paned = Glib::RefPtr<Gtk::Paned>::cast_dynamic(panedObj);
    builder->get_widget_derived("binDraw", m_binView);
    builder->get_widget_derived("imageDraw", m_content, m_appSupport, this);
    auto tableObj = builder->get_object("viewTable");
    m_table = Glib::RefPtr<Gtk::TreeView>::cast_dynamic(tableObj);
    m_listStore = ImageList::create();
    m_table->append_column("Name", ImageList::m_variableColumns.m_name);
    m_table->append_column("Value", ImageList::m_variableColumns.m_value);
    m_table->set_model(m_listStore);

    ViewMode viewMode;
    Glib::KeyFile* config = m_appSupport.getConfig();
    if (config->has_group(CONF_GROUP)
     && config->has_key(CONF_GROUP, CONF_VIEW)) {
        gint iViewMode = config->get_integer(CONF_GROUP, CONF_VIEW);
        viewMode = (ViewMode)iViewMode;
        m_content->setViewMode(viewMode);
    }
    else {
        viewMode = m_content->getViewMode();
    }
    if (config->has_group(CONF_GROUP)
     && config->has_key(CONF_GROUP, CONF_PANED)) {
        gint iDivPos = config->get_integer(CONF_GROUP, CONF_PANED);
        m_paned->set_position(iDivPos);
    }

    auto fitObj = builder->get_object("fit");
    m_fitRadio = Glib::RefPtr<Gtk::RadioButton>::cast_dynamic(fitObj);
    m_fitRadio->set_sensitive(true);
    auto orgObj = builder->get_object("nativ");
    m_nativRadio = Glib::RefPtr<Gtk::RadioButton>::cast_dynamic(orgObj);
    m_nativRadio->set_sensitive(true);
    Gtk::RadioButtonGroup viewGroup;
    m_nativRadio->set_group(viewGroup);
    m_fitRadio->set_group(viewGroup);
    switch (viewMode) {
        case ViewMode::NATIVE:
            m_nativRadio->set_active(true);
            break;
        case ViewMode::FIT:
            m_fitRadio->set_active(true);
            break;
    }
    // will be fired for actived & deactivated ... so one handler will suffice. Have to check if n > 2
    m_nativRadio->signal_toggled().connect(
        sigc::bind(sigc::mem_fun(*this, &ImageView::on_menu_view), ViewMode::NATIVE));
    //fitRadio->signal_clicked().connect(
    //    sigc::bind(sigc::mem_fun(*this, &ImageView::on_menu_view), ViewMode::FIT));

    auto prevObj = builder->get_object("prev");
    auto prevBtn = Glib::RefPtr<Gtk::Button>::cast_dynamic(prevObj);
    prevBtn->signal_clicked().connect(
        sigc::mem_fun(*this, &ImageView::on_menu_prev));
    prevBtn->set_sensitive(true);
    auto nextObj = builder->get_object("next");
    auto nextBtn = Glib::RefPtr<Gtk::Button>::cast_dynamic(nextObj);
    nextBtn->signal_clicked().connect(
        sigc::mem_fun(*this, &ImageView::on_menu_next));
    nextBtn->set_sensitive(true);

    //show_all_children();
    add_events(Gdk::EventMask::BUTTON_PRESS_MASK
             | Gdk::EventMask::BUTTON_RELEASE_MASK
             | Gdk::EventMask::SCROLL_MASK
             | Gdk::EventMask::BUTTON_MOTION_MASK
             | Gdk::EventMask::POINTER_MOTION_MASK);

    showFront();
}

ImageView::~ImageView()
{
}

void 
ImageView::showView(int32_t front, std::vector<Glib::RefPtr<Gio::File>>& picts, ApplicationSupport& m_appSupport)
{
    auto builder = Gtk::Builder::create();
    try {
        Gtk::Application* appl = m_appSupport.getApplication();
        builder->add_from_resource(appl->get_resource_base_path() + "/view.ui");
        ImageView* viewWin;
        builder->get_widget_derived("ImageView", viewWin, front, picts, m_appSupport);
        viewWin->show();
    }
    catch (const Glib::Error &ex) {
        std::cerr << "Unable to load view.ui: " << ex.what() << std::endl;
    }
}

void
ImageView::showFront()
{
    Glib::RefPtr<Gio::File> file = m_picts[m_front];
    set_title(file->get_basename());
    m_content->setFile(file);
    m_listStore->fillList(file);
}

void
ImageView::setPixbuf(Glib::RefPtr<DisplayImage>& pixbuf)
{
	m_binView->setPixbuf(pixbuf);
	m_listStore->fillList(pixbuf);
	m_table->expand_all();
}

void
ImageView::on_hide()
{
    Glib::KeyFile* config = m_appSupport.getConfig();
    gint iDivPos = m_paned->get_position();
    config->set_integer(CONF_GROUP, CONF_PANED, iDivPos);   // will be saved with window

    m_appSupport.removeWindow(this, CONF_PREFIX, CONF_GROUP);

    Gtk::Window::on_hide();
    delete this;    // this might not be the nicest way, but it works
}

bool
ImageView::on_motion_notify_event(GdkEventMotion* event)
{
    //std::cout << "motion state 0x" <<  std::hex << event->state
    //          << "btn1 0x" << Gdk::ModifierType::BUTTON1_MASK
    //          << std::endl << std::dec;

	bool drag = event->state & Gdk::ModifierType::BUTTON1_MASK;
    if (m_select) {
		auto cursor = m_content->mouse_pressed(event->x, event->y, drag);
		if (!cursor) {
			cursor = Gdk::Cursor::create(get_display(), Gdk::ARROW);
		}
		get_window()->set_cursor(cursor);
        return true;
    }
	else if (drag) {
        double dx = m_dragStartX - event->x;
        double dy = m_dragStartY - event->y;
        //std::cout << "motion dx " << dx << " dy " << dy << std::endl;

        Glib::RefPtr<Gtk::Adjustment> hadjust = m_scroll->get_hadjustment();
        hadjust->set_value(hadjust->get_value() + dx);
        Glib::RefPtr<Gtk::Adjustment> vadjust = m_scroll->get_vadjustment();
        vadjust->set_value(vadjust->get_value() + dy);

        m_dragStartX = event->x;
        m_dragStartY = event->y;
        return true;
    }
    return false;
}

bool
ImageView::on_scroll_event(GdkEventScroll* scroll_event)
{
    // may work for devices supporting scroll (tablet?)
    //std::cout << "scroll dx " << scroll_event->delta_x << " dy " << scroll_event->delta_y << std::endl;

    Glib::RefPtr<Gtk::Adjustment> hadjust = m_scroll->get_hadjustment();
    hadjust->set_value(hadjust->get_value() + scroll_event->delta_x);
    Glib::RefPtr<Gtk::Adjustment> vadjust = m_scroll->get_vadjustment();
    vadjust->set_value(vadjust->get_value() + scroll_event->delta_y);
    //Gtk::Viewport* view = dynamic_cast<Gtk::Viewport *>(m_content->get_parent());
    //if (view) {
    //    view->
    //}
    return true;
    //return false;
}

bool
ImageView::on_button_press_event(GdkEventButton* event)
{
    if (event->button == GDK_BUTTON_PRIMARY) {
        m_dragStartX = event->x;
        m_dragStartY = event->y;
    }
    if (gdk_event_triggers_context_menu((GdkEvent*)event))  {       // event->button == GDK_BUTTON_SECONDARY
        Gtk::Menu* popupMenu = build_popup();
        // deactivate prevent item signals to get generated ...
        // signal_unrealize will never get generated
        popupMenu->attach_to_widget(*this);     // this does the trick and calls the destructor
        popupMenu->popup(event->button, event->time);

        return true; // It has been handled.
    }
    return false;
}

Gtk::Menu *
ImageView::build_popup()
{
    // managed works when used with attach ...
    auto pMenuPopup = Gtk::make_managed<Gtk::Menu>();
    if (m_content->getDisplayImage()) {
        auto save = Gtk::make_managed<Gtk::MenuItem>("_Save", true);
        save->signal_activate().connect(sigc::mem_fun(*this, &ImageView::on_menu_save));
        pMenuPopup->append(*save);
    }
    if (m_picts.size() > 0) {
        auto last = Gtk::make_managed<Gtk::MenuItem>("_View", true);
        pMenuPopup->append(*last);
        auto subMenu = Gtk::make_managed<Gtk::Menu>();
        last->set_submenu(*subMenu);
        int32_t n = 0;
        for (auto p : m_picts)  {
            Glib::ustring name = p->get_basename();
            auto lastN = Gtk::make_managed<Gtk::MenuItem>(name, true);
            lastN->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &ImageView::on_menu_n), n));
            subMenu->append(*lastN);
            ++n;
        }
    }
    auto select = Gtk::make_managed<Gtk::CheckMenuItem>("_Select", true);
    select->set_active(m_select);
    select->signal_activate().connect(sigc::mem_fun(*this, &ImageView::on_select));
    pMenuPopup->append(*select);

    pMenuPopup->show_all();
    return pMenuPopup;
}

void
ImageView::on_select()
{
    m_select = !m_select;
	if (!m_select) {
		if (m_appSupport.askYesNo("Crop Image?")) {
			m_content->crop();
		}
	}
    m_content->setSelected(m_select);
}

void
ImageView::on_menu_save()
{
    Gtk::FileChooserDialog dialog("Save file"
								, Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
    m_appSupport.addDialogYesNo(dialog);

    auto config = m_appSupport.getConfig();
    if (config->has_group(CONF_GROUP)
     && config->has_key(CONF_GROUP, CONF_PATH)) {
        Glib::ustring path = config->get_string(CONF_GROUP, CONF_PATH);
        Glib::RefPtr<Gio::File> fPath = Gio::File::create_for_path(path);
        Glib::RefPtr<Gio::File> fParent = fPath->get_parent();     // as we stored selected file
        if (fParent->query_exists()) {
            dialog.set_current_folder(fParent->get_path());
        }
    }

    //Add filters, so that only certain file types can be selected:
    Glib::ustring prefFormat;
    if (config->has_group(CONF_GROUP)
     && config->has_key(CONF_GROUP, CONF_FILTER)) {
        prefFormat = config->get_string(CONF_GROUP, CONF_FILTER);
    }
    if (prefFormat.empty()) {   // otherwise we will get bmp argh...
        prefFormat = "jpeg";
    }
    ImageFilter::addFormats(dialog, prefFormat);
    int result = dialog.run();
	dialog.hide();	// only one open dialog at a time (prevent hiding newer instance)
    if (result == Gtk::RESPONSE_OK) {
        Glib::RefPtr<Gtk::FileFilter> fileFilter = dialog.get_filter();
        Glib::RefPtr<ImageFilter> filter = Glib::RefPtr<ImageFilter>::cast_dynamic(fileFilter);
        if (filter) {
            Gdk::PixbufFormat format = filter->getFormat();
            save_image(dialog.get_filename(), format);
        }
        else {
            m_appSupport.showError("Coud not identify format !!!");
        }
    }
}

void
ImageView::save_image(Glib::ustring filename, Gdk::PixbufFormat& format)
{
    //cout << "on save " << filename << " format " << filter->get_name() << endl;
    auto config = m_appSupport.getConfig();
    config->set_string(CONF_GROUP, CONF_PATH, filename);

    Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(filename);
	std::vector<Glib::ustring> keys;
	std::vector<Glib::ustring> opts;
	auto existOpts = m_content->getDisplayImage()->getOptions();
	if (ImageOptionDialog::hasOptions(format, file, existOpts)) {
		ImageOptionDialog optDlg(*this, format, file, existOpts);
		if (optDlg.run() != Gtk::RESPONSE_OK) {
			return;
		}
		optDlg.addOptions(keys, opts);
	}
    try {
        config->set_string(CONF_GROUP, CONF_FILTER, format.get_name());
        m_content->getDisplayImage()->getPixbuf()->save(filename, format.get_name(), keys, opts);
        m_appSupport.saveConfig();
    }
    catch (const Glib::Error &ex) {
        m_appSupport.showError(ex.what());
    }
}

void
ImageView::on_menu_next()
{
    ++m_front;
    m_front %= m_picts.size();
    showFront();
}

void
ImageView::on_menu_prev()
{
    --m_front;
    if (m_front < 0) {
        m_front += m_picts.size();
    }
    showFront();
}

void
ImageView::on_menu_n(int32_t n)
{
    m_front = n;
    showFront();
}

void
ImageView::on_menu_view(ViewMode viewMode)
{
    // as the signal itself does not give actual activation
    viewMode = m_fitRadio->get_active() ? ViewMode::FIT : ViewMode::NATIVE;
    //std::cout << "ImageView::on_menu_view" << (int)viewMode << std::endl;
    m_content->setViewMode(viewMode);
    Glib::KeyFile* config = m_appSupport.getConfig();
    gint iViewMode = (gint)viewMode;
    config->set_integer(CONF_GROUP, CONF_VIEW, iViewMode);
    m_appSupport.saveConfig();
}

bool
ImageView::is_mime_gdk_readable(const Glib::ustring& mime)
{
	std::vector<Gdk::PixbufFormat> formats = Gdk::Pixbuf::get_formats();
	bool supported = false;
	for (Gdk::PixbufFormat format : formats) {	// only include images that are displayable (readable)
		for (auto availMime : format.get_mime_types()) {
			if (mime == availMime
				&& !format.is_disabled()) {
				supported = true;
				break;
			}
		}
	}
	return supported;
}