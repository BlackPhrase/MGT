/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018, 2020 BlackPhrase
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

#include "quakedef.h"
#include "event_args.h"

#define MAX_EVENTS 32

typedef struct event_s
{
	unsigned short id;
	const char *name;
	event_args_t args;
	float delay;
	//pfnEventHook fnHook;
} event_t;

event_t gEventQue[MAX_EVENTS];

void CL_QueEvent(event_args_t *apArgs, float delay)
{
	static int nLast = 0;
	
	if(nLast >= MAX_EVENTS)
		return;
	
	gEventQue[nLast].args.flags = apArgs->flags;
	gEventQue[nLast].args.entindex = apArgs->entindex;
	
	VectorCopy(apArgs->origin, gEventQue[nLast].args.origin);
	VectorCopy(apArgs->angles, gEventQue[nLast].args.angles);
	VectorCopy(apArgs->velocity, gEventQue[nLast].args.velocity);
	
	gEventQue[nLast].args.ducking = apArgs->ducking;
	
	gEventQue[nLast].args.fparam1 = apArgs->fparam1;
	gEventQue[nLast].args.fparam2 = apArgs->fparam2;
	
	gEventQue[nLast].args.iparam1 = apArgs->iparam1;
	gEventQue[nLast].args.iparam2 = apArgs->iparam2;
	
	gEventQue[nLast].args.bparam1 = apArgs->bparam1;
	gEventQue[nLast].args.bparam2 = apArgs->bparam2;
	
	gEventQue[nLast].delay = delay;
	
	nLast++;
};

void CL_ParseEvent(bool abReliable)
{
	// TODO
	
	event_args_t args;
	memset(&args, 0, sizeof(args));
	
	CL_QueEvent(&args, 0.0f);
};

void CL_FireEvents()
{
	// TODO

	//for(int i = 0; i < MAX_EVENTS; i++)
	{
		//gEventQue[i]
		
		//Con_Printf("CL_FireEvents: Unknown event %d:%s", currentevent.id, currentevent.name);
	};
};