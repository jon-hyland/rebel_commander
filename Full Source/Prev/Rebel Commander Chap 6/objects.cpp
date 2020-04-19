#include "objects.h"
#include "timer.h"
#include "csprite.h"

extern CClippedSprite *g_pSprite[];  //sprites
extern CTimer Timer;

CObject::CObject()
{
	m_nX=m_nY=m_nXspeed=m_nYspeed=0;
	m_pSprite=NULL;
	m_nLastXMoveTime=m_nLastYMoveTime=0;
	m_nCurrentFrame=m_nFrameCount=m_nLastFrameTime=0;
	m_bForwardAnimation=TRUE;
	m_nFrameInterval=30;
}


void CObject::draw(LPDIRECTDRAWSURFACE surface)
{
	m_pSprite->draw(m_nCurrentFrame, m_nX, m_nY, surface);

	int t=m_nFrameInterval/(1+abs(m_nXspeed));
	if(m_nFrameCount>1&&Timer.elapsed(m_nLastFrameTime,t))
		if(m_bForwardAnimation)
		{
			if(++m_nCurrentFrame>=m_nFrameCount)
			{
				m_nCurrentFrame=m_nFrameCount-2;
				m_bForwardAnimation=FALSE;
			}
		}
		else
		{
			if(--m_nCurrentFrame<0)
			{
				m_nCurrentFrame=1;
				m_bForwardAnimation=TRUE;
			}
		}
}

void CObject::create(ObjectType object, int x, int y, int xspeed, int yspeed)
{
	m_nLastXMoveTime=m_nLastYMoveTime=Timer.time();
	m_nX=x;
	m_nY=y;
	m_nXspeed=xspeed;
	m_nYspeed=yspeed;
	m_pSprite=g_pSprite[object];
	m_nFrameCount=m_pSprite->frame_count();

	switch(object)
	{
		case SHIP_OBJECT: 
			m_nMinXSpeed=-3; m_nMaxXSpeed=3;
			m_nMinYSpeed=-4; m_nMaxYSpeed=4;
			break;
		case BLIP_OBJECT:
			m_nMinXSpeed=-2; m_nMaxXSpeed=-1;
			m_nMinYSpeed=-1; m_nMaxYSpeed=1;
			m_nFrameInterval=250;
			break;
	}
}

void CObject::accelerate(int xdelta, int ydelta)
{
	m_nXspeed+=xdelta;
	if(m_nXspeed<m_nMinXSpeed)m_nXspeed=m_nMinXSpeed;
	if(m_nXspeed>m_nMaxXSpeed)m_nXspeed=m_nMaxXSpeed;
	m_nYspeed+=ydelta;
	if(m_nYspeed<m_nMinYSpeed)m_nYspeed=m_nMinYSpeed;
	if(m_nYspeed>m_nMaxYSpeed)m_nYspeed=m_nMaxYSpeed;
}

void CObject::move()
{
	const int XSCALE=16;
	const int YSCALE=32;
	
	const int MARGIN=100;
	int xdelta, ydelta;
	int time=Timer.time();
	int tfactor=time-m_nLastXMoveTime;

	xdelta=(m_nXspeed*tfactor)/XSCALE;
	m_nX+=xdelta;
	if(m_nX<-MARGIN)m_nX=SCREEN_WIDTH+MARGIN;
	if(m_nX>SCREEN_WIDTH+MARGIN)m_nX=-MARGIN; 

	if(xdelta||m_nXspeed==0)
		m_nLastXMoveTime=time;

	tfactor=time-m_nLastYMoveTime;
	ydelta=(m_nYspeed*tfactor)/YSCALE;
	m_nY+=ydelta;
	if(m_nY<-MARGIN)m_nY=-MARGIN;
	if(m_nY>SCREEN_HEIGHT+MARGIN)
		m_nY=SCREEN_HEIGHT+MARGIN;
	if(ydelta||m_nYspeed==0)
		m_nLastYMoveTime=time;
}

