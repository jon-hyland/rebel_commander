//gtimer.cpp, the game timer class
//Copyright Ian Parberry, 2000
//Last updated March 14, 2000

#include "gtimer.h"

CGameTimer::CGameTimer():CTimer(){ //constructor
  m_nPauseTime=m_nCurrentPauseStart=0; m_bPaused=TRUE;
}

void CGameTimer::start(){ //start the timer
  m_bPaused=FALSE; CTimer::start();
}

void CGameTimer::pause(){ //pause the timer
  m_bPaused=TRUE; m_nCurrentPauseStart=timeGetTime();
}

void CGameTimer::unpause(){ //unpause the timer
  m_bPaused=FALSE;
  m_nPauseTime+=timeGetTime()-m_nCurrentPauseStart;
}

int CGameTimer::time(){ //return the time
  int current_time=timeGetTime();
  int result=current_time-m_nPauseTime;
  if(m_bPaused)result-=current_time-m_nCurrentPauseStart;
  return result-m_nStartTime;
}
