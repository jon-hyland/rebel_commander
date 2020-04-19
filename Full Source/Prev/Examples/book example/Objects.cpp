//objects.cpp

//Copyright Ian Parberry, 1999
//Last updated November 9, 1999

#include "objects.h"
#include "gtimer.h" //game timer
#include "csprite.h" //for clipped sprite class
#include "random.h" //for random number generator
#include "view.h" //for viewpoint class
#include "sound.h" //for sound manager
#include "score.h" //for score manager

extern CClippedSprite *g_pSprite[]; //sprites
extern CGameTimer Timer; //game timer
extern CRandom Random; //random number generator
extern CViewPoint Viewpoint; //player viewpoint
extern CSoundManager* SoundManager; //sound manager
extern CScoreManager ScoreManager; //score manager

CObject::CObject(ObjectType object,int x,int y,
                 int xspeed,int yspeed){ //constructor
  //defaults
  m_nCurrentFrame=0; m_nLastFrameTime=Timer.time();
  m_bForwardAnimation=TRUE; m_nFrameInterval=30;
  m_eMortality=IMMORTAL; m_eLocomotion=NO_MOTION;
  m_nLifeTime=1000; m_bVulnerable=FALSE; 
  m_bIntelligent=FALSE;
  //common values
  m_eObject=object; //type of object
  m_nLastXMoveTime=m_nLastYMoveTime=Timer.time(); //time
  m_nX=x; m_nY=y; //location
  m_nXspeed=xspeed; m_nYspeed=yspeed; //speed
  m_pSprite=g_pSprite[object];
  m_nFrameCount=m_pSprite->frame_count();
  m_nHeight=m_pSprite->height();
  m_nWidth=m_pSprite->width();
  m_nBirthTime=Timer.time(); //time of creation
  //customized values
  switch(object){
    case PLANE_OBJECT:
      switch(abs(m_nXspeed)){ //depending on horizontal speed
        case 1:
          SoundManager->play(SLOWPUTT_SOUND,LOOP_SOUND);
          break;
        case 2:
          SoundManager->play(MEDIUMPUTT_SOUND,LOOP_SOUND);
          break;
        case 3:
          SoundManager->play(FASTPUTT_SOUND,LOOP_SOUND);
          break;
      }
      m_nMinXSpeed=-3; m_nMaxXSpeed=-1;
      m_nMinYSpeed=-4; m_nMaxYSpeed=4;
      m_nFrameInterval=250;
      m_eMortality=IMMORTAL;
      m_eLocomotion=FLYING_MOTION;
      m_bVulnerable=FALSE;
      break;  
    case EXPLODINGPLANE_OBJECT:
      m_nYspeed=0;
      m_nMinXSpeed=-3; m_nMaxXSpeed=-1;
      m_nMinYSpeed=-4; m_nMaxYSpeed=4;
      m_nFrameInterval=200;
      m_eMortality=ONESHOT_MORTAL; m_bVulnerable=FALSE;
      m_eLocomotion=FLYING_MOTION;
      break;
    case DEADPLANE_OBJECT:
      SoundManager->play(SMARTS_SOUND);
      m_nYspeed=0;
      m_nMinXSpeed=-3; m_nMaxXSpeed=-1;
      m_nMinYSpeed=-4; m_nMaxYSpeed=4;
      m_nFrameInterval=3000;
      m_eMortality=ONESHOT_MORTAL; m_bVulnerable=FALSE;
      m_eLocomotion=FLYING_MOTION;
      break;
    case CROW_OBJECT:
      m_nMinXSpeed=-2; m_nMaxXSpeed=-1;
      m_nMinYSpeed=-1; m_nMaxYSpeed=1;
      m_nCurrentFrame=Random.number(0,m_nFrameCount-1);
      m_nFrameInterval=250+Random.number(-30,30);
      m_eMortality=IMMORTAL;
      m_eLocomotion=FLYING_MOTION;
      m_bVulnerable=TRUE;
      break;
    case FARM_OBJECT:
    case FIELD_OBJECT:
      m_nFrameCount=1;
      m_eMortality=IMMORTAL;
      m_eLocomotion=NO_MOTION;
      m_bVulnerable=FALSE;
      break;
    case BULLET_OBJECT:
      SoundManager->play(GUN_SOUND); //sound of gun firing
      m_nFrameCount=1;
      m_eMortality=MORTAL;
      m_eLocomotion=FLYING_MOTION;
      m_bVulnerable=FALSE;
      m_nLifeTime=500+Random.number(0,200);
      break;
    case DEADCROW_OBJECT:
      m_nCurrentFrame=0;
      m_eMortality=MORTAL;
      m_eLocomotion=FALLING_MOTION;
      m_bVulnerable=FALSE;
      m_nLifeTime=1000;
      break;
    case EXPLODINGCROW_OBJECT:
      SoundManager->play(BOOM_SOUND); //sound of explosion
      m_nCurrentFrame=0;
      m_nFrameInterval=100;
      m_eMortality=ONESHOT_MORTAL;
      m_eLocomotion=FLYING_MOTION;
      m_bVulnerable=FALSE;
      break;
  }
}


