/*
	This file is part of Warzone 2100.
	Copyright (C) 2008  Warzone Resurrection Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef __INCLUDED_SRC_TERRAIN_H__
#define __INCLUDED_SRC_TERRAIN_H__

#include "lib/ivis_common/pietypes.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

bool initTerrain(void);
void shutdownTerrain(void);

void drawTerrain(void);
void drawWater(void);

PIELIGHT getTileColour(int x, int y);
void setTileColour(int x, int y, PIELIGHT colour);

void markTileDirty(int i, int j);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
