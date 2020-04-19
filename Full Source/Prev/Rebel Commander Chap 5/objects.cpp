#include "objects.h"
#include "timer.h"

extern CTimer Timer;

CObject::CObject()
{
	m_nX=m_nY=m_nXspeed=m_nYspeed=0;
	m_pSprite=NULL;
	m_nLastXMoveTime=0;
}


void CObject::draw(LPDIRECTDRAWSURFACE surface)
{
	m_pSprite->draw(0, m_nX, m_nY, surface);
}

void CObject::create(int x, int y, int xspeed, int yspeed, CBaseSprite *sprite)
{
	m_nLastXMoveTime=0;
	m_nX=x;
	m_nY=y;
	m_nXspeed=xspeed;
	m_nYspeed=yspeed;
	m_pSprite=sprite;
}

void CObject::accelerate(int xdelta, int ydelta)
{
	m_nXspeed+=xdelta;
	m_nYspeed+=ydelta;
}

void CObject::move()
{
	const int XSCALE=8;
	const int MARGIN=100;
	int xdelta;
	int time=Timer.time();
	int tfactor=time-m_nLastXMoveTime;
	xdelta=(m_nXspeed*tfactor)/XSCALE;
	m_nX+=xdelta;
	if(m_nX<-MARGIN)m_nX=SCREEN_WIDTH+MARGIN;
	if(xdelta||m_nXspeed==0)
		m_nLastXMoveTime=time;
	if(m_nX>SCREEN_WIDTH+MARGIN)m_nX=-MARGIN;
}

