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

#include "Geometry.hpp"
#include "TextContext.hpp"
#include "Tile.hpp"


enum class State {
    CREATED,
    SCALED,
    COMPLETE
};

class Pict :  public Tile {
public:
    Pict(const Glib::RefPtr<Gio::File> path, const Glib::ustring &name, const std::string mime, TextContext *pictContext);
    virtual ~Pict();
    void load();
    void scale(float scale) override;
    void setPosition(Position &pos) override;
    bool create(TextContext &pictContext);
    Geometry *getPlane();
    const Glib::ustring &getName() override;
    const Glib::RefPtr<Gio::File> getPath() const;
    std::string getUri();
    std::string getMime();
    void display(const Matrix &projView) override;
    bool hasThumbnail();
    void setGray(Tex *gray);
    void setPixbuf(Glib::RefPtr<Gdk::Pixbuf>& pix);
    std::string getFileName() const override;
    void setThumbnail(Glib::RefPtr<Gio::File>& thumbnail);
    Glib::RefPtr<Gio::File> getThumbnail();
protected:
private:
    Glib::RefPtr<Gio::File> m_path;
    Glib::ustring m_name;
    std::string m_mime;
    Tex *m_tex;
    Glib::RefPtr<Gdk::Pixbuf> m_pix;
    //float m_scale;
    Tex *m_gray;
    Glib::RefPtr<Gio::File> m_thumbnail;
    State m_state;
};
