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
#include <glm/common.hpp> // glm::mix
#include <math.h>

#include "Text.hpp"
#include "Tile.hpp"
#include "Layout.hpp"


class CompareByName  {
    public:
    bool operator()(const Tile* a, const Tile* b) {
        if (a == nullptr) {
            return FALSE;
        }
        if (b == nullptr) {
            return TRUE;
        }
        return a->getFileName() > b->getFileName();
    }
};

Tile::Tile(GLenum type, GeometryContext *_ctx)
: Geometry(type, _ctx)
, m_toScale(1.0f)
, m_aktScale(1.0f)
, m_baseScale(1.0f)
, m_startRotate()
, m_toRotate()
, m_animation(Animation::NONE)
, m_started(LLONG_MIN)
, m_text(nullptr)
, m_textVisible(TRUE)
{
}


Tile::~Tile()
{
    if (m_text) {
        delete m_text;
        m_text = nullptr;
    }
}

void
Tile::setToScale(float toScale)
{
    m_toScale = toScale * m_baseScale;

    Position &act = getPos();
    Position p(act.x, act.y, act.z+(toScale > 1.0f ? 0.1f : -0.1f));
    setPosition(p);
    m_animation = Animation::SCALE;
    m_started = g_get_monotonic_time();
}

void
Tile::setRotateTo(const Rotational& rotate)
{
    m_startRotate = getRotation();
    m_toRotate = rotate;
    //std::cout << "Tile::setRotateTo start " << m_startRotate.getTheta()
    //          << " to " << m_toRotate.getTheta() << std::endl;
    m_animation = Animation::ROTATE;
    m_started = g_get_monotonic_time();
}

const Rotational&
Tile::getRotateTo() const
{
    return m_toRotate;
}

void
Tile::setRotation(const Rotational &rotate)
{
    m_startRotate = rotate;
    m_toRotate = rotate;
    Geometry::setRotation(rotate);
}

void
Tile::setScale(float scale)
{
    m_aktScale = scale;
    m_toScale =  scale;
    m_baseScale = scale;
    Geometry::setScale(scale);
}

Animation
Tile::getAnimation()
{
    return m_animation;
}

bool
Tile::advance()
{
    if (m_animation > Animation::NONE) {
        gint64 now = g_get_monotonic_time();
        double dt = ((double)(now - m_started)) / 1.0e6;
        if (dt <= 1.0) {
            if (m_animation == Animation::SCALE) {
                m_aktScale = glm::mix(m_aktScale, m_toScale, dt);  //m_aktScale -> ease looks nice,  m_baseScale -> linear looks lame
                Geometry::setScale(m_aktScale);
            }
            else if (m_animation == Animation::ROTATE) {
                float phiStart = m_startRotate.getPhi();
                float thetaStart = m_startRotate.getTheta();
                float psiStart = m_startRotate.getPsi();
                float phiTo = m_toRotate.getPhi();
                float thetaTo = m_toRotate.getTheta();
                float psiTo = m_toRotate.getPsi();
                float phiAkt = glm::mix(phiStart, phiTo, dt);
                float thetaAkt = glm::mix(thetaStart, thetaTo, dt);
                float psiAkt = glm::mix(psiStart, psiTo, dt);
                //if (thetaStart != thetaTo) {
                //    std::cout << "Tile::advance "
                //              << " dt " << dt
                //              << " start " << thetaStart
                //              << " to "  << thetaTo
                //              << " act " << thetaAkt << std::endl;
                //}
                Rotational m_aktRotate(phiAkt, thetaAkt, psiAkt);
                Geometry::setRotation(m_aktRotate);
            }
        }
        else if (dt > 1.0) {
            //std::cout << "Tile::advance to " << m_aktRotate.getTheta() << std::endl;
            if (m_animation == Animation::SCALE) {  // at end match expected value
                Geometry::setScale(m_toScale);
            }
            else if (m_animation == Animation::ROTATE) {
                Geometry::setRotation(m_toRotate);
            }
            m_animation = Animation::NONE;
        }
        return true;    // always update if we are animating something
    }
    return false;
}

void
Tile::setTextVisible(bool visible)
{
    m_textVisible = visible;    // remember if not yet created
    if (m_text != nullptr) {
        m_text->setVisible(visible);
    }
}

void
Tile::displayText(Matrix &projView, MarkContext *textContext, Font *font, Layout* layout)
{
    Position& akt = getPos();
    Position pos = layout->getTextPosition(akt);
    if (m_text == nullptr) {
        m_text = new Text(GL_TRIANGLES, textContext, font); // GL_LINES triangles are more effort but the full depth check matters
        const Glib::ustring txt = getName();
        m_text->setText(txt);
        m_text->setVisible(m_textVisible);
    }
    m_text->setPosition(pos);
    m_text->setScale(m_scale * 0.0015f);      // Quads 0.05f triangles 0.0015f
    m_text->display(projView);
}

void
Tile::setModified(guint64 modified)
{
    m_modified = modified;
}

guint64
Tile::getModified() const
{
    return m_modified;
}
