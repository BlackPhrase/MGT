/*
Copyright (C) 2019-2021 BlackPhrase

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

#include <memory>

class CScreen;
class CView;
class CDraw;

class CVideo
{
public:
	/// Called at startup to set up translation tables, takes 256 8 bit RGB values
	/// the palette data will go away after the call, so it must be copied off if
	/// the video driver will need it again
	void Init(unsigned char *palette);

	/// Called at shutdown
	void Shutdown();
	
	void Update();
private:
	void CheckChanges();
	void UpdateScreen();
	
	void LoadRendererModule(const char *asName);
	void UnloadRendererModule();
private:
	std::unique_ptr<CScreen> mpScreen;
	std::unique_ptr<CView> mpView;
	std::unique_ptr<CDraw> mpDraw;
	
	void *mpRendererModule{nullptr};
	
	IRenderer *mpRenderer{nullptr};
};