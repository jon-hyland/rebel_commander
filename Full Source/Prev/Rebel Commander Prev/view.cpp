#include "view.h"
#include "timer.h"

extern CTimer Timer;

CViewPoint::CViewPoint()
{
	m_nX=0;
	m_nBgOffset=0;
	m_nLastTimeBgDrawn=0;
}

void CViewPoint::set_position(int x)
{
	normalize(x);
	m_nX=x;
}

int CViewPoint::screen(int x)
{
	int delta=x-m_nX;
	if(delta>WORLD_WIDTH/2)delta-=WORLD_WIDTH;
	if(delta<-WORLD_WIDTH/2)delta+=WORLD_WIDTH;
	return SCREEN_WIDTH/2+delta;
}

void CViewPoint::normalize(int &x)
{
	while(x<0)x+=WORLD_WIDTH;
	while(x>=WORLD_WIDTH)x-=WORLD_WIDTH;
}

void CViewPoint::draw_background(LPDIRECTDRAWSURFACE lpSource, LPDIRECTDRAWSURFACE lpDestination, int speed)
{
	RECT rectDest;
	rectDest.left=SCREEN_WIDTH-m_nBgOffset;
	rectDest.right=SCREEN_WIDTH;
	rectDest.top=0;
	rectDest.bottom=SCREEN_HEIGHT;

	RECT rectSource;
	rectSource.left=0;
	rectSource.right=m_nBgOffset;
	rectSource.top=0;
	rectSource.bottom=SCREEN_HEIGHT;

	lpDestination->Blt(&rectDest,lpSource,&rectSource,DDBLT_KEYSRC|DDBLT_WAIT,NULL);

	rectDest.left=0;
	rectDest.right=SCREEN_WIDTH-m_nBgOffset;
	rectDest.top=0;
	rectDest.bottom=SCREEN_HEIGHT;

	rectSource.left=m_nBgOffset;
	rectSource.right=SCREEN_WIDTH;
	rectSource.top=0;
	rectSource.bottom=SCREEN_HEIGHT;

	lpDestination->Blt(&rectDest,lpSource,&rectSource,DDBLT_KEYSRC|DDBLT_WAIT,NULL);

	int delta=(speed*(Timer.time()-m_nLastTimeBgDrawn))/50;
	if(delta)
	{
		m_nBgOffset-=delta;
		if(m_nBgOffset>SCREEN_WIDTH)
			m_nBgOffset=0;
		if(m_nBgOffset<0)
			m_nBgOffset=SCREEN_WIDTH-1;
		m_nLastTimeBgDrawn=Timer.time();
	}
}
