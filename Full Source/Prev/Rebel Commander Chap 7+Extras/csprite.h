#include "bsprite.h"

#ifndef __CSPRITE_H__
#define __CSPRITE_H__

class CClippedSprite: public CBaseSprite
{
	private:
		RECT m_rectSource;
	public:
		CClippedSprite(int frames, int w, int h);
		virtual void draw(int frame, int x, int y, LPDIRECTDRAWSURFACE destination);
};

#endif
