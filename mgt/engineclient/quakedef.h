/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2020 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief primary header for both client and server

#pragma once

//#define QUAKEDEF_H // TODO: the hell is cache_user_t???

//#define	GLTEST			// experimental stuff

#define QUAKE_GAME // as opposed to utilities

#define VERSION 1.09
#define GLQUAKE_VERSION 1.00
#define D3DQUAKE_VERSION 0.01
#define WINQUAKE_VERSION 0.996
#define LINUX_VERSION 1.30
#define X11_VERSION 1.10

//define	PARANOID			// speed sapping error checking

#define GAMENAME "valve" // directory to look in by default

#include <cmath>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <csetjmp>
//#include <cctype>
//#include <ctime>

#if defined(_WIN32) && !defined(WINDED)

#if defined(_M_IX86)
#define __i386__ 1
#endif

void VID_LockBuffer();
void VID_UnlockBuffer();

#else

#define VID_LockBuffer()
#define VID_UnlockBuffer()

#endif

#if defined __i386__ // && !defined __sun__
#define id386 1
#else
#define id386 0
#endif

#ifdef SERVERONLY // no asm in dedicated server
#undef id386
#endif

#if id386
#define UNALIGNED_OK 1 // set to 0 if unaligned accesses are not supported
#else
#define UNALIGNED_OK 0
#endif

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
#define CACHE_SIZE 32 // used to align key data structures

#define UNUSED(x) (x = x) // for pesky compiler / lint warnings

#define MINIMUM_MEMORY 0x550000
//#define	MINIMUM_MEMORY_LEVELPAK	(MINIMUM_MEMORY + 0x100000) // TODO: unused

#define MAX_NUM_ARGVS 50

// up / down
#define PITCH 0

// left / right
#define YAW 1

// fall over
#define ROLL 2

#define MAX_QPATH 64   // max length of a quake game pathname
#define MAX_OSPATH 128 // max length of a filesystem pathname

#define ON_EPSILON 0.1 // point on plane side epsilon

#define MAX_MSGLEN 8000   // max length of a reliable message
#define MAX_DATAGRAM 1024 // max length of unreliable message

//
// per-level limits
//
#define MAX_EDICTS 600 // FIXME: ouch! ouch! ouch!
#define MAX_LIGHTSTYLES 64
#define MAX_MODELS 256 // these are sent over the net as bytes
#define MAX_SOUNDS 256 // so they cannot be blindly increased

#define SAVEGAME_COMMENT_LENGTH 39

#define MAX_STYLESTRING 64

//
// stats are integers communicated to the client by the server
//
#define MAX_CL_STATS 32
#define STAT_HEALTH 0
#define STAT_FRAGS 1
#define STAT_WEAPON 2
#define STAT_AMMO 3
#define STAT_ARMOR 4
#define STAT_WEAPONFRAME 5
#define STAT_SHELLS 6
#define STAT_NAILS 7
#define STAT_ROCKETS 8
#define STAT_CELLS 9
#define STAT_ACTIVEWEAPON 10
#define STAT_TOTALSECRETS 11
#define STAT_TOTALMONSTERS 12
#define STAT_SECRETS 13  // bumped on client side by svc_foundsecret
#define STAT_MONSTERS 14 // bumped by svc_killedmonster

// stock defines

//
// item flags
//
#define IT_SHOTGUN 1
#define IT_SUPER_SHOTGUN 2
#define IT_NAILGUN 4
#define IT_SUPER_NAILGUN 8

#define IT_GRENADE_LAUNCHER 16
#define IT_ROCKET_LAUNCHER 32
#define IT_LIGHTNING 64
#define IT_SUPER_LIGHTNING 128

#define IT_SHELLS 256
#define IT_NAILS 512
#define IT_ROCKETS 1024
#define IT_CELLS 2048

#define IT_AXE 4096

#define IT_ARMOR1 8192
#define IT_ARMOR2 16384
#define IT_ARMOR3 32768

#define IT_SUPERHEALTH 65536

#define IT_KEY1 131072
#define IT_KEY2 262144

#define IT_INVISIBILITY 524288

#define IT_INVULNERABILITY 1048576
#define IT_SUIT 2097152
#define IT_QUAD 4194304

#define IT_SIGIL1 (1 << 28)
#define IT_SIGIL2 (1 << 29)
#define IT_SIGIL3 (1 << 30)
#define IT_SIGIL4 (1 << 31)

//===========================================

#define MAX_SCOREBOARD 16

#define SOUND_CHANNELS 8

// TODO
//#ifdef __cplusplus
//extern "C" {
//#endif

#include "common.h"
#include "cvardef.h"
#include "bspfile.h"
#include "vid.h"
#include "com_model.h" //#include "engine/IMemory.hpp"
#include "engine/ISystem.hpp" //#include "sys.h"

extern ISystem *gpSystem;

//#include "filesystem_internal.h"
#include "filesystem/IFileSystem.hpp"
#include "filesystem/IFile.hpp"

extern IFileSystem *gpFileSystem;

//#include "zone.h"
#include "mathlib.h"
//#include "info.h"
#include "crc.h"

#include "entity_state.h"

#include "wad.h"
#include "cvar.h"
#include "network/INetwork.hpp"
#include "protocol.h"
#include "cmd.h"
#include "draw.h"
#include "screen.h"
#include "sbar.h"
#include "sound.h"
#include "render.h"
#include "cdll_int.h"

#ifdef GLQUAKE
#include "gl_model.h"
#else
#include "model.h"
#include "d_iface.h"
#endif

#include "client.h"
#include "input.h"
//#include "pmove.h"
#include "keys.h"
#include "console.h"
#include "engine/IConsole.hpp"

extern IConsole *gpConsole;

#include "view.h"
#include "menu.h"
#include "cdaudio.h"

#ifdef GLQUAKE
#include "glquake.h"
#endif

//=============================================================================

extern qboolean noclip_anglehack;

extern cvar_t developer;

extern qboolean host_initialized; // true if into command execution
extern double host_frametime;
extern byte *host_basepal;
extern byte *host_colormap;
extern int host_framecount; // incremented every frame, never reset
extern double realtime;     // not bounded in any way, changed at
                            // start of every frame, never reset

extern qboolean msg_suppress_1; // suppresses resolution and cache size console output
                                //  an fullscreen DIB focus gain/loss
extern int current_skill;       // skill level for currently loaded level (in case
                                //  the user changes the cvar while the level is
                                //  running, this reflects the level actually in use)

extern int minimum_memory;

#include "chase.h"

// TODO
//#ifdef __cplusplus
//};
//#endif