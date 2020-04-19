//defines.h: essential defines
//Copyright Ian Parberry, 1999
//Last updated March 14, 2000

#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SCREEN_WIDTH 640 //pixels wide
#define SCREEN_HEIGHT 480 //pixels high
#define COLORS 256 //number of colors
#define COLOR_DEPTH 8 //number of bits to store colors
#define TRANSPARENT_COLOR 255 //transparent palette position

enum GamePhaseType{
  LOGO_PHASE,TITLE_PHASE,CREDITS_PHASE,MENU_PHASE,PLAYING_PHASE,
  DEVICEMENU_PHASE,SUCCESS_PHASE,HIGHSCORE_PHASE,
  NEWHIGHSCORE_PHASE,HELP_PHASE
};

enum InputDeviceType{KEYBOARD_INPUT=0,MOUSE_INPUT,JOYSTICK_INPUT};

#endif