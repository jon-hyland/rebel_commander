#include "plot.h"

void PutPx(int x, int y, BYTE pixel, DDSURFACEDESC *ddsd)
{
	//make sure coods are (220-859) and (0-479)
	if((x<CONTAINED_SCREEN_LEFT)||(x>CONTAINED_SCREEN_RIGHT)||(y<CONTAINED_SCREEN_TOP)||(y>CONTAINED_SCREEN_BOTTOM))
		return;
	
	//adjust for (220) viewpoint
	x-=SCREEN_OFFSET;
	
	//pixels becomes pointer to surface data
	BYTE *pixels=(BYTE *)ddsd->lpSurface;
	//determine y multiplier
	DWORD pitch=(ddsd->lPitch>>1)*2;
	//set pixel
	pixels[y*pitch+x]=pixel;
}

BYTE GetPx(int x, int y, DDSURFACEDESC *ddsd)
{
	BYTE pixel;
	
	//make sure coods are (220-859) and (0-479)
	if((x<CONTAINED_SCREEN_LEFT)||(x>CONTAINED_SCREEN_RIGHT)||(y<CONTAINED_SCREEN_TOP)||(y>CONTAINED_SCREEN_BOTTOM))
		return NULL;

	//adjust for (220) viewpoint
	x-=SCREEN_OFFSET;
	
	//pixels becomes pointer to surface data
	BYTE *pixels=(BYTE *)ddsd->lpSurface;
	//determine y multiplier
	DWORD pitch=(ddsd->lPitch>>1)*2;
	//retrieve pixel
	pixel=pixels[y*pitch+x];

	return pixel;
}