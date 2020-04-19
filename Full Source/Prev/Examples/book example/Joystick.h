//joystick.h: header file for joystick.cpp
//joystick manager class

//Copyright Ian Parberry, 1999
//Last updated February 14, 2000

#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <windows.h>
#include <mmsystem.h>

#define JOYSTICKPOLLINTERVAL 100 //time between polls

class CJoystickManager{
  private:
    JOYINFOEX m_joyInfo; //current joystick state
    UINT m_uJoystickid; //joystick id
    int m_nX, m_nY; //x and y coordinates as percentages
    int m_nXmax, m_nYmax; //maximum x and y from h/w
    int m_nXmin, m_nYmin; //minimum x and y from h/w
    BOOL m_bButton[4]; //buttons down?
    BOOL m_bServiced[4]; //whether down button was serviced
    BOOL m_bOperational; //was init successful?
    int m_nLastPollTime; //last time joystick was polled
    BOOL m_bAutoRepeat; //allow autorepeat
    void load_data(); //process data from m_joyInfo
  public:
    CJoystickManager(); //constructor
    BOOL initialize(); //initialize settings
    BOOL poll(); //poll joystick state
    BOOL button_down(int button); //is button down?
    BOOL position(POINT &point); //position as percentage
    void autorepeat(BOOL setting=TRUE); //un/set autorepeat
    BOOL exists(); //TRUE if joystick exists
};

#endif
