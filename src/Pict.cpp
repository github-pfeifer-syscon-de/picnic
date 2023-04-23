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
#include <limits.h>
#include <iostream>
#include <iomanip>

#include "StringUtils.hpp"

#include "Pict.hpp"

Pict::Pict(const Glib::RefPtr<Gio::File> path, const Glib::ustring &name, const std::string mime, TextContext *pictContext)
: Tile(GL_TRIANGLES, pictContext)
, m_path(path)
, m_name(name)
, m_mime(mime)
, m_tex(nullptr)
, m_pix()
, m_state(State::CREATED)
{
}


Pict::~Pict()
{
    if (m_tex) {
        delete m_tex;
        m_tex = nullptr;
    }
}

void
Pict::setPosition(Position &pos)
{
    Geometry::setPosition(pos);
}

void
Pict::scale(float scale)
{
    if (m_state <= State::CREATED) {
        m_state = State::SCALED;
        m_scale = scale;
        Position p1(0.0f, 0.0f, 0.0f);   UV uv1(0.0f, 1.0f);
        Position p2(1.0f, 0.0f, 0.0f);   UV uv2(1.0f, 1.0f);
        Position p3(1.0f, 1.0f, 0.0f);   UV uv3(1.0f, 0.0f);
        Position p4(0.0f, 1.0f, 0.0f);   UV uv4(0.0f, 0.0f);
        addPoint(&p1, nullptr, nullptr, &uv1);
        addPoint(&p2, nullptr, nullptr, &uv2);
        addPoint(&p3, nullptr, nullptr, &uv3);
        addPoint(&p4, nullptr, nullptr, &uv4);
        addIndex(0, 1, 2);
        addIndex(2, 3, 0);
        setScale(m_scale);

        create_vao();
        checkError("temp createVao");
    }
}

void
Pict::display(const Matrix &projView)
{
    if (m_tex != nullptr) {
		//std::cout << "Pict::display tex " << getFileName() << std::endl;
        m_tex->use(GL_TEXTURE0);
        if (m_tex->getTex() == 0) {
            std::cout << getFileName() << " tex " <<  m_tex->getTex() << std::endl;
        }
    }
    else {
		//std::cout << "Pict::display gray " << getFileName() << std::endl;
        m_gray->use(GL_TEXTURE0);
    }

	//std::cout << "Pict::display display " << getFileName() << std::endl;
    Geometry::display(projView);
    if (m_tex != nullptr) {
        m_tex->unuse();
    }
    else {
        m_gray->unuse();
    }
	//std::cout << "Pict::display end " << getFileName() << std::endl;
}

std::string
Pict::getFileName() const
{
    return m_path->get_basename();
}

const Glib::ustring &
Pict::getName()
{
    return m_name;
}

const Glib::RefPtr<Gio::File>
Pict::getPath() const
{
    return m_path;
}

std::string
Pict::getUri()
{
    return m_path->get_uri();
}

std::string
Pict::getMime()
{
    return m_mime;
}

void
Pict::setPixbuf(Glib::RefPtr<Gdk::Pixbuf>& pix)
{
	//std::cout << "Pict::setPixbuf " << getUri()
	//	      << " width: " << pix->get_width()
	//	      << " height: " << pix->get_height()
	//	      << " chan: " << pix->get_n_channels()
	//	      << " alpha: " << (pix->get_has_alpha() ? "yes" : "no")
	//	      << std::endl;
    m_pix = pix;
}

bool
Pict::hasThumbnail()
{
    if (m_tex != nullptr
     || m_pix) {
        return true;
    }
    return false;
}

void
Pict::setGray(Tex *gray)
{
    m_gray = gray;
}

bool
Pict::create(TextContext &pictContext)
{
    if (m_tex != nullptr
     || !m_pix) {
        return false;
    }
    //Gdk::Pixbuf *buf = m_pix.get();
    //std::cout << "pict::create " << m_path
    //        << " width: " << buf->get_width()
    //        << " height: " << buf->get_height()
    //        << " chan: " << buf->get_n_channels()
    //        << " bits: " << buf->get_bits_per_sample()
    //        << " alph: " << (buf->get_has_alpha() ? "yes" : "no")
    //        << std::endl;

    remove();   // remove temorary plane

    float xaspect = std::min(1.0f, (float)m_pix->get_width() / (float)m_pix->get_height());
    float yaspect = std::min(1.0f, (float)m_pix->get_height() / (float)m_pix->get_width());
    float xoffs = (1.0f - xaspect) / 2.0f;
    float yoffs = (1.0f - yaspect) / 2.0f;  // this offsets the image from the point of rotation, nice for text, will show parts of images in background
    Position p1(xoffs + 0.0f,    yoffs + 0.0f,    0.0f);    UV uv1(0.0f, 1.0f);
    Position p2(xoffs + xaspect, yoffs + 0.0f,    0.0f);    UV uv2(1.0f, 1.0f);
    Position p3(xoffs + xaspect, yoffs + yaspect, 0.0f);    UV uv3(1.0f, 0.0f);
    Position p4(xoffs + 0.0f,    yoffs + yaspect, 0.0f);    UV uv4(0.0f, 0.0f);
    addPoint(&p1, nullptr, nullptr, &uv1);
    addPoint(&p2, nullptr, nullptr, &uv2);
    addPoint(&p3, nullptr, nullptr, &uv3);
    addPoint(&p4, nullptr, nullptr, &uv4);
    addIndex(0, 1, 2);
    addIndex(2, 3, 0);
    //setScale(m_scale);
    //Geometry::setRotation(m_rotate);
    create_vao();
    checkError("real createVao");
    pictContext.addGeometry(this);

    auto tex = new Tex();
    tex->create(m_pix);
	m_tex = tex;
    m_state = State::COMPLETE;

    m_pix.clear();    // reference no longer needed
    return true;
}

void
Pict::setThumbnail(Glib::RefPtr<Gio::File>& thumbnail)
{
	//std::cout << "Pict::setThumbnail "
	//	      << getUri()
	//	      << " " << thumbnail->get_path()
	//	      << std::endl;
    m_thumbnail = thumbnail;
}

Glib::RefPtr<Gio::File>
Pict::getThumbnail()
{
    return m_thumbnail;
}
