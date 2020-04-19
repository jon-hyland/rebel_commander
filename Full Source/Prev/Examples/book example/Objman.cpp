//objman.cpp: object manager class
//Copyright Ian Parberry, 1999
//Last updated December 7, 1999

#include <math.h> //for sqrt

#include "objman.h"
#include "view.h" //for viewpoint class
#include "gtimer.h" //game timer
#include "ai.h" //for intelligent objects
#include "score.h" //for score manager

//time that plane is invulnerable at start of level
const int INITIAL_INVULNERABLE_TIME=3000;

extern CViewPoint Viewpoint; //player viewpoint
extern CGameTimer Timer; //game timer
extern CScoreManager ScoreManager; //score manager

CObjectManager::CObjectManager(int max){ //constructor
  m_nMaxCount=max; m_nCount=0; m_nCurrentObject=0;
  m_nStartInvulnerableTime=Timer.time(); //start invulnerable
  m_nLastGunFireTime=0;
  m_pObjectList=new CObject*[max]; //object list
  for(int i=0; i<m_nMaxCount; i++) //create objects
    m_pObjectList[i]=NULL;
}

CObjectManager::~CObjectManager(){ //destructor
  for(int i=0; i<m_nMaxCount; i++) //for each object
    delete m_pObjectList[i]; //delete it
  delete[]m_pObjectList; //delete object list
}

int CObjectManager::create(ObjectType object,int x,int y,
                           int xspeed,int yspeed){
//create object at first available index
  if(m_nCount<m_nMaxCount){ //if room, create object
    int i=0;//index into object list
    while(m_pObjectList[i]!=NULL)i++; //find first free slot
    return create(i,object,x,y,xspeed,yspeed); //create it there
  }
  else return -1;
}

int CObjectManager::create(int index,ObjectType object,
                           int x,int y,int xspeed,int yspeed){
//create object at given index
  if(m_nCount<m_nMaxCount&&m_pObjectList[index]==NULL){
    if(object==CROW_OBJECT)
      m_pObjectList[index]=
        new CIntelligentObject(object,x,y,xspeed,yspeed);
    else
      m_pObjectList[index]=new CObject(object,x,y,xspeed,yspeed);
    m_nCount++;
    return index;
  }
  else return -1;
}

void CObjectManager::animate(LPDIRECTDRAWSURFACE surface){
  //move objects
  for(int i=0; i<m_nMaxCount; i++) //for each object slot
    if(m_pObjectList[i]!=NULL){ //if there's an object there
      m_pObjectList[i]->move(); //move it
      if(m_pObjectList[i]->m_bIntelligent) //if intelligent
        //tell object about plane current position
        ((CIntelligentObject*)m_pObjectList[i])->plane(
          m_pObjectList[m_nCurrentObject]->m_nX,
          m_pObjectList[m_nCurrentObject]->m_nY,
          distance(i,m_nCurrentObject));
    }
  //move viewpoint with plane
  Viewpoint.set_position(
    m_pObjectList[m_nCurrentObject]->m_nX);
  //collision detection
  collision_detection();
  //cull old objects
  cull();
  //draw objects
  for(i=0; i<m_nMaxCount; i++) //for each object slot
    if(m_pObjectList[i]!=NULL) //if there's an object there
      m_pObjectList[i]->draw(surface); //draw it
}

void CObjectManager::accelerate(int xdelta,int ydelta){ 
  //change speed of current object
  CObject *plane= m_pObjectList[m_nCurrentObject];
  if(plane->m_eObject==PLANE_OBJECT) //if not dead or exploding
    plane->accelerate(xdelta,ydelta);
}

void CObjectManager::set_current(int index){
  //set current object
  if(index>=0&&index<m_nCount)m_nCurrentObject=index;
}

int CObjectManager::speed(){ 
  //return magnitude of current object speed
  return abs(m_pObjectList[m_nCurrentObject]->m_nXspeed);
}

int CObjectManager::distance(int x1,int y1,int x2,int y2){
//return distance in universe
  int x=abs(x1-x2),y=abs(y1-y2);//x and y distance
  //compensate for wrap-around world
  if(x>WORLD_WIDTH/2)x-=WORLD_WIDTH;
  //return result
  return (int)sqrt((double)x*x+(double)y*y);
}

int CObjectManager::distance(int first,int second){
//return distance between objects
  //bail if bad index
  if(first<0||first>=m_nMaxCount)return -1;
  if(second<0||second>=m_nMaxCount)return -1;
  //get coordinates of centers
  int x1,y1,x2,y2; //coordinates of objects
  x1=m_pObjectList[first]->m_nX;
  y1=m_pObjectList[first]->m_nY-
    m_pObjectList[first]->m_nHeight/2;
  x2=m_pObjectList[second]->m_nX;
  y2=m_pObjectList[second]->m_nY-
    m_pObjectList[second]->m_nHeight/2;
  //return distance between coordinates
  return distance(x1,y1,x2,y2);
}

void CObjectManager::kill(int index){ //remove object
  //add to score for killing it
  ScoreManager.add_to_score(m_pObjectList[index]->m_eObject);
  //housekeeping
  m_nCount--;
  delete m_pObjectList[index];
  m_pObjectList[index]=NULL;
}

void CObjectManager::fire_gun(){ //fire current object's gun
  CObject *plane= m_pObjectList[m_nCurrentObject];
  if(plane->m_eObject==PLANE_OBJECT) //if not dead or exploding
    if(Timer.elapsed(m_nLastGunFireTime,200))
      create(BULLET_OBJECT,plane->m_nX-60,plane->m_nY-50,-5,0);
}

