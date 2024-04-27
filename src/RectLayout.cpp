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
#include <math.h>

#include "Tile.hpp"
#include "RectLayout.hpp"

RectLayout::RectLayout(NaviGlArea *glArea)
: Layout(glArea)
, m_last{nullptr}
, m_front{0u}
{
}


RectLayout::~RectLayout()
{
}

float
RectLayout::getZposition(const psc::mem::active_ptr<Tile>& tile)
{
    return 0.0f;
}

void
RectLayout::position(bool scale, guint width, guint height)
{
    float a = 1.0f;     // target aspect
    float v = (float)width / (float)height;
    float xl, yl;
    if (v >= 1.0) {
        // wide viewport, use full height
        float va = v/a;
        xl = va * 10.0f;
        yl = 10.0f;
    } else {
        // tall viewport, use full width
        float av = a/v;
        xl = 10.0f;
        yl = av * 10.0f;
    }
    // try to align count on each side to aspect
    unsigned int cnt = m_tiles.size();
    float xCnt = sqrtf((float)cnt * (float)width / (float)height);
    unsigned int xN = floorf(xCnt);
    float yCnt = (float)xN * (float)height / (float)width;
    unsigned int yN = ceilf(yCnt);
    float sx = (2.0f * xl / (float)xN);
    float sy = (2.0f * yl / (float)yN);
    //std::cout << "cnt: " << cnt << " xside: " << xN << " yside: " << yN << " sx: " << sx << " sy: " << sy << std::endl;
    float x = -xl;
    float y = yl;
    for (auto p : m_tiles) {
        float z = getZposition(p);
        if (auto lp = p.lease()) {
            if (scale) {
                float fscale = std::min(sx, sy)*0.9f;
                lp->scale(fscale);
            }
            Position pos(x, y, z);
            lp->setPosition(pos);
            x += sx;
            if (x >= xl) {
                y -= sy;
                x = -xl;
            }
        }
    }
}

Position
RectLayout::getTextPosition(const Position &akt)
{
    Position pos(akt.x, akt.y - 0.30f, akt.z + 0.01f);
    return pos;
}

void
RectLayout::hover(float mx, float my)
{
    bool hit = false;
    uint32_t n = 0;
    for (auto pict : m_tiles) {
        if (auto lpict = pict.lease()) {
            if (lpict->hit(mx, my)) {
                hit = true;
                m_front = n;
                if (m_last
                 && m_last != pict) {
                    if (auto llast = m_last.lease()) {
                        llast->setToScale(1.0f);
                    }
                    m_last = nullptr;
                }
                if (!m_last
                 || m_last != pict) {
                    if (auto lpict = pict.lease()) {
                        lpict->setToScale(2.5f);
                    }
                    m_last = pict;
                }
                break;
            }
        }
        ++n;
    }
    if (!hit
     && m_last) {
        if (auto llast = m_last.lease()) {
            llast->setToScale(1.0f);   // ret scale on leaving img
        }
        m_last.resetAll();
    }
}


int32_t
RectLayout::getFront() const
{
    return m_front;     // last zoomed
}
