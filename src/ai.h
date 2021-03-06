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
/** @file
 *  Definitions for the AI system structures
 */

#ifndef __INCLUDED_SRC_AI_H__
#define __INCLUDED_SRC_AI_H__

#include "droiddef.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#define ALLIANCE_BROKEN		0			// states of alliance between players
#define ALLIANCE_REQUESTED	1
#define ALLIANCE_INVITATION	2
#define ALLIANCE_FORMED		3
#define	ALLIANCE_NULL		4			// for setting values only.

#define NO_ALLIANCES		0			//alliance possibilities for games.
#define ALLIANCES			1
#define	ALLIANCES_TEAMS		2			//locked teams

// alliances
extern uint8_t alliances[MAX_PLAYER_SLOTS][MAX_PLAYER_SLOTS];
extern uint8_t alliancebits[MAX_PLAYER_SLOTS];
extern uint8_t satuplinkbits;

/** Check no alliance has formed. This is a define to make sure we inline it. */
#define aiCheckAlliances(_s1, _s2) (alliances[_s1][_s2] == ALLIANCE_FORMED)

/* Initialise the AI system */
BOOL aiInitialise(void);

/* Shutdown the AI system */
BOOL aiShutdown(void);

/* Initialise a droid structure for AI */
//extern BOOL aiInitDroid(DROID *psDroid);

/* Do the AI for a droid */
void aiUpdateDroid(DROID *psDroid);

// Find the nearest best target for a droid
// returns integer representing quality of choice, -1 if failed
SDWORD aiBestNearestTarget(DROID *psDroid, BASE_OBJECT **ppsObj, int weapon_slot, UWORD *targetOrigin);

// Are there a lot of bullets heading towards the structure?
BOOL aiObjectIsProbablyDoomed(BASE_OBJECT *psObject);

// Update the expected damage of the object.
void aiObjectAddExpectedDamage(BASE_OBJECT *psObject, SDWORD damage);

/* See if there is a target in range added int weapon_slot*/
BOOL aiChooseTarget(BASE_OBJECT *psObj,
						   BASE_OBJECT **ppsTarget, int weapon_slot, BOOL bUpdateTarget, UWORD *targetOrigin);

/** See if there is a target in range for Sensor objects. */
BOOL aiChooseSensorTarget(BASE_OBJECT *psObj, BASE_OBJECT **ppsTarget);

/*set of rules which determine whether the weapon associated with the object
can fire on the propulsion type of the target*/
BOOL validTarget(BASE_OBJECT *psObject, BASE_OBJECT *psTarget, int weapon_slot);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __INCLUDED_SRC_AI_H__
