#include "plot.h"

void PutPx(int x, int y, BYTE pixel, DDSURFACEDESC *ddsd)
{
	BYTE *pixels=(BYTE *)ddsd->lpSurface;
	DWORD pitch=(ddsd->lPitch>>1)*2;
	pixels[y*pitch+x]=pixel;
}
