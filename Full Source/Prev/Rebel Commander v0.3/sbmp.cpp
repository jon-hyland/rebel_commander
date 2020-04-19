#include "sbmp.h"

BOOL CBmpSpriteFileReader::draw(LPDIRECTDRAWSURFACE surface, int width, int ht, int x, int y)
{
	DDSURFACEDESC ddsd;
	BYTE *dest, *src;

	memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	ddsd.dwSize=sizeof(ddsd);

	if(FAILED(surface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))
		return FALSE;

	dest=(BYTE*)ddsd.lpSurface;
	src=m_cImage+((m_BMPFileInfo.biHeight-1-y)*m_BMPFileInfo.biWidth+x);

	for(int i=0; i<ht; i++)
	{
		memcpy(dest, src, width);
		dest+=ddsd.lPitch;
		src-=m_BMPFileInfo.biWidth;
	}

	surface->Unlock(NULL);

	return TRUE;
}