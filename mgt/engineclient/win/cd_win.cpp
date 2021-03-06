/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
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

#include <windows.h>
#include "quakedef.h"
#include "icdaudio.h"

extern HWND mainwindow;
extern cvar_t bgmvolume;

static bool cdValid = false;
static bool playing = false;
static bool wasPlaying = false;

static bool playLooping = false;
static float cdvolume;
static byte remap[100];
static byte cdrom;
static byte playTrack;
static byte maxTrack;

UINT wDeviceID;

class CCDAudio final : public ICDAudio
{
public:
	CCDAudio();
	~CCDAudio();

	int Init() override;
	void Shutdown() override;

	void Frame() override;

	void Pause() override;
	void Resume() override;

private:
	void Play(byte track, bool looping) /*override*/;
	void Stop() /*override*/;

	void Eject();
	void CloseDoor();

	int GetAudioDiskInfo();

	bool mbInitialized{ false };
	bool mbEnabled{ false };
};

int CCDAudio::Init()
{
	DWORD dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_SET_PARMS mciSetParms;
	int n;

	if(cls.state == ca_dedicated)
		return -1;

	if(COM_CheckParm("-nocdaudio"))
		return -1;

	mciOpenParms.lpstrDeviceType = "cdaudio";
	if(dwReturn = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_SHAREABLE, (DWORD)(LPVOID)&mciOpenParms))
	{
		Con_Printf("CDAudio_Init: MCI_OPEN failed (%i)\n", dwReturn);
		return -1;
	};
	wDeviceID = mciOpenParms.wDeviceID;

	// Set the time format to track/minute/second/frame (TMSF).
	mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;

	if(dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciSetParms))
	{
		Con_Printf("MCI_SET_TIME_FORMAT failed (%i)\n", dwReturn);
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD) nullptr);
		return -1;
	};

	for(n = 0; n < 100; n++)
		remap[n] = n;

	mbInitialized = true;
	mbEnabled = true;

	if(GetAudioDiskInfo())
	{
		Con_Printf("CDAudio_Init: No CD in player.\n");
		cdValid = false;
	};

	Cmd_AddCommand("cd", CD_f);

	Con_Printf("CD Audio Initialized\n");

	return 0;
};

void CCDAudio::Shutdown()
{
	if(!mbInitialized)
		return;

	Stop();

	if(mciSendCommand(wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD) nullptr))
		Con_DPrintf("CDAudio_Shutdown: MCI_CLOSE failed\n");
};

void CDAudio::Frame() // TODO: was Update
{
	if(!mbEnabled)
		return;

	if(bgmvolume.value != cdvolume)
	{
		if(cdvolume)
		{
			Cvar_SetValue("bgmvolume", 0.0);
			cdvolume = bgmvolume.value;
			Pause();
		}
		else
		{
			Cvar_SetValue("bgmvolume", 1.0);
			cdvolume = bgmvolume.value;
			Resume();
		};
	};
};

void CCDAudio::Play(byte track, bool looping)
{
	DWORD dwReturn;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;

	if(!mbEnabled)
		return;

	if(!cdValid)
	{
		GetAudioDiskInfo();
		if(!cdValid)
			return;
	};

	track = remap[track];

	if(track < 1 || track > maxTrack)
	{
		Con_DPrintf("CDAudio: Bad track number %u.\n", track);
		return;
	};

	// don't try to play a non-audio track
	mciStatusParms.dwItem = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatusParms.dwTrack = track;
	dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD)(LPVOID)&mciStatusParms);
	if(dwReturn)
	{
		Con_DPrintf("MCI_STATUS failed (%i)\n", dwReturn);
		return;
	};

	if(mciStatusParms.dwReturn != MCI_CDA_TRACK_AUDIO)
	{
		Con_Printf("CDAudio: track %i is not audio\n", track);
		return;
	};

	// get the length of the track to be played
	mciStatusParms.dwItem = MCI_STATUS_LENGTH;
	mciStatusParms.dwTrack = track;
	dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD)(LPVOID)&mciStatusParms);
	if(dwReturn)
	{
		Con_DPrintf("MCI_STATUS failed (%i)\n", dwReturn);
		return;
	};

	if(playing)
	{
		if(playTrack == track)
			return;
		Stop();
	};

	mciPlayParms.dwFrom = MCI_MAKE_TMSF(track, 0, 0, 0);
	mciPlayParms.dwTo = (mciStatusParms.dwReturn << 8) | track;
	mciPlayParms.dwCallback = (DWORD)mainwindow;
	dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)(LPVOID)&mciPlayParms);
	if(dwReturn)
	{
		Con_DPrintf("CDAudio: MCI_PLAY failed (%i)\n", dwReturn);
		return;
	};

	playLooping = looping;
	playTrack = track;
	playing = true;

	if(cdvolume == 0.0)
		Pause();
};

void CCDAudio::Stop()
{
	DWORD dwReturn;

	if(!mbEnabled)
		return;

	if(!playing)
		return;

	if(dwReturn = mciSendCommand(wDeviceID, MCI_STOP, 0, (DWORD) nullptr))
		Con_DPrintf("MCI_STOP failed (%i)", dwReturn);

	wasPlaying = false;
	playing = false;
};

void CCDAudio::Pause()
{
	DWORD dwReturn;
	MCI_GENERIC_PARMS mciGenericParms;

	if(!mbEnabled)
		return;

	if(!playing)
		return;

	mciGenericParms.dwCallback = (DWORD)mainwindow;
	if(dwReturn = mciSendCommand(wDeviceID, MCI_PAUSE, 0, (DWORD)(LPVOID)&mciGenericParms))
		Con_DPrintf("MCI_PAUSE failed (%i)", dwReturn);

	wasPlaying = playing;
	playing = false;
};

