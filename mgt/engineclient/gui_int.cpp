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
/// @brief

#include "quakedef.h"
#include "gui_int.h"
#include "vguiwrap3.h"

void GUI_Startup()
{
	VGuiWrap3_Startup();
};

void GUI_Shutdown()
{
	VGuiWrap3_Shutdown();
};

void GUI_ViewportPaintBackground(int extents[4]){
	// TODO
};

void GUI_ConsolePrint(const char *text)
{
	// TODO: something else?
	Con_Print(text);
};