CObject::~CObject(){ //destructor
  if(m_eObject==PLANE_OBJECT){ //kill plane engine sounds
    SoundManager->stop(SLOWPUTT_SOUND);
    SoundManager->stop(MEDIUMPUTT_SOUND);
    SoundManager->stop(FASTPUTT_SOUND);
  }
}

void CObject::draw(LPDIRECTDRAWSURFACE surface){ //draw
  //draw the current frame
  m_pSprite->draw(m_nCurrentFrame,Viewpoint.screen(m_nX),
    m_nY,surface);
  //figure out which frame is next
  if(m_eMortality==ONESHOT_MORTAL){ //animation plays once
    if(Timer.elapsed(m_nLastFrameTime,m_nFrameInterval))
      ++m_nCurrentFrame;
  }
  else{ //repeating animation
    int t=m_nFrameInterval/(1+abs(m_nXspeed)); //frame interval
    if(m_nFrameCount>1&&Timer.elapsed(m_nLastFrameTime,t))
      if(m_bForwardAnimation){ //forward animation
        if(++m_nCurrentFrame>=m_nFrameCount){ //wrap
          m_nCurrentFrame=m_nFrameCount-2;
          m_bForwardAnimation=FALSE;
        }
      }
      else{ //backward animation
        if(--m_nCurrentFrame<0){ //wrap
          m_nCurrentFrame=1;
          m_bForwardAnimation=TRUE;
        }
      }
    }
  //draw damage to plane - paste on top of plane image
  if(m_eObject==PLANE_OBJECT){
    switch(ScoreManager.health()){ //depending on health
      case 1: //heavy damage
        g_pSprite[DAMAGEDPLANE_OBJECT]->
          draw(1,Viewpoint.screen(m_nX),m_nY,surface);
        break;
      case 2: //mild damage
        g_pSprite[DAMAGEDPLANE_OBJECT]->
          draw(0,Viewpoint.screen(m_nX),m_nY,surface);
        break;
      default: break;
    }
  }
}

void CObject::accelerate(int xdelta,int ydelta){ 
//change speed  
  int old_xspeed=m_nXspeed; //old speed
  //horizontal
  m_nXspeed+=xdelta;
  if(m_nXspeed<m_nMinXSpeed)m_nXspeed=m_nMinXSpeed;
  if(m_nXspeed>m_nMaxXSpeed)m_nXspeed=m_nMaxXSpeed;
  //vertical
  m_nYspeed+=ydelta;
  if(m_nYspeed<m_nMinYSpeed)m_nYspeed=m_nMinYSpeed;
  if(m_nYspeed>m_nMaxYSpeed)m_nYspeed=m_nMaxYSpeed;
  //change plane sound
  if(m_eObject==PLANE_OBJECT&&old_xspeed!=m_nXspeed){
    //stop old sound (one of these will work)
    SoundManager->stop(SLOWPUTT_SOUND);
    SoundManager->stop(MEDIUMPUTT_SOUND);
    SoundManager->stop(FASTPUTT_SOUND);
    //start new sound       
    SoundManager->
      play(SLOWPUTT_SOUND-1+abs(m_nXspeed),LOOP_SOUND);
  }
}

void CObject::move(){ //move object
  const int XSCALE=16; //to scale back horizontal motion
  const int YSCALE=32; //to scale back vertical motion
  const int YMARGIN=20; //vertical margin;
  int xdelta,ydelta; //change in position
  int time=Timer.time(); //current time
  int tfactor; //time since last move
  switch(m_eLocomotion){
    case FLYING_MOTION:
      //horizontal motion
      tfactor=time-m_nLastXMoveTime; //time since last move
      xdelta=(m_nXspeed*tfactor)/XSCALE; //x distance moved 
      m_nX+=xdelta; //x motion
      Viewpoint.normalize(m_nX); //normalize to world width
      if(xdelta||m_nXspeed==0) //record time of move
        m_nLastXMoveTime=time;
      //vertical motion
      tfactor=time-m_nLastYMoveTime; //time since last move
      ydelta=(m_nYspeed*tfactor)/YSCALE; //y distance moved
      m_nY+=ydelta; //y motion
      if(m_nY<YMARGIN)m_nY=YMARGIN;
      if(m_nY>=SCREEN_HEIGHT)m_nY=SCREEN_HEIGHT-1;
      if(ydelta||m_nYspeed==0) //record time of move
        m_nLastYMoveTime=time;
      break;
    case FALLING_MOTION:
      m_nXspeed=0;
      //time since born, for acceleration due to gravity
      if(time>m_nLastYMoveTime) //eg, if not paused
        tfactor=time-m_nBirthTime; //acceleration due to gravity
      else tfactor=0;
      //vertical motion
      ydelta=tfactor/YSCALE; m_nY+=ydelta;
      if(m_nY<YMARGIN)m_nY=YMARGIN;
      if(ydelta||m_nYspeed==0) //record time of move
        m_nLastYMoveTime=time; 
      //kill objects that have fallen below the screen
      if(m_nY>SCREEN_HEIGHT){
        SoundManager->play(THUMP_SOUND); //object hitting ground
        m_nLifeTime=0; m_eMortality=MORTAL; //force cull
      }
      break;
    default: break;
  }
}

void CObject::set_speed(int xspeed,int yspeed){ //set speed
  accelerate(xspeed-m_nXspeed,yspeed-m_nYspeed);
}
