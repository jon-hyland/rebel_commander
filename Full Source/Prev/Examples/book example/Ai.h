//ai.h: header file for intelligent object class

//Copyright Ian Parberry, 1999
//Last updated October 25, 1999

#ifndef __AI__
#define __AI__

enum StateType{CRUISING_STATE,AVOIDING_STATE};

#include "objects.h"

class CIntelligentObject: public CObject{
  private:
    StateType m_eState; //state
    int m_nDesiredHeight; //desired altitude
    int m_nHeightTime; //time between height changes
    int m_nHeightDelayTime; //time to next height change
    int m_nSpeedVariationTime; //last time speed varied
    int m_nSpeedVariationDuration; //time to next speed vrn
    int m_nLastAiTime; //last time AI was used
    int m_nAiDelayTime; //time until AI next used
    int m_nDistance; //distance to plane
    int m_nVerticalDistance; //vertical distance from plane
    int m_nHorizontalDistance; //hor. distance from plane
    void ai(); //artificial intelligence
    void cruising_ai(); //ai for cruising along
    void avoiding_ai(); //ai for avoiding plane
    void set_state(StateType state); //change state
  public:
    CIntelligentObject(ObjectType object,int x,int y,
      int xspeed,int yspeed); //constructor
    virtual void move(); //move depending on time and speed
    void plane(int x,int y,int d); //relationship w/plane
};

#endif
