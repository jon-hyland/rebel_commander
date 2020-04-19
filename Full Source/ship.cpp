#include "ship.h"

extern GameState;
extern CClippedSprite *g_pSprite[];
extern CTimer Timer;
extern CRandom Random;
extern CViewPoint Viewpoint;
extern CObjectManager ObjectManager;
extern CPowerUpManager UpManager;
extern CSoundManager *SoundManager;
extern int Lives;
extern BOOL Invincible;
extern BOOL MenuActive;
extern BOOL GameOver;
extern DWORD GameOverTime;

CShip::CShip()
{
}

void CShip::Create(int x, int y)
{
	DWORD time=Timer.GameTime();

	SoundManager->Play(ENGINE_LOOP_SOUND, TRUE);
	m_nX=x;	m_nY=y;
	m_nBottomBoundary=400;
	m_nVectorX=m_nVectorY=0;
	m_nVectorSpeed=SHIP_SPEED_BASE;
	m_nDirection=0;
	m_nSpeedUpCount=0;
	m_nWeaponCount=0;
	m_nShipKills=0;
	m_enFireType=SHIP_LASER_ONE;
	m_bDying=m_bDead=FALSE;
	m_nLastXMoveTime=m_nLastYMoveTime=Timer.GameTime();
	m_nColliWidth=36; m_nColliHeight=16;
	m_nWidth=g_pSprite[SHIP_SPRITE]->GetWidth();
	m_nHeight=g_pSprite[SHIP_SPRITE]->GetHeight();
	m_pSprite=g_pSprite[SHIP_SPRITE];
	m_pUpSprite=g_pSprite[SHIP_UP_SPRITE];
	m_pDownSprite=g_pSprite[SHIP_DOWN_SPRITE];
	m_pDyingSprite=g_pSprite[SHIP_DYING_SPRITE];
	m_nFrameInterval=33;
	m_nTurnFrameInterval=250;
	m_nDyingFrameInterval=270;
	m_nLastFrameTime=Timer.GameTime();
	m_nDyingFrameCount=m_pDyingSprite->frame_count();
	m_nDyingCurrentFrame=0;
	m_nTurnCurrentFrame=0;
	m_nTurnFrameCount=m_pUpSprite->frame_count();
	m_nFrameCount=m_pSprite->frame_count();
	m_nCurrentFrame=0;
	m_nInvincibilityTimeExpires=time+SHIP_INVINCIBILITY_TIME_ON_CREATE;
	m_bFlicker=TRUE;
}

void CShip::SetDirection(int direction)
{
	m_nDirection=direction;
}

void CShip::SetBottomBoundary(int y)
{
	m_nBottomBoundary=y;
}

void CShip::IncreaseKills()
{
	m_nShipKills++;
}

void CShip::Kill()
{
	DWORD time=Timer.GameTime();

	if((!Invincible)&&(time>m_nInvincibilityTimeExpires))
	{
		m_bDying=TRUE;
		m_nDyingLastFrameTime=Timer.GameTime();
		SoundManager->Stop(ENGINE_LOOP_SOUND);
		SoundManager->Play(SHIP_DIE_SOUND);
	}
}

void CShip::UpSpeed()
{
	SoundManager->Play(UPGRADE_SOUND);
	m_nSpeedUpCount++;
	m_nSpeedTimeStart=Timer.GameTime();
	m_nVectorSpeed=float(SHIP_SPEED_BASE+(m_nSpeedUpCount*SHIP_SPEED_MULTIPLE));
}

void CShip::UpWeapon()
{
	SoundManager->Play(UPGRADE_SOUND);
	m_nWeaponCount++;
	switch(m_nWeaponCount)
	{
		case 0: m_enFireType=SHIP_LASER_ONE; break;
	}
}

