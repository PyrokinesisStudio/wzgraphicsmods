/*
	This file is part of Warzone 2100.
	Copyright (C) 1999-2004  Eidos Interactive
	Copyright (C) 2005-2009  Warzone Resurrection Project

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
/***************************************************************************/
/*
 * pieBlitFunc.h
 *
 * patch for exisitng ivis rectangle draw functions.
 *
 */
/***************************************************************************/

#ifndef _pieBlitFunc_h
#define _pieBlitFunc_h

/***************************************************************************/

#include "lib/framework/frame.h"
#include "lib/framework/string_ext.h"
#include "piedef.h"

/***************************************************************************/
/*
 *	Global Definitions
 */
/***************************************************************************/
#define NUM_BACKDROPS 10

#define BACKDROP_WIDTH	640
#define BACKDROP_HEIGHT	480

/* These are Qamly's hacks used for map previews. They need to be power of
 * two sized until we clean up this mess properly. */
#define BACKDROP_HACK_WIDTH 512
#define BACKDROP_HACK_HEIGHT 512

/***************************************************************************/
/*
 *	Global Variables
 */
/***************************************************************************/

/***************************************************************************/
/*
 *	Global ProtoTypes
 */
/***************************************************************************/
extern void pie_Line(int x0, int y0, int x1, int y1, PIELIGHT colour);
extern void pie_Box(int x0,int y0, int x1, int y1, PIELIGHT colour);
extern void pie_BoxFill(int x0,int y0, int x1, int y1, PIELIGHT colour);
extern void pie_ImageFileID(IMAGEFILE *ImageFile, UWORD ID, int x, int y);
extern void pie_ImageFileIDTile(IMAGEFILE *ImageFile, UWORD ID, int x, int y, int Width, int Height);

extern void pie_TransBoxFill(float x0, float y0, float x1, float y1);
extern void pie_UniTransBoxFill(float x0, float y0, float x1, float y1, PIELIGHT colour);

extern BOOL pie_InitRadar(void);
extern BOOL pie_ShutdownRadar(void);
extern void pie_DownLoadRadar(UDWORD *buffer, int width, int height);
extern void pie_RenderRadar(int x, int y, int width, int height);

extern void pie_UploadDisplayBuffer(void);

typedef enum _screenType
{
	SCREEN_RANDOMBDROP,
	SCREEN_CREDITS,
	SCREEN_MISSIONEND,
} SCREENTYPE;

extern void pie_LoadBackDrop(SCREENTYPE screenType);

#endif //
