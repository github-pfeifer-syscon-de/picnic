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

enum class Animation {
    NONE,
    SCALE,
    ROTATE
};

#include <string>
#include <Geom2.hpp>
#include <Text2.hpp>
#include <GeometryContext.hpp>
#include <MarkContext.hpp>
#include <Font2.hpp>
#include <TextContext.hpp>


class Layout;

class Tile
: public psc::gl::Geom2 {
public:
    Tile(GLenum type, GeometryContext *_ctx);
    virtual ~Tile() = default;

    void scaleTo(float scale);
    void setToScale(float scale);
    void setRotateTo(const Rotational& rotate);
    const Rotational& getRotateTo() const;
    bool advance();
    virtual void setPosition(Position &pos) = 0;
    virtual void scale(float scale) = 0;
    virtual void setScale(float scale) override;
    virtual void setRotation(const Rotational &rotate);
    virtual std::string getFileName() const = 0;
    Animation getAnimation();
    virtual const Glib::ustring &getName() = 0;
    void displayText(Matrix &projView, MarkContext *textContext, const psc::gl::ptrFont2& font, const std::shared_ptr<Layout>& layout);
    void setTextVisible(bool visible);
    void setModified(guint64 modified);
    guint64 getModified() const;

private:
    float m_toScale;
    float m_aktScale;
    float m_baseScale;
    Rotational m_startRotate;
    Rotational m_toRotate;
    Animation m_animation;
    gint64 m_started;
    psc::gl::aptrText2 m_text;
    bool m_textVisible;
    guint64 m_modified;
};
