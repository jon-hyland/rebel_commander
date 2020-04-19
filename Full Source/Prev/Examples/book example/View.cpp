//view.cpp: viewpoint manager
//Copyright Ian Parberry, 1999
//Last updated October 5, 1999

#include "view.h"
#include "gtimer.h" //game timer

extern CGameTimer Timer; //game timer

CViewPoint::CViewPoint(){ //constructor
  m_nX=0; m_nBgOffset=0; m_nLastTimeBgDrawn=0;
}

void CViewPoint::set_position(int x){ //set current viewpoint
  normalize(x); m_nX=x;
}

int CViewPoint::screen(int x){ 
  //screen coords relative to viewpt
  int delta=x-m_nX;
  if(delta>WORLD_WIDTH/2)delta-=WORLD_WIDTH;
  if(delta<-WORLD_WIDTH/2)delta+=WORLD_WIDTH;
  return SCREEN_WIDTH/2+delta;
}

void CViewPoint::normalize(int &x){ //nomrmalize to world
  while(x<0)x+=WORLD_WIDTH;
  while(x>=WORLD_WIDTH)x-=WORLD_WIDTH;
}

void CViewPoint::draw_background(LPDIRECTDRAWSURFACE lpSource,
  LPDIRECTDRAWSURFACE lpDestination,int speed){
//draw scrolling background from surface lpSource to
//surface lpDestination
  //compute destination rectangle
  RECT rectDest; //destination rectangle
  rectDest.left=SCREEN_WIDTH-m_nBgOffset; 
  rectDest.right=SCREEN_WIDTH;
  rectDest.top=0; 
  rectDest.bottom=SCREEN_HEIGHT; //vertical extent
  //compute source rectangle
  RECT rectSource; //source rectangle
  rectSource.left=0;
  rectSource.right=m_nBgOffset;
  rectSource.top=0; 
  rectSource.bottom=SCREEN_HEIGHT;
  //draw left half of screen
  lpDestination->
    Blt(&rectDest,lpSource,&rectSource,DDBLT_WAIT,NULL);
  //compute destination rectangle
  rectDest.left=0;
  rectDest.right=SCREEN_WIDTH-m_nBgOffset;
  rectDest.top=0; 
  rectDest.bottom=SCREEN_HEIGHT;
  //compute source rectangle
  rectSource.left=m_nBgOffset; 
  rectSource.right=SCREEN_WIDTH; 
  rectSource.top=0; 
  rectSource.bottom=SCREEN_HEIGHT; 
  //draw right half of screen
  lpDestination->
    Blt(&rectDest,lpSource,&rectSource,DDBLT_WAIT,NULL); 
  //compute new offset
  int delta=(speed*(Timer.time()-m_nLastTimeBgDrawn))/50;
  if(delta){ //if nonzero
    m_nBgOffset-=delta; //initial offset
    if(m_nBgOffset>SCREEN_WIDTH) //too positive
      m_nBgOffset=0;
    if(m_nBgOffset<0) //too negative
      m_nBgOffset=SCREEN_WIDTH-1;
    m_nLastTimeBgDrawn=Timer.time(); //record time of move
  }
} //draw_background
