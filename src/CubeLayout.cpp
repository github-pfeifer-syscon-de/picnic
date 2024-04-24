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

#include <cstdint>
#include <limits>
#include <cmath>

#include "Tile.hpp"
#include "CubeLayout.hpp"

// the depth is does not show up nicely

CubeLayout::CubeLayout(NaviGlArea *glArea)
: RectLayout(glArea)
, m_min{std::numeric_limits<uint64_t>::max()}
, m_max{std::numeric_limits<uint64_t>::min()}
{
}


CubeLayout::~CubeLayout()
{
}

void
CubeLayout::position(bool scale, guint width, guint height)
{
    for (auto p : m_tiles) {
        if (auto lp = p.lease()) {
            guint64 modified = lp->getModified();
            m_min = std::min(m_min, modified);
            m_max = std::max(m_max, modified);
        }
    }
    RectLayout::position(scale, width, height);
}

float
CubeLayout::getZposition(const psc::mem::active_ptr<Tile>& tile)
{
#ifdef __x86_64__
    guint64 diff = std::max(1ul, m_max - m_min);
#else
    guint64 diff = std::max(1ull, m_max - m_min);
#endif
    if (auto ltile = tile.lease()) {
        guint modified = ltile->getModified();
        float z = (float)((double)(m_max - modified) / (double)diff) * -10.0f;
        return z;
    }
    return 0.0f;
}