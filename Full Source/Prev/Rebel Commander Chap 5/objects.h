#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "bsprite.h"

class CObject
{
private:
	int m_nX, m_nY;
	int m_nXspeed, m_nYspeed;
	int m_nLastXMoveTime;
	CBaseSprite *m_pSprite;
public:
	CObject();
	void draw(LPDIRECTDRAWSURFACE surface);
	void create(int x, int y, int xspeed, int yspeed, CBaseSprite *sprite);
	void accelerate(int xdelta, int ydelta=0);
	void move();
};

#endif