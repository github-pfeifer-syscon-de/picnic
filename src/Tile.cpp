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
: psc::gl::Geom2(type, _ctx)
, m_toScale{1.0f}
, m_aktScale{1.0f}
, m_baseScale{1.0f}
, m_startRotate()
, m_toRotate()
, m_animation{Animation::NONE}
, m_started{LLONG_MIN}
, m_text{nullptr}
, m_textVisible{true}
{
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
    psc::gl::Geom2::setRotation(rotate);
}

void
Tile::setScale(float scale)
{
    m_aktScale = scale;
    m_toScale =  scale;
    m_baseScale = scale;
    psc::gl::Geom2::setScale(scale);
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
                psc::gl::Geom2::setScale(m_aktScale);
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
                psc::gl::Geom2::setRotation(m_aktRotate);
            }
        }
        else if (dt > 1.0) {
            //std::cout << "Tile::advance to " << m_aktRotate.getTheta() << std::endl;
            if (m_animation == Animation::SCALE) {  // at end match expected value
                psc::gl::Geom2::setScale(m_toScale);
            }
            else if (m_animation == Animation::ROTATE) {
                psc::gl::Geom2::setRotation(m_toRotate);
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
    if (auto ltext = m_text.lease()) {
        ltext->setVisible(visible);
    }
}

void
Tile::displayText(Matrix &projView, MarkContext *textContext, const psc::gl::ptrFont2& font, const std::shared_ptr<Layout>& layout)
{
    Position& akt = getPos();
    Position pos = layout->getTextPosition(akt);
    if (!m_text) {
        m_text = psc::mem::make_active<psc::gl::Text2>(GL_TRIANGLES, textContext, font); // GL_LINES triangles are more effort but the full depth check matters
        if (auto ltext = m_text.lease()) {
            const Glib::ustring txt = getName();
            ltext->setText(txt);
            ltext->setVisible(m_textVisible);
        }
    }
    if (auto ltext = m_text.lease()) {
        ltext->setPosition(pos);
        ltext->setScale(m_scale * 0.0012f);      // Quads 0.05f triangles 0.0012f
        ltext->display(projView);
    }
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
