//score.h: header file for score manager class

//Copyright Ian Parberry, 1999
//Last updated December 7, 1999

#ifndef __SCORE__
#define __SCORE__

#include "objects.h" //for object types

const int LEVEL_BONUS_SCORE=10; //pts for level completion

class CScoreManager{
  private:
    int m_nScore; //points scored
    int m_nHealth; //how many times plane has been hit
    int m_nLives; //how many lives the player has
  public:
    CScoreManager(); //constructor
    void reset(); //reset to start conditions
    int score(); //return current points score
    int health(); //return plane health
    int lives(); //return number of lives
    void add_to_score(int delta); //add delta to score
    void add_to_score(ObjectType object); //for killing object
    void collision(); //collision, bad for health
    void deduct_life(); //lose a life if health bad
};

#endif
