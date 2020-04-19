#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "sbmp.h"

#ifndef __BSPRITE_H__
#define __BSPRITE_H__

class CBaseSprite
{
protected:
	LPDIRECTDRAWSURFACE *m_lpDDImage;
	int m_nFrameCount;
	int m_nWidth, m_nHeight;
	void CreateSurfaces();
public:
	CBaseSprite(int frames, int w, int h);
	~CBaseSprite();
	BOOL load(CBmpSpriteFileReader *image, int frame, int x, int y);	// SPECIFY X,Y POS IN FILE (TOP LEFT)
	virtual void draw(int frame, int x, int y, LPDIRECTDRAWSURFACE destination);
	void Release();
	BOOL Restore();
	int frame_count();
	int height();
	int width();
};

#endif