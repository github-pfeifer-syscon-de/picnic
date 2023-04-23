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
#include <math.h>


#include "NaviGlArea.hpp"
#include "Tile.hpp"
#include "RollLayout.hpp"


RollLayout::RollLayout(NaviGlArea *glArea)
: Layout(glArea)
, m_front(0)
{
}


RollLayout::~RollLayout()
{
}

float
RollLayout::getZposition(const Tile *tile)
{
    return 0.0f;
}

void
RollLayout::position(bool scale, guint width, guint height)
{
    placeTiles();
}

Position
RollLayout::getTextPosition(const Position &akt)
{
    Position add( + 1.5f, + 2.40f, + 7.1f);
    Position pos{akt + add};
    return pos;
}

void
RollLayout::addToFront(int inc)
{
    m_front += inc;
    while (m_front < 0) {  // always keep front in range of indexes
        m_front += m_tiles.size();
    }
    m_front = m_front % m_tiles.size();
}

int32_t
RollLayout::getFront() const
{
    return m_front;
}

bool
RollLayout::scroll(GdkEventScroll* event)
{
    float rotStep = getRotStep();
    float aktRot = 90.0f;       // the first movement will always skip the gap
    bool textVisible = TRUE;
    if (event->direction == GDK_SCROLL_DOWN) {
        textVisible = FALSE;
        for (uint32_t j = 0; j < m_tiles.size(); ++j) {
            Tile *tile = m_tiles[(j + m_front) % m_tiles.size()];
            Rotational rotate = tile->getRotateTo();    // if we get a event while moving accumulate movement -> smooth and fast
            Rotational rotateTo(rotate.getPhi() + 0.0f, rotate.getTheta() + aktRot, rotate.getPsi() + 0.0f);
            //std::cout << "down m_front " << m_front << " name " << tile->getFileName() << " from  " << rotate.getTheta() << " to " << rotateTo.getTheta() <<std::endl;
            tile->setRotateTo(rotateTo);
            tile->setTextVisible(textVisible);
            aktRot = rotStep;
        }
        addToFront(+1);
        m_tiles[m_front]->setTextVisible(TRUE);
    }
    else if (event->direction == GDK_SCROLL_UP) {
        addToFront(-1);
        for (uint32_t j = 0; j < m_tiles.size(); ++j) {
            Tile *tile = m_tiles[(j + m_front) % m_tiles.size()];
            Rotational rotate = tile->getRotateTo();    // if we get a event while moving accumulate movement -> smooth and fast
            Rotational rotateTo(rotate.getPhi() + 0.0f, rotate.getTheta() - aktRot, rotate.getPsi() + 0.0f);
            //std::cout << "up m_front " << m_front << " name " << tile->getFileName() << " from  " << rotate.getTheta() << " to " << rotateTo.getTheta() <<std::endl;
            tile->setRotateTo(rotateTo);
            tile->setTextVisible(textVisible);
            textVisible = FALSE;
            aktRot = rotStep;
        }
    }
    return TRUE;
}

void
RollLayout::hover(float mx, float my)
{
}

float
RollLayout::getRotStep()
{
    uint32_t cnt = m_tiles.size();
    float rotStep = (cnt <= 1) ? 270.0f : 270.0f / (float)(cnt - 1);
    return rotStep;
}

void
RollLayout::placeTiles()
{
    float rotStep = getRotStep();
    float rotAct = 0.0f;        // is up front
    bool textVisible = true;
    for (int32_t i = 0; i < (int)m_tiles.size(); ++i) {
        int j = (i + m_front) % (int)m_tiles.size();
        Tile *tile = m_tiles[j];
        //std::cout <<  "RollLayout::placeTiles"
        //          << " name " << p->getFileName()
        //          << " j " << j <<  "/" << m_tiles.size()
        //          << " theta " << rotAct << std::endl;
        Rotational rotate(0.0f, (float)rotAct, 0.0f);
        tile->setRotation(rotate);
        tile->setTextVisible(textVisible);
        textVisible = false;
        rotAct -= rotStep;
    }
}

void
RollLayout::add(Tile *tile)
{
	Layout::add(tile);
	float fscale = 12.0f;
	float z = getZposition(tile);
	Position pos(-8.0f, 0.0f, z);
	tile->scale(fscale);
	tile->setPosition(pos);
}