void CCDAudio::Resume()
{
	DWORD dwReturn;
	MCI_PLAY_PARMS mciPlayParms;

	if(!mbEnabled)
		return;

	if(!cdValid)
		return;

	if(!wasPlaying)
		return;

	mciPlayParms.dwFrom = MCI_MAKE_TMSF(playTrack, 0, 0, 0);
	mciPlayParms.dwTo = MCI_MAKE_TMSF(playTrack + 1, 0, 0, 0);
	mciPlayParms.dwCallback = (DWORD)mainwindow;
	dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_TO | MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);
	if(dwReturn)
	{
		Con_DPrintf("CDAudio: MCI_PLAY failed (%i)\n", dwReturn);
		return;
	};
	playing = true;
}

static void CD_f()
{
	char *command;
	int ret;
	int n;
	int startAddress;

	if(Cmd_Argc() < 2)
		return;

	command = Cmd_Argv(1);

	if(Q_strcasecmp(command, "on") == 0)
	{
		mbEnabled = true;
		return;
	};

	if(Q_strcasecmp(command, "off") == 0)
	{
		if(playing)
			Stop();
		mbEnabled = false;
		return;
	};

	if(Q_strcasecmp(command, "reset") == 0)
	{
		mbEnabled = true;
		if(playing)
			Stop();
		for(n = 0; n < 100; n++)
			remap[n] = n;
		GetAudioDiskInfo();
		return;
	};

	if(Q_strcasecmp(command, "remap") == 0)
	{
		ret = Cmd_Argc() - 2;
		if(ret <= 0)
		{
			for(n = 1; n < 100; n++)
				if(remap[n] != n)
					Con_Printf("  %u -> %u\n", n, remap[n]);
			return;
		};
		for(n = 1; n <= ret; n++)
			remap[n] = Q_atoi(Cmd_Argv(n + 1));
		return;
	};

	if(Q_strcasecmp(command, "close") == 0)
	{
		CloseDoor();
		return;
	};

	if(!cdValid)
	{
		GetAudioDiskInfo();
		if(!cdValid)
		{
			Con_Printf("No CD in player.\n");
			return;
		};
	};

	if(Q_strcasecmp(command, "play") == 0)
	{
		Play((byte)Q_atoi(Cmd_Argv(2)), false);
		return;
	};

	if(Q_strcasecmp(command, "loop") == 0)
	{
		Play((byte)Q_atoi(Cmd_Argv(2)), true);
		return;
	};

	if(Q_strcasecmp(command, "stop") == 0)
	{
		Stop();
		return;
	};

	if(Q_strcasecmp(command, "pause") == 0)
	{
		Pause();
		return;
	};

	if(Q_strcasecmp(command, "resume") == 0)
	{
		Resume();
		return;
	};

	if(Q_strcasecmp(command, "eject") == 0)
	{
		if(playing)
			Stop();
		Eject();
		cdValid = false;
		return;
	};

	if(Q_strcasecmp(command, "info") == 0)
	{
		Con_Printf("%u tracks\n", maxTrack);
		if(playing)
			Con_Printf("Currently %s track %u\n", playLooping ? "looping" : "playing", playTrack);
		else if(wasPlaying)
			Con_Printf("Paused %s track %u\n", playLooping ? "looping" : "playing", playTrack);
		Con_Printf("Volume is %f\n", cdvolume);
		return;
	};
};

LONG CDAudio_MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(lParam != wDeviceID)
		return 1;

	switch(wParam)
	{
	case MCI_NOTIFY_SUCCESSFUL:
		if(playing)
		{
			playing = false;
			if(playLooping)
				Play(playTrack, true);
		};
		break;

	case MCI_NOTIFY_ABORTED:
	case MCI_NOTIFY_SUPERSEDED:
		break;

	case MCI_NOTIFY_FAILURE:
		Con_DPrintf("MCI_NOTIFY_FAILURE\n");
		Stop();
		cdValid = false;
		break;

	default:
		Con_DPrintf("Unexpected MM_MCINOTIFY type (%i)\n", wParam);
		return 1;
	};

	return 0;
};

void CCDAudio::Eject()
{
	DWORD dwReturn;

	if(dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD) nullptr))
		Con_DPrintf("MCI_SET_DOOR_OPEN failed (%i)\n", dwReturn);
};

void CCDAudio::CloseDoor()
{
	DWORD dwReturn;

	if(dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_CLOSED, (DWORD) nullptr))
		Con_DPrintf("MCI_SET_DOOR_CLOSED failed (%i)\n", dwReturn);
};

int CCDAudio::GetAudioDiskInfo()
{
	DWORD dwReturn;
	MCI_STATUS_PARMS mciStatusParms;

	cdValid = false;

	mciStatusParms.dwItem = MCI_STATUS_READY;
	dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)(LPVOID)&mciStatusParms);
	if(dwReturn)
	{
		Con_DPrintf("CDAudio: drive ready test - get status failed\n");
		return -1;
	};

	if(!mciStatusParms.dwReturn)
	{
		Con_DPrintf("CDAudio: drive not ready\n");
		return -1;
	};

	mciStatusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
	dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)(LPVOID)&mciStatusParms);
	if(dwReturn)
	{
		Con_DPrintf("CDAudio: get tracks - status failed\n");
		return -1;
	};

	if(mciStatusParms.dwReturn < 1)
	{
		Con_DPrintf("CDAudio: no music tracks\n");
		return -1;
	};

	cdValid = true;
	maxTrack = mciStatusParms.dwReturn;

	return 0;
};