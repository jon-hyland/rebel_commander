#include "powerup.h"

extern GameState;
extern CClippedSprite *g_pSprite[];
extern CTimer Timer;
extern CRandom Random;
extern CViewPoint Viewpoint;
extern CObjectManager ObjectManager;
extern CSoundManager *SoundManager;
extern double PlayerScore;

CPowerUp::CPowerUp()
{
}

void CPowerUp::Create(UpType up_type, int x, int y)
{
	SpriteType sprite_type;

	if(up_type==WEAPON_UP) 
		sprite_type=WEAPON_UP_SPRITE;
	else if(up_type==SPEED_UP)
		sprite_type=SPEED_UP_SPRITE;
	else if(up_type==LIFE_UP)
		sprite_type=LIFE_UP_SPRITE;

	m_enUpType=up_type;
	m_nX=x;	m_nY=y;
	m_bDead=FALSE;
	m_nColliWidth=30; m_nColliHeight=26;
	m_nWidth=g_pSprite[sprite_type]->GetWidth();
	m_nHeight=g_pSprite[sprite_type]->GetHeight();
	m_pSprite=g_pSprite[sprite_type];
	m_nFrameInterval=99;
	m_nLastFrameTime=Timer.GameTime();
	m_nFrameCount=m_pSprite->frame_count();
	m_nCurrentFrame=0;
}


void CPowerUp::Kill()
{
	m_bDead=TRUE;
	PlayerScore+=1000;
	//destroy in self?
}

void CPowerUp::Draw(LPDIRECTDRAWSURFACE surface)
{
	int t;

	//if up is still alive
	if(!m_bDead)
	{

		//draw
		m_pSprite->draw(m_nCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
		if(GameState!=0)
		{
			//perform animation
			t=m_nFrameInterval;
			if(m_nFrameCount>1&&Timer.Elapsed(m_nLastFrameTime,t))
				if(++m_nCurrentFrame>=m_nFrameCount)
				{	
					m_nCurrentFrame=0;
				}
		}
		
		
	}
}


int CPowerUp::GetX() { return m_nX; }
int CPowerUp::GetY() { return m_nY; }
int CPowerUp::GetColliWidth() { return m_nColliWidth; }
int CPowerUp::GetColliHeight() { return m_nColliHeight; }
BOOL CPowerUp::Dead() { return m_bDead; }
UpType CPowerUp::GetUpType() { return m_enUpType; }