void CShip::Draw(LPDIRECTDRAWSURFACE surface)
{
	int t;
	static ShipState enShipState;  //SHIP_NORMAL, SHIP_UP, SHIP_DOWN, SHIP_DYING
	DWORD time=Timer.GameTime();

	//if ship is still avive
	if(!m_bDead&&!m_bDying)
	{
		//calculate proper ship state based on direction
		if(enShipState==SHIP_NORMAL)
		{
			switch(m_nDirection)
			{
				case 7: case 8: case 9:
					enShipState=SHIP_UP;
					break;
				case 1: case 2: case 3:
					enShipState=SHIP_DOWN;
					break;
			}

			m_bTurnAnimateBack=FALSE;
			m_nTurnLastFrameTime=Timer.GameTime();
			m_nTurnCurrentFrame=0;
		}
		if((enShipState==SHIP_UP)&&(m_nDirection!=7)&&(m_nDirection!=8)&&(m_nDirection!=9))
			m_bTurnAnimateBack=TRUE;
		if((enShipState==SHIP_DOWN)&&(m_nDirection!=1)&&(m_nDirection!=2)&&(m_nDirection!=3))
			m_bTurnAnimateBack=TRUE;

		//flicker if invincible
		if(((time<m_nInvincibilityTimeExpires)||(Invincible))&&(!MenuActive))
		{
			//will flicker every other frame
			m_bFlicker=!m_bFlicker;
			if(m_bFlicker)
				return; //don't draw anything

		}

		//draw based on ship state
		switch(enShipState)
		{
			case SHIP_NORMAL:
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
				break;
			case SHIP_UP:
				m_pUpSprite->draw(m_nTurnCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
				if(GameState!=0)
				{
					//perform animation
					t=int(m_nTurnFrameInterval/(1+abs(int(m_nVectorSpeed/6))));
					if(m_nTurnFrameCount>1&&Timer.Elapsed(m_nTurnLastFrameTime,t))
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
				}
				break;
			case SHIP_DOWN:
				m_pDownSprite->draw(m_nTurnCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
				if(GameState!=0)
				{
					//perform animation
					t=int(m_nTurnFrameInterval/(1+abs(int(m_nVectorSpeed/6))));
					if(m_nTurnFrameCount>1&&Timer.Elapsed(m_nTurnLastFrameTime,t))
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
				}
				break;
		}
	}
	else if((m_bDying)&&(!m_bDead))
	{
		m_pDyingSprite->draw(m_nDyingCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
		if(GameState!=0)
		{
			//perform animation
			int t=m_nDyingFrameInterval;
			if(m_nDyingFrameCount>1&&Timer.Elapsed(m_nDyingLastFrameTime,t))
				if(++m_nDyingCurrentFrame>=m_nDyingFrameCount) 
				{
					m_nDyingCurrentFrame--;
					m_bDead=TRUE;
					
					//create new ship or game over
					if(--Lives<0)
					{
						Lives=0;
						GameOver=TRUE;
						GameOverTime=time;
					}
					else
						Create(300, 230);
				}
		}
	}
}

void CShip::Move()
{
	const int XSCALE=16;
	const int YSCALE=16;
	
	int xdelta, ydelta;
	int time=Timer.GameTime();
	int tfactor;

	//if dead or dying, don't move
	if(m_bDying||m_bDead)
		return;

	//check if speed up ran out
	if((m_nSpeedUpCount>0)&&(Timer.GameTime()>m_nSpeedTimeStart+SPEED_UP_TIME))
	{
		m_nSpeedTimeStart=Timer.GameTime();
		m_nSpeedUpCount--;
		m_nVectorSpeed=float(SHIP_SPEED_BASE+(m_nSpeedUpCount*SHIP_SPEED_MULTIPLE));	
	}
	
	//set ship vector based on direction
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
		case 7:	//up-left
			m_nVectorX=-m_nVectorSpeed;
			m_nVectorY=-m_nVectorSpeed;
			break;
		case 9:	//up-right
			m_nVectorX=m_nVectorSpeed;
			m_nVectorY=-m_nVectorSpeed;
			break;
		case 1:	//down-left
			m_nVectorX=-m_nVectorSpeed;
			m_nVectorY=m_nVectorSpeed;
			break;
		case 3:	//down-right
			m_nVectorX=m_nVectorSpeed;
			m_nVectorY=m_nVectorSpeed;
			break;
		case 0: case 5:	//stop
			m_nVectorX=0;
			m_nVectorY=0;
			break;
	}

	//move ship relative to time and vector
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

	//constrain x,y coods to contained area
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

	//check for collisions with enemy objects
	for(int i=0; i<MAX_OBJECT_INSTANCES; i++)
		//check if object exists and is still alive
		if((ObjectManager.ObjList[i]!=NULL)&&(!ObjectManager.ObjList[i]->Dead())&&(!ObjectManager.ObjList[i]->Dying()))
		{
			//if collision is detected (using collision dimentions)
			if(abs(m_nX-ObjectManager.ObjList[i]->GetX())<(m_nColliWidth/2)+(ObjectManager.ObjList[i]->GetColliWidth()/2)
				&&
				abs(m_nY-ObjectManager.ObjList[i]->GetY())<(m_nColliHeight/2)+(ObjectManager.ObjList[i]->GetColliWidth()/2))
			{
				//kill the ship object
				Kill();
			}
		}

	//check for collisions with powerup objects
	for(i=0; i<MAX_UP_INSTANCES; i++)
		//check if up exists and is still alive
		if((UpManager.UpList[i]!=NULL)&&(!UpManager.UpList[i]->Dead()))
		{
			//if collision is detected (using collision dimentions)
			if(abs(m_nX-UpManager.UpList[i]->GetX())<(m_nColliWidth/2)+(UpManager.UpList[i]->GetColliWidth()/2)
				&&
				abs(m_nY-UpManager.UpList[i]->GetY())<(m_nColliHeight/2)+(UpManager.UpList[i]->GetColliWidth()/2))
			{
				//kill the powerup object
				UpManager.UpList[i]->Kill();
				//perform a ship upgrade
				switch(UpManager.UpList[i]->GetUpType())
				{
					case SPEED_UP:
						UpSpeed();
						break;
					case WEAPON_UP:
						break;
					case LIFE_UP:
						Lives++;
						//MAKE LIFE_UP SOUND!
						break;
				}
			}
		}

	//check if ship passed bottom bounday
	if(m_nY>m_nBottomBoundary)
		Kill();

}

int CShip::GetX() { return m_nX; }
int CShip::GetY() { return m_nY; }
float CShip::GetVectorX() { return m_nVectorX; }
float CShip::GetVectorY() { return m_nVectorY; }
int CShip::GetColliWidth() { return m_nColliWidth; }
int CShip::GetColliHeight() { return m_nColliHeight; }
int CShip::GetDirection() { return m_nDirection; }
int CShip::GetBottomBoundary() { return m_nBottomBoundary; }
BOOL CShip::Dying() { return m_bDying; }
BOOL CShip::Dead() { return m_bDead; }
int CShip::GetSpeedUpCount() { return m_nSpeedUpCount; }
FireType CShip::GetFireType() { return m_enFireType; }
int CShip::GetShipKills() { return m_nShipKills; }
int CShip::GetSpeedUpPercentRemaining()
{ 
	static int last_percent;
	DWORD time=Timer.GameTime();
	
	if(m_nSpeedUpCount>0)
	{
		if(MenuActive) return last_percent;
		last_percent=int((((float(m_nSpeedTimeStart)+SPEED_UP_TIME)-time)/SPEED_UP_TIME)*100);
		return int((((float(m_nSpeedTimeStart)+SPEED_UP_TIME)-time)/SPEED_UP_TIME)*100);
	}
	else
		return 0;
}


