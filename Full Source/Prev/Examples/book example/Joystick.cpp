//joystick.cpp: simple joystick handler

//Copyright Ian Parberry, 1999
//Last updated March 14, 2000

#include "joystick.h"
#include "gtimer.h" //game timer

extern CGameTimer Timer; //game timer

CJoystickManager::CJoystickManager(){ //constructor
  m_nLastPollTime=0; //not polled yet
  m_bOperational=FALSE; //not initialized yet
  m_uJoystickid=0; m_bAutoRepeat=TRUE;
  m_nX=m_nY=0; m_nXmax=m_nYmax=m_nXmin=m_nYmin=0;
  for(int i=0; i<4; i++) //init button positions
    m_bServiced[i]=m_bButton[i]=FALSE;
}

BOOL CJoystickManager::initialize(){ //init joystick
  m_bOperational=FALSE; //assume failure
  //attempt to init joystick
  if(!joyGetNumDevs())return FALSE; //bail if no joysticks
  //init JOYINFOEX
  memset(&m_joyInfo,0,sizeof(JOYINFOEX));
  m_joyInfo.dwSize=sizeof(JOYINFOEX);
  m_joyInfo.dwFlags=JOY_RETURNBUTTONS|JOY_RETURNX|JOY_RETURNY;
  //get id of first of 2 possible joysticks
  if(joyGetPosEx(JOYSTICKID1,&m_joyInfo)==JOYERR_NOERROR)
    m_uJoystickid=JOYSTICKID1;
  else if(joyGetPosEx(JOYSTICKID2,&m_joyInfo)==JOYERR_NOERROR)
    m_uJoystickid=JOYSTICKID2;
  else return FALSE;
  //get min and max extents of joystick
  JOYCAPS joycaps;
  if(joyGetDevCaps(m_uJoystickid,&joycaps,
  sizeof(joycaps))!=JOYERR_NOERROR)
    return FALSE; //bail if can't get caps info
  m_nXmax=joycaps.wXmax; m_nYmax=joycaps.wYmax; //max extent
  m_nXmin=joycaps.wXmin; m_nYmin=joycaps.wYmin; //min extent
  //init data fields with fresh info
  load_data();
  //record success
  return m_bOperational=TRUE;
}

void CJoystickManager::load_data(){ //process joystick data
  //buttons
  m_bButton[0]=m_joyInfo.dwButtons&1;
  m_bButton[1]=m_joyInfo.dwButtons&2;
  m_bButton[2]=m_joyInfo.dwButtons&4;
  m_bButton[3]=m_joyInfo.dwButtons&8;
  //X position: normalize to range 0..100
  if(m_nXmax!=m_nXmin)
    m_nX=(100*(m_joyInfo.dwXpos-m_nXmin))/(m_nXmax-m_nXmin);
  if(m_nX<0)m_nX=0; if(m_nX>100)m_nX=100;
  //Y position: normalize to range 0..100
  if(m_nYmax!=m_nYmin)
    m_nY=(100*(m_joyInfo.dwYpos-m_nYmin))/(m_nYmax-m_nYmin);
  if(m_nY<0)m_nY=0; if(m_nY>100)m_nY=100;
}

BOOL CJoystickManager::poll(){ //poll joystick state
  if(!m_bOperational)return FALSE; //bail if not initialized
  //allow only infrequent polls
  if(!Timer.elapsed(m_nLastPollTime,JOYSTICKPOLLINTERVAL))
    return FALSE;
  //get new data into JOYINFOEX member
  if(joyGetPosEx(m_uJoystickid,&m_joyInfo)==JOYERR_UNPLUGGED)
    return FALSE; //bail if cannot get data from joystick
  //decode data into member variables
  load_data();
  //init serviced flags for noautorepeat
  for(int i=0; i<4; i++)
    if(!m_bButton[i])m_bServiced[i]=FALSE;
  //done
  return TRUE;
}

BOOL CJoystickManager::button_down(int button){ 
//is button down?
  BOOL result;
  button--; //adjust for 0-based array
  if(!m_bOperational)return FALSE; //bail if not initialized
  if(button<0||button>=4)return FALSE; //bail if wrong number
  result=m_bButton[button]; //is button down?
  m_bButton[button]=FALSE; //pop it up
  if(!m_bAutoRepeat&&result&&m_bServiced[button])
    return FALSE; //prevent button bounce
  if(result) //if button is down
    m_bServiced[button]=TRUE; //mark as serviced
  return result;
}

BOOL CJoystickManager::position(POINT &point){ 
  if(!m_bOperational)return FALSE; //bail if not initialized
  point.x=m_nX; point.y=m_nY;
  return TRUE;
}

void CJoystickManager::autorepeat(BOOL setting){ 
  m_bAutoRepeat=setting; //allow autorepeat
  for(int i=0; i<4; i++) //reset buttons
    m_bServiced[i]=m_bButton[i]=FALSE;
}

BOOL CJoystickManager::exists(){ 
  //TRUE if joystick apparently exists
  return m_bOperational;
}
