#include "fire.h"

extern CClippedSprite *g_pSprite[];
extern CTimer Timer;
extern CRandom Random;
extern CViewPoint Viewpoint;
extern CShip Ship;
extern CObjectManager ObjectManager;
extern CSoundManager *SoundManager;

CFire::CFire(FireType firetype, int orig_x, int orig_y, int target_x, int target_y, float vector_speed)
{
	m_enFireType=firetype;
	m_nOriginX=m_nX=orig_x;
	m_nOriginY=m_nY=orig_y;
	m_nTargetX=target_x;
	m_nTargetY=target_y;
	m_nVectorSpeed=vector_speed;
	
	m_nLastXMoveTime=m_nLastYMoveTime=Timer.GameTime();
	m_bDead=FALSE;
	m_bShipFire=FALSE;
	m_nVectorX=m_nVectorY=0;
	m_pSprite=NULL;

	switch(m_enFireType)
	{
		case SHIP_LASER_ONE:
			m_nWidth=16;
			m_nHeight=4;
			m_nColliWidth=16;
			m_nColliHeight=4;
			m_nOriginX=m_nX+=28;	//offsets from ship
			m_nOriginY=m_nY-=1;
			m_nTargetY-=1;
			m_bShipFire=TRUE;
			AlterVector(m_nVectorX, m_nVectorY, m_nX, m_nY, m_nTargetX, m_nTargetY, m_nVectorSpeed);
			SoundManager->Play(ZAP_FULL_SOUND);
			break;
		case ENEMY_FIRE_ONE:
			m_nWidth=12;
			m_nHeight=12;
			m_nColliWidth=10;
			m_nColliHeight=10;
			m_pSprite=g_pSprite[DEFAULT_FIRE_SPRITE];
			AlterVector(m_nVectorX, m_nVectorY, m_nX, m_nY, m_nTargetX, m_nTargetY, m_nVectorSpeed);
			//no sound
			break;
	}
}

//changes the direction (vector) of a fire object based on a target position (used?)
void CFire::AlterVector(float &vectorx, float &vectory, int current_x, int current_y, int target_x, int target_y, float vector_speed)
{
	float vector_length;
	static float vector_remainder;

	vectorx=float(target_x-current_x);
	vectory=float(target_y-current_y);
	vector_length=float(sqrt(vectorx*vectorx+vectory*vectory));
	if(vector_length<vector_speed)
	{
		vectorx=(vectorx/vector_speed)*vector_speed+vector_remainder;
		vectory=(vectory/vector_speed)*vector_speed+vector_remainder;
		vector_remainder=0;//vector_speed-vector_length;
	}
	else
	{
		vectorx=(vectorx/vector_length)*vector_speed+vector_remainder;
		vectory=(vectory/vector_length)*vector_speed+vector_remainder;
		vector_remainder=0;
	}
}

BOOL CFire::ReachedDestination(int dest_x, int dest_y, int current_x, int current_y, int orig_x, int orig_y)
{
	if(current_x==dest_x&&current_y==dest_y)
		return TRUE;
	return FALSE;
}

void CFire::Move()
{
	const int XSCALE=16;
	const int YSCALE=16;
	
	int xdelta, ydelta;
	int time=Timer.GameTime();
	int tfactor;
	int i;
	
	switch(m_enFireType)
	{
		//every fire type moves differently, could have different death triggers,
		//and may alter vector as needed
		case SHIP_LASER_ONE:	//a simple one
			if((m_nX>=860)||(m_nX<220)||(m_nY>=480)||(m_nY<0))
				m_bDead=TRUE;
			if(!m_bDead)
			{
				tfactor=time-m_nLastXMoveTime;
				xdelta=int((m_nVectorX*tfactor)/XSCALE);
				m_nX+=xdelta;
				if(xdelta||m_nVectorX==0)
					m_nLastXMoveTime=time;
			}
			break;
		case ENEMY_FIRE_ONE:
			if((m_nX>=860)||(m_nX<220)||(m_nY>=480)||(m_nY<0))
				m_bDead=TRUE;
			if(!m_bDead)
			{
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
			}
			break;
	}

	//check for collisions with ship and enemy objects (depending on fire type)
	switch(m_enFireType)
	{
		case SHIP_LASER_ONE:
			for(i=0; i<MAX_OBJECT_INSTANCES; i++)
				//check if object exists and is still alive
				if((ObjectManager.ObjList[i]!=NULL)&&(!ObjectManager.ObjList[i]->Dead())&&(!ObjectManager.ObjList[i]->Dying()))
				{
					//if collision is detected (using collision dimentions)
					if (abs(m_nX-ObjectManager.ObjList[i]->GetX())<(m_nColliWidth/2)+(ObjectManager.ObjList[i]->GetColliWidth()/2)
						&&
						abs(m_nY-ObjectManager.ObjList[i]->GetY())<(m_nColliHeight/2)+(ObjectManager.ObjList[i]->GetColliWidth()/2))
					{
						//kill the enemy object
						ObjectManager.ObjList[i]->Kill();

						//kill the fire object
						m_bDead=TRUE;
					}
				}
			break;
		case ENEMY_FIRE_ONE:
			//if ship is alive
			if((!Ship.Dead())&&(!Ship.Dying()))
			{
				//if collision is detected (using collision demintions)
				if (abs(m_nX-Ship.GetX())<(m_nColliWidth/2)+(Ship.GetColliWidth()/2)
					&&
					abs(m_nY-Ship.GetY())<(m_nColliHeight/2)+(Ship.GetColliWidth()/2))
				{
					//kill the ship
					Ship.Kill();
					//kill the fire object
					m_bDead=TRUE;
				}
			}
			break;
	}
}

