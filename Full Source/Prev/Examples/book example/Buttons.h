//buttons.h: header file for button class

//Copyright Ian Parberry, 1999
//Last updated March 15, 2000

#ifndef __BUTTONS__
#define __BUTTONS__

#include <windows.h>
#include <ddraw.h>
#include "csprite.h"

class CButtonManager{ //button manager class
  private:
    RECT *m_rectHotspot; //hotspot locations
    BOOL *m_bDown; //is button held down?
    int m_nMaxCount; //number of hotspots allowed
    int m_nCount; //number of current hotspots
    SIZE m_sizeButton; //size of buttons, if all the same size
    CBaseSprite* m_spriteDefault; //default button sprite
    CBmpSpriteFileReader m_cImage; //button images
    int m_nButtonDownSound; //sound index for button down
    int m_nButtonUpSound; //sound index for button up
    BOOL m_bRadio; //TRUE for radio buttons
    BOOL PointInRect(RECT rect,POINT point); //is point in rect?
    BOOL addbutton(RECT rect); //add button to list
    BOOL loadsprite(char *filename); //load button sprite
    void loadimages(); //load button images
    void display_menu(); //display menu page
  public:
    CButtonManager(int count,SIZE size,char *filename);
    CButtonManager(int count,SIZE size,POINT first,
      int ydelta,char *filename); //constructor
    ~CButtonManager(); //destructor
    BOOL addbutton(POINT point); //add button of default size
    int hit(POINT point); //return hit, if any
    int release(POINT point); //return hit on down button
    BOOL buttondown(POINT point); //animate button down
    BOOL buttondown(int index,BOOL sound=TRUE); //animate
    BOOL buttonup(POINT point); //animate button up
    BOOL buttonup(int index); //animate button up
    void allbuttonsup(); //animate all buttons up
    void setsounds(int down,int up); //set sounds
    void set_radio(BOOL on=TRUE); //set to radio buttons
    BOOL Restore(); //restore surfaces
    void Release(); //release surfaces
};
#endif
