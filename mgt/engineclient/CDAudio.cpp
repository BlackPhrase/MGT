/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
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

#include "quakedef.h"
#include "icdaudio.h"

#include "null/CDAudioNull.hpp"

//extern ICDAudio *CreateCDAudio();

ICDAudio *gpCDAudio{ nullptr };

int CDAudio_Init()
{
	gpCDAudio = new CCDAudioNull();  // TODO: CreateCDAudio()?

	if(gpCDAudio)
		return gpCDAudio->Init();

	return 0;
};

void CDAudio_Shutdown()
{
	if(gpCDAudio)
		gpCDAudio->Shutdown();
};

void CDAudio_Update()
{
	if(gpCDAudio)
		gpCDAudio->Frame();
};

void CDAudio_Play(byte track, bool looping)
{
	//if(gpCDAudio)
		//gpCDAudio->Play(track, looping);
};

void CDAudio_Stop()
{
	//if(gpCDAudio)
		//gpCDAudio->Stop();
};

void CDAudio_Pause()
{
	if(gpCDAudio)
		gpCDAudio->Pause();
};

void CDAudio_Resume()
{
	if(gpCDAudio)
		gpCDAudio->Resume();
};