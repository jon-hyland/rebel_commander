//text.h: header file for text.cpp

//Copyright Ian Parberry, 9
//Last updated December 7, 1999

#ifndef __TEXT__
#define __TEXT__

#include "csprite.h" //for text sprites

enum chartype{CHAR_A=0,CHAR_B,CHAR_C,CHAR_D,CHAR_E,CHAR_F,
  CHAR_G,CHAR_H,CHAR_I,CHAR_J,CHAR_K,CHAR_L,CHAR_M,CHAR_N,
  CHAR_O,CHAR_P,CHAR_Q,CHAR_R,CHAR_S,CHAR_T,CHAR_U,CHAR_V,
  CHAR_W,CHAR_X,CHAR_Y,CHAR_Z,CHAR_0,CHAR_1,CHAR_2,CHAR_3,
  CHAR_4,CHAR_5,CHAR_6,CHAR_7,CHAR_8,CHAR_9,NUM_CHARS};


class CTextManager{ //fixed width font text sprite manager
  private:
    CClippedSprite *character[NUM_CHARS]; //char sprites
    int width,height; //width and height of chars
    int x,y; //current cursor location
    void draw(chartype c,LPDIRECTDRAWSURFACE surface);
  public:
    CTextManager(int wdth,int ht); //constructor
    ~CTextManager(); //destructor
    BOOL load(CBmpSpriteFileReader *file,
      int x,int y); //load sprites from file at (x,y)
    void jump(int xloc,int yloc); //move cursor to here
    void draw(char* string,
      LPDIRECTDRAWSURFACE surface); //draw string
    BOOL Restore(); //restore sprite surfaces
    void Release(); //release sprite surfaces
};

#endif
