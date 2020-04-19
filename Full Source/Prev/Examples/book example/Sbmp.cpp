//sbmp.cpp: member functions for sprite file reader
//Copyright Ian Parberry, 1999
//Last updated May 22, 2000

#include "sbmp.h"

BOOL CBmpSpriteFileReader::draw(LPDIRECTDRAWSURFACE surface,
                                int width,int ht,int x,int y){
  DDSURFACEDESC ddsd; //direct draw surface descriptor
  BYTE *dest,*src; //destination and source pointers
  //init surface descriptor
  memset(&ddsd,0,sizeof(DDSURFACEDESC));
  ddsd.dwSize=sizeof(ddsd);
  //lock down surface
  if(FAILED(surface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL)))
    return FALSE;
  //calculate addresses and width
  dest=(BYTE*)ddsd.lpSurface; //destination pointer
  src=m_cImage+ //source pointer
    ((m_BMPFileInfo.biHeight-1-y)*m_BMPFileInfo.biWidth+x);
  //move data to surface
  for(int i=0; i<ht; i++){ //for each row
    memcpy(dest,src,width); //copy
    dest+=ddsd.lPitch; src-=m_BMPFileInfo.biWidth; //next row
  }
  //clean up and return
  surface->Unlock(NULL);
  return TRUE;
} //draw

