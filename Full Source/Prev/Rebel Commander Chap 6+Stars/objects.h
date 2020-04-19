#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "bsprite.h"

enum ObjectType{SHIP_OBJECT=0,BLIP_OBJECT,NUM_SPRITES};

class CObject
{
private:
	int m_nX, m_nY;
	int m_nXspeed, m_nYspeed;
	int m_nLastXMoveTime;
	int m_nLastYMoveTime;
	CBaseSprite *m_pSprite;
	int m_nMinXSpeed, m_nMaxXSpeed;
	int m_nMinYSpeed, m_nMaxYSpeed;
	int m_nCurrentFrame;
	int m_nFrameCount;
	int m_nLastFrameTime;
	int m_nFrameInterval;
	BOOL m_bForwardAnimation;
public:
	CObject();
	void draw(LPDIRECTDRAWSURFACE surface);
	void create(ObjectType object, int x, int y, int xspeed, int yspeed);
	void accelerate(int xdelta, int ydelta=0);
	void move();
};

#endif