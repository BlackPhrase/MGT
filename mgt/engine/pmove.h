/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief player movement

#pragma once

#include "pm_shared/pm_defs.h"
#include "pm_shared/pm_movevars.h"

/*
typedef struct
{
	vec3_t normal;
	float dist;
} pmplane_t;

typedef struct
{
	qboolean allsolid;   // if true, plane is not valid
	qboolean startsolid; // if true, the initial point was in a solid area
	qboolean inopen, inwater;
	float fraction;  // time completed, 1.0 = didn't hit anything
	vec3_t endpos;   // final position
	pmplane_t plane; // surface normal at impact
	int ent;         // entity the surface is on
} pmtrace_t;

typedef struct
{
	vec3_t origin;
	model_t *model;    // only for bsp models
	vec3_t mins, maxs; // only for non-bsp models
	int info;          // for client or server to identify
} physent_t;

typedef struct
{
	int sequence; // just for debugging prints

	// player state
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	int oldbuttons;
	float waterjumptime;
	qboolean dead;
	int spectator;

	// world state
	int numphysent;
	physent_t physents[MAX_PHYSENTS]; // 0 should be the world

	// input
	usercmd_t cmd;

	// results
	int numtouch;
	int touchindex[MAX_PHYSENTS];
} playermove_t;

typedef struct
{
	float gravity;
	float stopspeed;
	float maxspeed;
	float spectatormaxspeed;
	float accelerate;
	float airaccelerate;
	float wateraccelerate;
	float friction;
	float waterfriction;
	float entgravity;
} movevars_t;
*/

extern playermove_t *pmove;

extern int onground;
extern int waterlevel;
extern int watertype;

extern vec3_t player_mins;
extern vec3_t player_maxs;

void PlayerMove();

void Pmove_Init(playermove_t *ppmove);

int PM_HullPointContents(hull_t *hull, int num, vec3_t p);

int PM_PointContents(vec3_t point);
qboolean PM_TestPlayerPosition(vec3_t point);
pmtrace_t PM_PlayerMove(vec3_t start, vec3_t stop);