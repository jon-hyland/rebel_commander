//score.cpp: score manager class
//Copyright Ian Parberry, 1999
//Last updated December 7, 1999

#include <windows.h>
#include <windowsx.h>

#include "score.h"
#include "sound.h" //for sound manager

extern CSoundManager* SoundManager; //sound manager
extern BOOL endphase; //level ends when last life gone

const int START_LIVES=3; //initial number of lives
const int MAX_HEALTH=3; //maximum plane health
const int SCORE_PER_LIFE=100; //pts to earn another life

CScoreManager::CScoreManager(){ //constructor
  reset();
}

void CScoreManager::reset(){ //reset to start conditions
  m_nScore=0; m_nHealth=MAX_HEALTH; m_nLives=START_LIVES;
}

void CScoreManager::add_to_score(int delta){
  //add delta to score
  int oldscore=m_nScore; //old score
  m_nScore+=delta; //add to score
  //compute new lives, if any
  int newlives= //new lives earned
    m_nScore/SCORE_PER_LIFE-oldscore/SCORE_PER_LIFE;
  if(newlives) //play sound if nonzero
    SoundManager->play(NEWLIFE_SOUND);
  m_nLives+=newlives; //add new lives to counter
}

void CScoreManager::add_to_score(ObjectType object){
  //add score for killing object
  const int CROW_SCORE=3; //score for killing crow
  switch(object){ //score depends on object
    case DEADCROW_OBJECT: add_to_score(CROW_SCORE); break;
    default: break; //default is nothing
  }
}

void CScoreManager::collision(){ //collision, bad for health
  if(--m_nHealth>0) //deduct from health
    SoundManager->play(STINGS_SOUND); //play sound
}

void CScoreManager::deduct_life(){ //lose a life
  if(m_nHealth<=0){ //if health bad
    m_nHealth=MAX_HEALTH; //make healthy
    if(--m_nLives<0) //deduct a life, if last
      endphase=TRUE; //end of game
  }
}

int CScoreManager::lives(){ //return number of lives
  return m_nLives;
}

int CScoreManager::score(){ //return score
  return m_nScore;
}

int CScoreManager::health(){ //return health
  return m_nHealth;
}
