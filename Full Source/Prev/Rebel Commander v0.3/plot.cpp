#include "plot.h"

void PutPx(int x, int y, BYTE pixel, DDSURFACEDESC *ddsd)
{
	if((x<CONTAINED_SCREEN_LEFT)||(x>CONTAINED_SCREEN_RIGHT)||(y<CONTAINED_SCREEN_TOP)||(y>CONTAINED_SCREEN_BOTTOM))
		return;
	
	x-=SCREEN_OFFSET;
	
	BYTE *pixels=(BYTE *)ddsd->lpSurface;
	DWORD pitch=(ddsd->lPitch>>1)*2;
	pixels[y*pitch+x]=pixel;
}
