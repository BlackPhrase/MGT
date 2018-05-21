/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief

#include "quakedef.h"
#include "vgui_int.h"

void *gpGUILib{nullptr};
IGUI *gpGUI{nullptr};

void VGui_Startup()
{
	gpGUILib = Sys_LoadModule("gui");
	
	if(!gpGUILib)
		return;
	
	auto pfnGUIFactory{Sys_GetFactory(gpGUILib)};
	
	if(!pfnGUIFactory)
		return;
	
	gpGUI = pfnGUIFactory(MGT_GUI_INTERFACE_VERSION, nullptr);
	
	if(!gpGUI)
		return;
	
	if(!gpGUI->Init())
		return;
};

void VGui_Shutdown()
{
	if(gpGUI)
		gpGUI->Shutdown();
	
	if(gpGUILib)
	{
		Sys_UnloadModule(gpGUILib);
		gpGUILib = nullptr;
	};
};

void VGui_ViewportPaintBackground(int extents[4])
{
	// TODO
};

void VGui_ConsolePrint(const char *text)
{
	// TODO: something else?
	Con_Print(text);
};

void *VGui_GetPanel()
{
	// TODO
	return nullptr;
};