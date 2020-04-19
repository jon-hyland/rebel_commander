//sbmp.h: header file for sprite file reader
//Copyright Ian Parberry, 1999
//Last updated September 28, 1999

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "bmp.h" //for base class
#include "defines.h"

#ifndef __sbmp_h__
#define __sbmp_h__

class CBmpSpriteFileReader:
public CBmpFileReader{ //bmp sprite file input class
  public:
    BOOL draw(LPDIRECTDRAWSURFACE surface,
      int width,int ht,int x,int y); //draw sprite
};

#endif
