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

#include <gtkmm.h>

#include "ConcurrentCollections.hpp"

class Tile;
class NaviGlArea;

class Layout {
public:
    Layout(NaviGlArea *glArea);
    virtual ~Layout();

    virtual void add(Tile *tile);
    virtual void position(bool scale, guint width, guint height) = 0;
    virtual void hover(float mx, float my) = 0;
    virtual Position getTextPosition(const Position &pos) = 0;
    virtual int32_t getFront() const = 0;
    void clear();
    virtual bool scroll(GdkEventScroll* event);
    const TVectorConcurrent<Tile *>& getTiles();
    bool sort();
protected:
    TVectorConcurrent<Tile *> m_tiles;  // as additions will be asynchronously use concurrent version
    NaviGlArea *m_glArea;
private:
    bool m_needSort;
};

