#include "csprite.h"

CClippedSprite::CClippedSprite(int frames, int w, int h):
CBaseSprite(frames,w,h)
{
	m_rectSource.left=0;
	m_rectSource.right=w;
	m_rectSource.top=0;
	m_rectSource.bottom=h;
}

void CClippedSprite::draw(int frame, int x, int y, LPDIRECTDRAWSURFACE dest)
{
	RECT rectDest;

	rectDest.left=x-m_nWidth/2;
	rectDest.right=rectDest.left+m_nWidth;
	rectDest.top=y-m_nHeight/2;
	rectDest.bottom=rectDest.top+m_nHeight;

	dest->Blt(&rectDest, m_lpDDImage[frame], &m_rectSource, DDBLT_KEYSRC|DDBLT_WAIT,NULL);
}