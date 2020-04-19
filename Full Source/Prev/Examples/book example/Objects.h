//objects.h: header file for CObject class

//Copyright Ian Parberry, 1999
//Last updated December 7, 1999

#ifndef __OBJECTS__
#define __OBJECTS__

#include "bsprite.h"
#include "bmp.h"

//object types
enum ObjectType{CROW_OBJECT=0,PLANE_OBJECT,
  DAMAGEDPLANE_OBJECT,EXPLODINGPLANE_OBJECT,
  DEADPLANE_OBJECT,FARM_OBJECT,FIELD_OBJECT,BULLET_OBJECT,
  EXPLODINGCROW_OBJECT,DEADCROW_OBJECT,NUM_SPRITES};
//note: NUM_SPRITES must be last

enum MortalityType{MORTAL,IMMORTAL,ONESHOT_MORTAL};
enum LocomotionType{FLYING_MOTION,FALLING_MOTION,NO_MOTION};

class CObject{ //class for a moving object
  friend class CObjectManager;
  protected:
    int m_nX,m_nY; //current location
    int m_nXspeed,m_nYspeed; //current speed
    int m_nLastXMoveTime; //last time moved horizontally
    int m_nLastYMoveTime; //last time moved vertically
    CBaseSprite *m_pSprite; //pointer to sprite
    int m_nWidth,m_nHeight; //width and height of sprite
    int m_nMinXSpeed,m_nMaxXSpeed; //min, max horizontal speeds
    int m_nMinYSpeed,m_nMaxYSpeed; //min, max vertical speeds
    int m_nCurrentFrame; //frame to be displayed
    int m_nFrameCount; //number of frames in animation
    int m_nLastFrameTime; //last time the frame was changed
    int m_nFrameInterval; //interval between frames
    BOOL m_bForwardAnimation; //is animation going forwards?
    LocomotionType m_eLocomotion; //mode of travel
    ObjectType m_eObject; //what kind of object is this?
    MortalityType m_eMortality; //whether it dies or not
    int m_nBirthTime; //time of creation
    int m_nLifeTime; //time that object lives
    BOOL m_bVulnerable; //vulnerable to bullets
    BOOL m_bIntelligent; //TRUE if object is intelligent
  public:
    CObject(ObjectType object,int x,int y,
      int xspeed,int yspeed); //constructor
    ~CObject(); //destructor
    void draw(LPDIRECTDRAWSURFACE surface); //draw
    void accelerate(int xdelta,int ydelta=0); //change speed
    virtual void move(); //move depending on time and speed
    void set_speed(int xspeed,int yspeed); //set speed
};

#endif
