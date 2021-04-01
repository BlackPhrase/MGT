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
/// @brief dynamic variable tracking

#include "quakedef.h"

cvar_t *cvar_vars{nullptr};
const char *cvar_null_string{""};

/*
============
Cvar_List_f

============
*/
void Cvar_List_f()
{
	cvar_t	*var;
	int		i;

	i = 0;
	for (var = cvar_vars ; var ; var = var->next, i++)
	{
		Con_Printf ("%s : %s", var->name, var->string);
		
		if (var->flags & FCVAR_ARCHIVE)
			Con_Printf (", a");
		
		if (var->flags & FCVAR_USERINFO)
			Con_Printf (", i");
		
		if (var->flags & FCVAR_SERVER)
			Con_Printf (", sv");
		
		Con_Printf ("\n");
	};
	
	Con_Printf("--------------\n");
	Con_Printf("%i Total CVars\n", i);
	Con_Printf("CvarList ? for syntax\n");
};

CConVar::CConVar(const char *asName, const char *asValue, int anFlags, const char *asDesc)
	: msDefValue(asValue), msDesc(asDesc)
{
	if(!mpData)
		mpData = new cvar_t;
	
	mpData->name = (char*)asName; // TODO
	mpData->string = (char*)msDefValue; // TODO
	
	mpData->flags = anFlags;
};

CConVar::~CConVar() //= default;
{
	if(mpData)
	{
		delete mpData;
		mpData = nullptr;
	};
};

/*
============
Cvar_VariableValue
============
*/
float Cvar_VariableValue(const char *var_name)
{
	auto var{Cvar_FindVar(var_name)};
	if(!var)
		return 0;
	return Q_atof(var->string);
};

/*
============
Cvar_VariableInt
============
*/
int Cvar_VariableInt(const char *var_name)
{
	cvar_t *var;

	var = Cvar_FindVar(var_name);
	if(!var)
		return 0;
	return Q_atoi(var->string); // TODO
};

/*
============
Cvar_VariableString
============
*/
const char *Cvar_VariableString(const char *var_name)
{
	auto var{Cvar_FindVar(var_name)};
	
	if(!var)
		return cvar_null_string;
	
	return var->string;
}

/*
============
Cvar_Set
============
*/
void Cvar_Set(const char *var_name, const char *value)
{
	auto var{Cvar_FindVar(var_name)};
	if(!var)
	{
		// there is an error in C code if this happens
		gpSystem->Printf("Cvar_Set: variable %s not found\n", var_name);
		return;
	};

	bool changed = Q_strcmp(var->string, value);

	Z_Free(var->string); // free the old value string

	var->string = (char*)Z_Malloc(Q_strlen(value) + 1);
	Q_strcpy(var->string, value);
	var->value = Q_atof(var->string);
	//if (var->flags & FCVAR_SERVER && changed) // TODO (was var->server)
	{
		if(sv.active)
			SV_BroadcastPrintf("\"%s\" changed to \"%s\"\n", var->name, var->string);
	}
}

/*
============
Cvar_SetValue
============
*/
void Cvar_SetValue(const char *var_name, float value)
{
	char val[32]{};

	sprintf(val, "%f", value);
	Cvar_Set(var_name, val);
};

cvar_t *Cvar_RegisterClientVariable(const char *name, const char *value, int flags)
{
	//flags |= FCVAR_CLIENT;
	// TODO
	return nullptr;
};

/*
============
Cvar_Command

Handles variable inspection and changing from the console
============
*/
bool Cvar_Command()
{
	// check variables
	auto v = Cvar_FindVar(Cmd_Argv(0));
	if(!v)
		return false;

	// perform a variable print or set
	if(Cmd_Argc() == 1)
	{
		gpSystem->Printf("\"%s\" is \"%s\"\n", v->name, v->string);
		return true;
	};

	Cvar_Set(v->name, Cmd_Argv(1));
	return true;
};

/*
============
Cvar_Init

Reads in all archived cvars
============
*/
void Cvar_Init()
{
	//Cmd_AddCommand("set", Cvar_Set_f);
	Cmd_AddCommand("cvarlist", Cvar_List_f);
};