void CFire::DrawPlot(DDSURFACEDESC *ddsd)
{
	int x;//, y;	//increment vars for plotting

	//follow pattern of draw then animate

	//draw function will assume a locked surface but will not enforce

	switch(m_enFireType)
	{
		case SHIP_LASER_ONE:	//draw laser 1 (length=16, width=4)
			for(x=-7; x<=8; x++)
			{
				switch(x)
				{
					case -7: case -6:
						PutPx(x+m_nX, -1+m_nY, 177, ddsd);
						PutPx(x+m_nX, 0+m_nY, 177, ddsd);
						PutPx(x+m_nX, 1+m_nY, 98, ddsd);
						PutPx(x+m_nX, 2+m_nY, 98, ddsd);
						break;
					case -3: case -2:
						PutPx(x+m_nX, -1+m_nY, 156, ddsd);
						PutPx(x+m_nX, 0+m_nY, 156, ddsd);
						PutPx(x+m_nX, 1+m_nY, 177, ddsd);
						PutPx(x+m_nX, 2+m_nY, 177, ddsd);
						break;
					case -1: case 0:
						PutPx(x+m_nX, -1+m_nY, 98, ddsd);
						PutPx(x+m_nX, 0+m_nY, 98, ddsd);
						break;
					case 1: case 2:
						PutPx(x+m_nX, -1+m_nY, 62, ddsd);
						PutPx(x+m_nX, 0+m_nY, 62, ddsd);
						PutPx(x+m_nX, 1+m_nY, 156, ddsd);
						PutPx(x+m_nX, 2+m_nY, 156, ddsd);
						break;
					case 3: case 4:
						PutPx(x+m_nX, -1+m_nY, 85, ddsd);
						PutPx(x+m_nX, 0+m_nY, 85, ddsd);
						PutPx(x+m_nX, 1+m_nY, 156, ddsd);
						PutPx(x+m_nX, 2+m_nY, 156, ddsd);
						break;
					case 5: case 6:
						PutPx(x+m_nX, -1+m_nY, 85, ddsd);
						PutPx(x+m_nX, 0+m_nY, 85, ddsd);
						PutPx(x+m_nX, 1+m_nY, 62, ddsd);
						PutPx(x+m_nX, 2+m_nY, 62, ddsd);
						break;
					case 7: case 8:
						PutPx(x+m_nX, -1+m_nY, 156, ddsd);
						PutPx(x+m_nX, 0+m_nY, 156, ddsd);
						PutPx(x+m_nX, 1+m_nY, 156, ddsd);
						PutPx(x+m_nX, 2+m_nY, 156, ddsd);
						break;
				}
			}
			break;
	}
}


void CFire::DrawBlit(LPDIRECTDRAWSURFACE surface)
{
	//follow pattern of draw then animate

	//draw function will assume an unlocked surface but will not enforce

	switch(m_enFireType)
	{
		case ENEMY_FIRE_ONE:
			//blit the DEFAULT_FIRE_SPRITE
			//PutPx(m_nX, m_nY, 255, ddsd);
			//PutPx(m_nX+1, m_nY, 255, ddsd);
			//PutPx(m_nX, m_nY+1, 255, ddsd);
			//PutPx(m_nX+1, m_nY+1, 255, ddsd);
			m_pSprite->draw(0,Viewpoint.screen(m_nX),m_nY,surface);
			//SoundManager->Play(SELECT_SOUND);
			break;
			
	}
}


FireType CFire::GetFireType() { return m_enFireType; }