//text.cpp: code for text manager

//Copyright Ian Parberry, 1999
//Last updated December 7, 1999

#include <string.h>

#include "text.h"
#include "bmp.h"

CTextManager::CTextManager(int wdth,int ht){ //constructor
  width=wdth; height=ht; x=y=0;
  for(int i=0; i<NUM_CHARS; i++)
    character[i]=new CClippedSprite(1,width,height);
}

CTextManager::~CTextManager(){ //destructor
  for(int i=0; i<NUM_CHARS; i++)delete character[i];
}

BOOL CTextManager::load(CBmpSpriteFileReader *bmp,
                        int x,int y){ //load sprites
  BOOL result=TRUE;
  int cx=x,cy=y; //location to read from
  //grab numbers, assume they are first
  for(int c=CHAR_0; c<=CHAR_9; c++){
    result=result&&character[c]->load(bmp,0,cx,cy);
    cx+=width+1;
  }
  //grab letters
  cx=x; cy=y+height+1; //assume letters below numbers
  for(c=CHAR_A; c<=CHAR_Z; c++){
    result=result&&character[c]->load(bmp,0,cx,cy);
    cx+=width+1;
  }
  return result;
}

void CTextManager::jump(int xloc,int yloc){ //jump to here
  x=xloc; y=yloc;
}

void CTextManager::draw(chartype c,
    LPDIRECTDRAWSURFACE surface){ //draw char to surface
  int cx=x+width/2,cy=y; //compute location
  if(c>=0&&c<NUM_CHARS)
    character[c]->draw(0,cx,cy,surface); //draw
  x+=width; //advance to next char
}

void CTextManager::draw(char* string,
    LPDIRECTDRAWSURFACE surface){ //draw string to surface
  for(int i=0; i<(int)strlen(string); i++){ //for each char
    char c=string[i]; //grab character
    if(c>='0'&&c<='9') //number
      draw((chartype)(CHAR_0+c-'0'),surface);
    else if(c>='A'&&c<='Z') //upper case
      draw((chartype)(CHAR_A+c-'A'),surface);
    else if(c>='a'&&c<='z') //lower case
      draw((chartype)(CHAR_A+c-'a'),surface);
    else if(c==' ')x+=width; //space
  }
}

BOOL CTextManager::Restore(){ //restore sprite surfaces
  BOOL result=TRUE;
  for(int c=0; c<NUM_CHARS; c++) //for each char sprite
    result=result&&character[c]->Restore(); //restore it
  return result;
}

void CTextManager::Release(){ //release sprite surfaces
  for(int c=0; c<NUM_CHARS; c++) //for each char sprite
    character[c]->Release(); //release it
}
