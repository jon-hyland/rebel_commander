//sndlist.h: list of sound definitions

//Copyright Ian Parberry, 1999
//Last updated March 14, 2000

#ifndef __SNDLIST__
#define __SNDLIST__

#define LOOP_SOUND TRUE

enum GameSoundType{ //sounds used in game engine
  CAW_SOUND=0, //sound a crow makes
  GUN_SOUND, //sound of gun firing
  BOOM_SOUND, //sound of explosion
  THUMP_SOUND, //sound of object hitting the ground
  //next 3 sounds must be consecutive and in this order
  SLOWPUTT_SOUND, //sound of slow engine
  MEDIUMPUTT_SOUND, //sound of medium engine
  FASTPUTT_SOUND, //sound of fast engine
  //mouse clicks
  BIGCLICK_SOUND, //loud click
  SMALLCLICK_SOUND, //soft click
  //more game sounds
  SUCCESS_SOUND, //completion of level
  FAILED_SOUND, //sound of failing
  NEWLIFE_SOUND, //sound of getting a new life
  STINGS_SOUND, //that stings (damage)
  SMARTS_SOUND, //that smarts (damage)
  //sounds for high score list
  CONGRATS_SOUND, //congratulations
  TYPE_SPACE_SOUND, //typewriter typing a space
  TYPE_RETURN_SOUND, //typewriter typing a return
  TYPE_CHAR_SOUND, //typewriter typing a character
  PAGETURN_SOUND, //sound of page being turned
};

enum IntroSoundType{ //sounds used during the intro
  TITLE_SOUND=0, //sound used during title screen
  LOGO_SOUND, //signature chord
  CREDITS_SOUND, //played during credits
};

#endif
