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


#include <NaviGlArea.hpp>
#include "Layout.hpp"
#include "Tile.hpp"

class CompareByName  {
    public:
    bool operator()(const psc::mem::active_ptr<Tile>& a, const psc::mem::active_ptr<Tile>& b) {
        if (!a) {
            return false;
        }
        if (!b) {
            return true;
        }
        auto la = a.lease();
        auto lb = b.lease();
        if (la && lb) {
            bool gt = la->getFileName() > lb->getFileName();
            return gt;
        }
        return a.get() > b.get();   // just use some criteria ...
        //std::cout << "CompareByName " << a->getFileName()<< " " << (gt ? "<=" : ">") << " " << b->getFileName() << std::endl;
    }
};

Layout::Layout(NaviGlArea *glArea)
: m_tiles()
, m_glArea{glArea}
, m_needSort{false}
{
}


Layout::~Layout()
{
    clear();    // allocation is handled by view
}

void
Layout::add(const psc::mem::active_ptr<Tile>& tile)
{
    m_tiles.push_back(tile);
    m_needSort = true;
}

bool
Layout::sort()
{
    if (m_needSort) {
        m_tiles.sort(CompareByName());
        m_needSort = false;
        return true;
    }
    return false;
}

void
Layout::clear()
{
    m_needSort = false;
    m_tiles.clear();
}

bool
Layout::scroll(GdkEventScroll* event)
{
    return FALSE;
}


const TVectorConcurrent<psc::mem::active_ptr<Tile>>&
Layout::getTiles()
{
    return m_tiles;
}