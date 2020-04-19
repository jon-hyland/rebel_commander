#ifndef __SOUND_H__
#define __SOUND_H__

#ifndef WIN32
#define WIN32
#endif

#include <stdio.h>
#include <dsound.h>
#include "defines.h"

class CSoundManager
{
private:
	int m_nCount;
	LPDIRECTSOUND m_lpDirectSound;
	LPLPDIRECTSOUNDBUFFER m_lpBuffer[MAX_SOUNDS];
	int m_nCopyCount[MAX_SOUNDS];
	BOOL m_bOperational;
	BOOL CreateBuffer(int index, int length, int copies);
	BOOL LoadBuffer(int index, BYTE *buffer, int length);
	int LoadSound(char *filename, BYTE **sound);
	BOOL CopyBuffer(int index, int copies);
	BOOL m_bSoundOn;
public:
	CSoundManager(HWND hwnd);
	~CSoundManager();
	void Clear();
	void Load(char *filename, int copies=1);
	void Play(int index, BOOL looping=FALSE);
	void Stop(int index);
	void Stop();
	void TurnOn();
	void TurnOff();
	BOOL Operational();
	BOOL SoundOn();
};

#endif