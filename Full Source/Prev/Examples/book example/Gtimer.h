//gtimer.h, header file for gtimer.cpp
//Copyright Ian Parberry, 2000
//Last updated April 4, 2000

#ifndef __gtimer_h__
#define __gtimer_h__

//system includes
#include <windows.h> //needed for BOOL

#include "timer.h" //for base timer class


class CGameTimer: public CTimer{ //game timer class
  private:
    int m_nPauseTime; //amount of time spent paused
    int m_nCurrentPauseStart; //last time timer was paused
    BOOL m_bPaused; //TRUE if currently paused
  public:
    CGameTimer(); //constructor
    void start(); //start the timer
    int time(); //return the time
    void pause(); //pause the timer
    void unpause(); //unpause the timer
};

#endif
