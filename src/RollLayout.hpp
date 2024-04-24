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

#include "Layout.hpp"

class RollLayout : public Layout {
public:
    RollLayout(NaviGlArea *glArea);
    virtual ~RollLayout();

    virtual void position(bool scale, guint width, guint height) override;
    virtual void hover(float mx, float my);
    virtual float getZposition(const psc::mem::active_ptr<Tile>& tile);
    bool scroll(GdkEventScroll* event) override;
    Position getTextPosition(const Position &pos) override;
    int32_t getFront() const override;
    void add(const psc::mem::active_ptr<Tile>& tile) override;

protected :
    void placeTiles();

private:
    float getRotStep();
    void addToFront(int inc);

    int32_t m_front;    // this requires int as we will generate negative values (temporarily)
};



