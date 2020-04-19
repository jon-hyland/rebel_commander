#include "ship.h"

extern CClippedSprite *g_pSprite[];
extern CTimer Timer;
extern CRandom Random;
extern CViewPoint Viewpoint;

CShip::CShip()
{
	m_nX=m_nY=0;
	m_nVectorX=m_nVectorY=0;
	m_nVectorSpeed=5;
	m_nDirection=0;
	m_nLastXMoveTime=m_nLastYMoveTime=0;
	m_nWidth=m_nHeight=0;
	m_bDying=m_bDead=FALSE;
	m_pSprite=NULL;
	m_nCurrentFrame=m_nFrameCount=m_nLastFrameTime=0;
	m_nFrameInterval=33;
	m_nTurnCurrentFrame=m_nTurnFrameCount=m_nTurnLastFrameTime=0;
	m_nTurnFrameInterval=250;
	m_nDyingCurrentFrame=m_nDyingFrameCount=m_nDyingLastFrameTime=0;
	m_nDyingFrameInterval=0;
}

void CShip::Create(int x, int y)
{
	m_nX=x;	m_nY=y;
	m_nVectorX=m_nVectorY=0;
	m_nLastXMoveTime=m_nLastYMoveTime=Timer.time();
	m_nWidth=g_pSprite[SHIP_SPRITE]->GetWidth();
	m_nHeight=g_pSprite[SHIP_SPRITE]->GetHeight();
	m_pSprite=g_pSprite[SHIP_SPRITE];
	m_pSpriteUp=g_pSprite[SHIP_UP_SPRITE];
	m_pSpriteDown=g_pSprite[SHIP_DOWN_SPRITE];
	m_nTurnCurrentFrame=0;
	m_nTurnFrameCount=m_pSpriteUp->frame_count();
	m_nFrameCount=m_pSprite->frame_count();
	m_nCurrentFrame=0;
}

void CShip::SetDirection(int direction)
{
	m_nDirection=direction;
}

void CShip::Draw(LPDIRECTDRAWSURFACE surface)
{
	int t;
	static ShipState enShipState;  //SHIP_NORMAL, SHIP_UP, SHIP_DOWN, SHIP_DYING

	if(enShipState==SHIP_NORMAL)
	{
		if(m_nDirection==8) enShipState=SHIP_UP;
		else if(m_nDirection==2) enShipState=SHIP_DOWN;
		m_bTurnAnimateBack=FALSE;
		m_nTurnLastFrameTime=Timer.time();
		m_nTurnCurrentFrame=0;
	}
	if((enShipState==SHIP_UP)&&(m_nDirection!=8))
		m_bTurnAnimateBack=TRUE;
	if((enShipState==SHIP_DOWN)&&(m_nDirection!=2))
		m_bTurnAnimateBack=TRUE;
	

	switch(enShipState)
	{
		case SHIP_NORMAL:
			m_pSprite->draw(m_nCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
			t=m_nFrameInterval;
			if(m_nFrameCount>1&&Timer.elapsed(m_nLastFrameTime,t))
				if(++m_nCurrentFrame>=m_nFrameCount)
				{	
					m_nCurrentFrame=0;
				}
			break;
		case SHIP_UP:
			m_pSpriteUp->draw(m_nTurnCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
			t=int(m_nTurnFrameInterval/(1+abs(int(m_nVectorSpeed/6))));
			if(m_nTurnFrameCount>1&&Timer.elapsed(m_nTurnLastFrameTime,t))
				if(!m_bTurnAnimateBack)
				{
					if(++m_nTurnCurrentFrame>=m_nTurnFrameCount)
					{
						m_nTurnCurrentFrame=m_nTurnFrameCount-1;
					}
				}
				else
				{
					if(--m_nTurnCurrentFrame<0)
					{
						m_nTurnCurrentFrame=0;
						enShipState=SHIP_NORMAL;
					}
				}
			break;
		case SHIP_DOWN:
			m_pSpriteDown->draw(m_nTurnCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
			t=int(m_nTurnFrameInterval/(1+abs(int(m_nVectorSpeed/6))));
			if(m_nTurnFrameCount>1&&Timer.elapsed(m_nTurnLastFrameTime,t))
				if(!m_bTurnAnimateBack)
				{
					if(++m_nTurnCurrentFrame>=m_nTurnFrameCount)
					{
						m_nTurnCurrentFrame=m_nTurnFrameCount-1;
					}
				}
				else
				{
					if(--m_nTurnCurrentFrame<0)
					{
						m_nTurnCurrentFrame=0;
						enShipState=SHIP_NORMAL;
					}
				}
			break;
	}
}

void CShip::Move()
{
	const int XSCALE=16;
	const int YSCALE=16;
	
	int xdelta, ydelta;
	int time=Timer.time();
	int tfactor;
	
	switch(m_nDirection)
	{
		case 8:	//up
			m_nVectorX=0;
			m_nVectorY=-m_nVectorSpeed;
			break;
		case 2:	//down
			m_nVectorX=0;
			m_nVectorY=m_nVectorSpeed;
			break;
		case 4:	//left
			m_nVectorX=-m_nVectorSpeed;
			m_nVectorY=0;
			break;
		case 6:	//right
			m_nVectorX=m_nVectorSpeed;
			m_nVectorY=0;
			break;
		case 0: case 5:	//stop
			m_nVectorX=0;
			m_nVectorY=0;
			break;
	}

	tfactor=time-m_nLastXMoveTime;
	xdelta=int((m_nVectorX*tfactor)/XSCALE);
	m_nX+=xdelta;
	if(xdelta||m_nVectorX==0)
		m_nLastXMoveTime=time;

	tfactor=time-m_nLastYMoveTime;
	ydelta=int((m_nVectorY*tfactor)/YSCALE);
	m_nY+=ydelta;
	if(ydelta||m_nVectorY==0)
		m_nLastYMoveTime=time;


	if(m_nX<CONTAINED_SCREEN_LEFT)
	{	
		m_nX=CONTAINED_SCREEN_LEFT;	
		m_nVectorX=0;
	}
	if(m_nX>CONTAINED_SCREEN_RIGHT)
	{	
		m_nX=CONTAINED_SCREEN_RIGHT;
		m_nVectorX=0;
	}
	if(m_nY<0)
	{	
		m_nY=0;	
		m_nVectorY=0;
	}
	if(m_nY>WORLD_HEIGHT-1)
	{	
		m_nY=WORLD_HEIGHT-1;
		m_nVectorY=0;
	}

}

int CShip::GetX() { return m_nX; }
int CShip::GetY() { return m_nY; }
float CShip::GetVectorX() { return m_nVectorX; }
float CShip::GetVectorY() { return m_nVectorY; }
int CShip::GetDirection() { return m_nDirection; }
BOOL CShip::Dying() { return m_bDying; }
BOOL CShip::Dead() { return m_bDead; }
