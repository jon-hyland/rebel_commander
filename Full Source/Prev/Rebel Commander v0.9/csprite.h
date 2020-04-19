#ifndef __CSPRITE_H__
#define __CSPRITE_H__

#include "bsprite.h"

class CClippedSprite: public CBaseSprite
{
	private:
		int m_nWidth;	
		int m_nHeight;
		RECT m_rectSource;
	public:
		CClippedSprite(int frames, int w, int h);
		virtual void draw(int frame, int x, int y, LPDIRECTDRAWSURFACE destination);
		int GetWidth();
		int GetHeight();
};

#endif
