//psound.cpp: pausable sound manager

//Copyright Ian Parberry, 2000
//Last updated March 14, 2000

#include "psound.h"

CGameSoundManager::CGameSoundManager(HWND hwnd):
CSoundManager(hwnd){ //constructor
  for(int i=0; i<MAX_SOUNDS; i++)
    m_bPausedPlaying[i]=m_bPausedLooping[i]=NULL;
}

void CGameSoundManager::clear(){ //clear all sounds
  CSoundManager::clear();
  for(int i=0; i<m_nCount; i++){ //for each sound
    delete[]m_bPausedPlaying[i];
    delete[]m_bPausedLooping[i];
  }
}

BOOL CGameSoundManager::CreateBuffer(int index,int length,
                                     int copies){
  //create and initialize pause flags
  m_bPausedPlaying[index]=new BOOL[copies];
  m_bPausedLooping[index]=new BOOL[copies];
  for(int i=0; i<copies; i++) //initialize them
    m_bPausedPlaying[index][i]=m_bPausedLooping[index][i]=FALSE;
  //create sound buffers
  return CSoundManager::CreateBuffer(index,length,copies);
}

void CGameSoundManager::pause(int index){ //pause sound
  if(!m_bOperational)return; //bail if not initialized correctly
  if(index<0||index>=m_nCount)return; //bail if bad index
  DWORD dwStatus=0; //to save status of sound copy
  for(int i=0; i<m_nCopyCount[index]; i++){ //for each copy
    //record status for later unpausing
    if(SUCCEEDED(m_lpBuffer[index][i]->GetStatus(&dwStatus))){ 
      if(dwStatus&DSBSTATUS_LOOPING){ //looping
        m_bPausedLooping[index][i]=TRUE;
        m_bPausedPlaying[index][i]=FALSE;
      }
      else if(dwStatus&DSBSTATUS_PLAYING){ //playing not looping
        m_bPausedLooping[index][i]=FALSE;
        m_bPausedPlaying[index][i]=TRUE;
      }
      else{ //not playing
        m_bPausedLooping[index][i]=FALSE;
        m_bPausedPlaying[index][i]=FALSE;
      }
    }
    else{ //if data is invalid, assume not in use
      m_bPausedLooping[index][i]=FALSE;
      m_bPausedPlaying[index][i]=FALSE;
    }
    //stop sound copy (stays at same place in sound)
    m_lpBuffer[index][i]->Stop(); //stop playing
  }
}

void CGameSoundManager::pause(void){ //pause all sounds
  if(!m_bOperational)return; //bail if not initialized correctly
  for(int i=0; i<m_nCount; i++)pause(i); //pause them all
}

void CGameSoundManager::unpause(int index){ //unpause sound
  if(!m_bOperational)return; //bail if not initialized correctly
  if(index<0||index>=m_nCount)return; //bail if bad index
  for(int i=0; i<m_nCopyCount[index]; i++) //for each copy
    if(m_bPausedPlaying[index][i]) //was paused while playing
      m_lpBuffer[index][i]->Play(0,0,0); //restart playing
    else if(m_bPausedLooping[index][i]) //was paused looping
      m_lpBuffer[index][i]->Play(0,0,DSBPLAY_LOOPING); //so loop
}

void CGameSoundManager::unpause(void){ //unpause all sounds
  if(!m_bOperational)return; //bail if not initialized correctly
  for(int i=0; i<m_nCount; i++)unpause(i); //unpause them all
}