void CObjectManager::cull(){ //cull old objects
  CObject *object;
  for(int i=0; i<m_nMaxCount; i++){ //for each object
    object=m_pObjectList[i]; //current object
    if(object!=NULL){
      //died of old age
      if(object->m_eMortality==MORTAL&& //if mortal and ...
      //...lived long enough...
      (Timer.time()-object->m_nBirthTime>object->m_nLifeTime))
        kill(i); //...then kill it 
      else //one-shot animation
        //if object played one time only...
        if(object->m_eMortality==ONESHOT_MORTAL&& 
        //...and played once already...
        object->m_nCurrentFrame>=object->m_nFrameCount)
          replace(i); //...then replace the object
    }
  }
}

void CObjectManager::replace(int index){ 
//replace object by next in series
  CObject *object=m_pObjectList[index]; //current object
  ObjectType newtype;
  BOOL successor=TRUE; //assume it has a successor
  //decide on new object type
  switch(object->m_eObject){
    case CROW_OBJECT: newtype=EXPLODINGCROW_OBJECT; break;
    case EXPLODINGCROW_OBJECT: newtype=DEADCROW_OBJECT; break;
    case PLANE_OBJECT: newtype=EXPLODINGPLANE_OBJECT; break;
    case EXPLODINGPLANE_OBJECT:
      newtype=DEADPLANE_OBJECT; //you're dead
      ScoreManager.deduct_life(); //deduct a life from score
      break;
    case DEADPLANE_OBJECT: //you're done being dead
      newtype=PLANE_OBJECT; //reincarnate as a regular plane
      m_nStartInvulnerableTime=Timer.time(); //start invulnerable
      break;
    default: successor=FALSE; break; //has no successor
  }
  //replace old object with new one
  int x=object->m_nX,y=object->m_nY; //location
  int xspeed=object->m_nXspeed;
  int yspeed=object->m_nYspeed;
  kill(index); //kill old object
  if(successor) //if it has a successor
    create(index,newtype,x,y,xspeed,yspeed); //create new one
}

void CObjectManager::collision_detection(){ 
//check for all collisions
  for(int i=0; i<m_nMaxCount; i++) //for each object slot
    if(m_pObjectList[i]!=NULL) //if object exists
      switch(m_pObjectList[i]->m_eObject){ //depending on type
        case BULLET_OBJECT: //if it is a bullet
          if(collision_detection(i)) //if object hit something
            kill(i); //kill object doing the hitting
          break;
        case PLANE_OBJECT: //if it was the plane
          if(Timer.time()>=m_nStartInvulnerableTime+
          INITIAL_INVULNERABLE_TIME) //if plane is vulnerable
            if(collision_detection(i)){ //if plane hit something
              if(ScoreManager.health()<=1) //if health low
                replace(m_nCurrentObject); //explosion
              ScoreManager.collision(); //register collision
            }
          break;
      }
}

BOOL CObjectManager::collision_detection(int index){
//check whether object with this index collides
  int i=0; //index of object collided with
  BOOL finished=FALSE; //finished when collision detected
  while(i<m_nMaxCount&&!finished){
    if(m_pObjectList[i]!=NULL) //if i is a valid object index
      if(m_pObjectList[i]->m_bVulnerable&& //if vulnerable
      distance(index,i)<m_pObjectList[i]->m_nHeight/2+
      m_pObjectList[index]->m_nHeight/2){ //and close enough
        finished=TRUE; //hit found
        replace(i); //replace object that is hit
      }
    ++i; //next object
  }
  return finished;
}

void CObjectManager::reset(){ //reset to original conditions
  m_nCount=0; m_nCurrentObject=0; m_nLastGunFireTime=0;
  m_nStartInvulnerableTime=Timer.time(); //start invulnerable
  for(int i=0; i<m_nMaxCount; i++){
    delete m_pObjectList[i];
    m_pObjectList[i]=NULL;
  }
}

BOOL CObjectManager::won(){ //TRUE if enemies all dead
  int count=0; //how many enemies left
  for(int i=0; i<m_nMaxCount; i++)
    if(m_pObjectList[i]!=NULL)
     switch(m_pObjectList[i]->m_eObject){
       case CROW_OBJECT:
       case EXPLODINGCROW_OBJECT:
       case DEADCROW_OBJECT:
         count++;
         break;
     }
  return count<=0;
}

void CObjectManager::set_speed(int xdelta,int ydelta){ 
  CObject *plane=m_pObjectList[m_nCurrentObject];
  if(plane->m_eObject==PLANE_OBJECT)
    plane->set_speed(xdelta,ydelta);
}

void CObjectManager::speed_limits(int &xmin,int &xmax,
                                  int &ymin,int &ymax){
//return speed limits on current object
  CObject *plane=m_pObjectList[m_nCurrentObject];
  xmin=plane->m_nMinXSpeed; xmax=plane->m_nMaxXSpeed;
  ymin=plane->m_nMinYSpeed; ymax=plane->m_nMaxYSpeed;
}

int CObjectManager::enemies(){ //return number of enemies
  int count=0; //how many enemies left
  for(int i=0; i<m_nMaxCount; i++)
    if(m_pObjectList[i]!=NULL)
     switch(m_pObjectList[i]->m_eObject){
       case CROW_OBJECT:
       case EXPLODINGCROW_OBJECT:
       case DEADCROW_OBJECT:
         count++;
         break;
     }
  return count;
}
