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

class Tile;

#include "RectLayout.hpp"

class CubeLayout
: public RectLayout
{
public:
    CubeLayout(NaviGlArea *glArea);
    virtual ~CubeLayout();

    void position(bool scale, guint width, guint height) override;
    float getZposition(const psc::mem::active_ptr<Tile>& tile) override;
private:
    uint64_t m_min;
    uint64_t m_max;
};



