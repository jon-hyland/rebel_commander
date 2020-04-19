//ai.cpp: artificial intelligence

//Copyright Ian Parberry, 1999
//Last updated March 14, 2000

#include "ai.h"
#include "gtimer.h" //game timer
#include "random.h" //for random number generator
#include "view.h" //for viewpoint manager
#include "sound.h" //for sound manager

extern CGameTimer Timer;  //game timer
extern CRandom Random; //random number generator
extern CSoundManager* SoundManager; //sound manager

const int CLOSE_DISTANCE=200; //close to plane
const int FAR_DISTANCE=300; //far from plane

//fall back at this vertical distance from plane
const int FALLBACK_DISTANCE=150; 

//horizontal distance considered to be behind plane
const int BEHIND_DISTANCE=-5; 

CIntelligentObject::CIntelligentObject(ObjectType object,
  int x,int y,int xspeed,int yspeed):
CObject(object,x,y,xspeed,yspeed){ //constructor
  m_bIntelligent=TRUE;
  m_nDesiredHeight=240; 
  m_nHeightTime=0;  m_nHeightDelayTime=0;
  m_nSpeedVariationTime=m_nSpeedVariationDuration=0;
  m_nDistance=m_nHorizontalDistance=m_nVerticalDistance=0;
  m_eState=CRUISING_STATE;
  m_nLastAiTime=0; m_nAiDelayTime=0;
}

void CIntelligentObject::move(){ //move object
  CObject::move(); //move like a dumb object
  ai(); //act intelligently
}

void CIntelligentObject::ai(){ //main AI function
  //do the following periodically
  if(Timer.elapsed(m_nLastAiTime,m_nAiDelayTime)) 
    switch(m_eState){ //behavior depends on state
      case CRUISING_STATE: cruising_ai(); break;
      case AVOIDING_STATE: avoiding_ai(); break;
      default: break;
    }
}

void CIntelligentObject::plane(int x, int y,int d){ 
//compute relationship with plane
  //distances from plane
  m_nDistance=d;
  m_nVerticalDistance=abs(m_nY-y);
  //horizontal distance
  m_nHorizontalDistance=m_nX-x;
  if(m_nHorizontalDistance>WORLD_WIDTH/2)
    m_nHorizontalDistance-=WORLD_WIDTH;
  if(m_nHorizontalDistance<-WORLD_WIDTH/2)
    m_nHorizontalDistance+=WORLD_WIDTH;
}

void CIntelligentObject::set_state(StateType state){
  m_eState=state; //change state
  switch(m_eState){ //change behavior settings
    case CRUISING_STATE:
      m_nAiDelayTime=300+Random.number(0,300);
      m_nXspeed=-1;
      m_nHeightDelayTime=8000+Random.number(0,5000);
      break;
    case AVOIDING_STATE:
      SoundManager->play(CAW_SOUND); //sound of crow cawing
      m_nAiDelayTime=200+Random.number(0,200); 
      m_nXspeed=-3;
      m_nDesiredHeight=Random.number(100,400);
      m_nHeightDelayTime=3000+Random.number(0,2000);
      m_nSpeedVariationDuration=5000+Random.number(0,2000);
      break;
    default: break;
  }
}

void CIntelligentObject::cruising_ai(){ //just cruising along
  //height variation
  if(m_nDesiredHeight<m_nY-20)m_nYspeed=-1;
  else if(m_nDesiredHeight>m_nY+20)m_nYspeed=1;
  else m_nYspeed=0;
  if(Timer.elapsed(m_nHeightTime,m_nHeightDelayTime)){
    m_nDesiredHeight=Random.number(150,400);
    m_nHeightDelayTime=15000+Random.number(0,5000);
  }
  //look for plane
  if(m_nDistance<CLOSE_DISTANCE&&
  m_nHorizontalDistance<BEHIND_DISTANCE)
    set_state(AVOIDING_STATE);
}

void CIntelligentObject::avoiding_ai(){ //avoiding plane
  //height variation
  if(Timer.elapsed(m_nHeightTime,m_nHeightDelayTime)){
    m_nDesiredHeight=Random.number(100,450);
    if(m_nDesiredHeight<m_nY)m_nYspeed=-2;
    if(m_nDesiredHeight>m_nY)m_nYspeed=2;
    m_nHeightDelayTime=3000+Random.number(0,2000);
  }
  //speed variation
  if(Timer.elapsed(m_nSpeedVariationTime,
  m_nSpeedVariationDuration))
    if(m_nXspeed==-2){
      m_nXspeed=-3; 
      m_nSpeedVariationDuration=10000+Random.number(0,3000);
    }
    else{
      m_nXspeed=-2; 
      m_nSpeedVariationDuration=5000+Random.number(0,2000);
    }
  if(m_nHorizontalDistance>BEHIND_DISTANCE) //if behind
    m_nXspeed=-1; //slow down 
  //look for plane, maybe leave avoiding state
  if(m_nDistance>FAR_DISTANCE|| //if far away, or
  (m_nDistance<CLOSE_DISTANCE&& //close and
  m_nVerticalDistance>FALLBACK_DISTANCE)) //higher or lower
    set_state(CRUISING_STATE); //then back to cruising
}
