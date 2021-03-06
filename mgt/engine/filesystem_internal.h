/*
 * This file is part of Magenta Engine
 *
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
/// @brief internal filesystem module wrapper

//
// file IO
//

#pragma once

#define FILESYSTEM_INTERNAL_H

#define FILESYSTEM_INVALID_HANDLE (FileHandle_t)0

enum FileSystemSeek_t
{
	FILESYSTEM_SEEK_HEAD = 0,
	FILESYSTEM_SEEK_CURRENT,
	FILESYSTEM_SEEK_TAIL
	//...?
};

enum
{
	FILESYSTEM_INVALID_FIND_HANDLE = -1
};

enum FileWarningLevel_t
{
	FILESYSTEM_WARNING_QUIET = 0,      ///< Don't print anything
	FILESYSTEM_WARNING_REPORTUNCLOSED, ///< On shutdown, report names of files left unclosed
	FILESYSTEM_WARNING_REPORTUSAGE,    ///< Report number of times a file was opened/closed
	FILESYSTEM_WARNING_REPORTALLACCESS ///< Report all open/close events to console (SLOW!!!!)
};

struct IFile;

////////////
// TODO: here or somewhere else?????

void FileSystem_Init(const char *basedir, void *filesystemFactory); // TODO: return bool/int?
void FileSystem_Shutdown();
////////////

/// returns the file size
/// return -1 if file is not present
/// the file should be in BINARY mode for stupid OSs that care
IFile *FS_FileOpenRead(const char *path);

IFile *FS_FileOpenWrite(const char *path);

IFile *FS_FileOpen(const char *path, const char *mode);

void FS_FileClose(IFile *handle);

void FS_FileSeek(IFile *handle, int position);

int FS_FileRead(IFile *handle, void *dest, int count);
int FS_FileWrite(IFile *handle, const void *data, int count);

int FS_FileTime(const char *path);

int FS_FileSize(const char *path);

void FS_mkdir(const char *path);

// Dynamic Library Management

//void *FS_LoadLibrary(const char *name, const char *path);