/*
Copyright (C) 2021 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include "CommonTypes.hpp"

interface IGame;

class CSystem;

class CGame
{
public:
	CGame(CSystem *apSystem);
	
	void Init();
	void Shutdown();
	
	void Update(float afTimeStep);
private:
	void LoadModule();
	void UnloadModule();
	
	void *mpGameModule{nullptr};
	
	IGame *mpGame{nullptr};
	CSystem *mpSystem{nullptr};
};