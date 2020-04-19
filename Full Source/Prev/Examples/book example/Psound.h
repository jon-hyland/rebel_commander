//psound.h: header class for psound.cpp
//CGameSoundManager extends CSoundManager by allowing pause
//Copyright Ian Parberry, 2000
//Last updated March 14, 2000

#ifndef __PSOUND__
#define __PSOUND__

#include "sound.h" //simple sound manager

class CGameSoundManager:public CSoundManager{ 
  //game sound manager - can be paused
  private:
    BOOL *m_bPausedPlaying[MAX_SOUNDS]; //paused while playing
    BOOL *m_bPausedLooping[MAX_SOUNDS]; //paused while looping
    BOOL CreateBuffer(int index,int length,int copies);
  public:
    CGameSoundManager(HWND hwnd); //constructor
    void clear(); //clear all sounds
    void pause(int index); //pause sound
    void pause(void); //pause all sounds
    void unpause(int index); //unpause sound
    void unpause(void); //unpause all sounds
};

#endif
