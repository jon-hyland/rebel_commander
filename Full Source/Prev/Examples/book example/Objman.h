//objman.h: header file for the object manager

//Copyright Ian Parberry, 1999
//Last updated December 7, 1999

#ifndef __OBJMAN__
#define __OBJMAN__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "objects.h"

class CObjectManager{
  private:
    CObject **m_pObjectList; //list of objects in game
    int m_nCount; //how many objects in list
    int m_nMaxCount; //maximum number of objects
    int m_nCurrentObject; //index of the current object
    int m_nLastGunFireTime; //time gun was last fired
    int m_nStartInvulnerableTime; //time level started
    //distance functions
    int distance(int x1,int y1,int x2,int y2); //in universe
    int distance(int first,int second); //between objects
    //collision detection
    void collision_detection(); //all collisions
    BOOL collision_detection(int index); //with this object
    //managing dead objects
    void cull(); //cull dead objects
    void kill(int index); //remove object from list
    void replace(int index); //replace by next in series
  public:
    CObjectManager(int max); //constructor
    ~CObjectManager(); //destructor
    void reset(); //reset to original conditions
    BOOL won(); //TRUE if all enemies dead
    int create(ObjectType object,int x,int y,
      int xspeed,int yspeed); //create new object
    int create(int index,ObjectType object,
      int x,int y,int xspeed,int yspeed); //create at index
    //animate all objects
    void animate(LPDIRECTDRAWSURFACE surface);
    //the following functions operate on the current object
    void accelerate(int xdelta,int ydelta=0); //change speed
    void set_current(int index); //set current object
    int speed(); //return magnitude of speed
    void fire_gun(); //fire the gun
    void set_speed(int xdelta,int ydelta); //of current object
    void speed_limits(int &xmin,int &xmax,
      int &ymin,int &ymax); //speed limits 
    int enemies(); //number of enemies left
};

#endif
