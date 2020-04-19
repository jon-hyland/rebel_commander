#include "object.h"

extern int GameState;
extern CClippedSprite *g_pSprite[];
extern CTimer Timer;
extern CRandom Random;
extern CViewPoint Viewpoint;
extern CObjectManager ObjectManager;
extern CSoundManager *SoundManager;

CObject::CObject(ObjectType object, SpriteType object_sprite, SpriteType dying_sprite, int x, int y, ObjEventItem **event_list)
{
	m_enObjectType=object;
	m_nX=x;	m_nY=y;
	m_nLastX=0; m_nLastY=0;
	m_nVectorX=0; m_nVectorY=0;
	m_nLastXMoveTime=m_nLastYMoveTime=Timer.GameTime();
	m_nWidth=g_pSprite[object_sprite]->GetWidth();
	m_nHeight=g_pSprite[object_sprite]->GetHeight();
	m_pSprite=g_pSprite[object_sprite];
	m_nFrameCount=m_pSprite->frame_count();
	m_nCurrentFrame=0;
	m_nLastFrameTime=Timer.GameTime();
	m_bAnimateBack=FALSE;
	m_bPivotAnimation=TRUE;
	m_pDyingSprite=g_pSprite[dying_sprite];
	m_nDyingFrameCount=m_pDyingSprite->frame_count();
	m_nDyingCurrentFrame=0; 
	m_nDyingLastFrameTime=0;
	m_nDyingFrameInterval=500;
	m_pEventList=NULL;
	m_bDying=FALSE; m_bDead=FALSE;

	switch(m_enObjectType)
	{
		case BLIP_OBJECT:
			m_nColliWidth=25;
			m_nColliHeight=25;
			m_bPivotAnimation=FALSE;
			m_nFrameInterval=100;
			m_nDyingFrameInterval=50;
			break;
		case GROUND_CANNON_OBJECT:
			break;
	}

	m_pEventList=new ObjEventItem*[MAX_OBJECT_EVENTS];
	for(int i=0; i<MAX_OBJECT_EVENTS; i++)
	{
		m_pEventList[i]=new ObjEventItem;
		m_pEventList[i]->bMoveToPosition=event_list[i]->bMoveToPosition;
		m_pEventList[i]->iTargetX=event_list[i]->iTargetX;
		m_pEventList[i]->iTargetY=event_list[i]->iTargetY;
		m_pEventList[i]->iVectorSpeed=event_list[i]->iVectorSpeed;
		m_pEventList[i]->iNextEventIndex=event_list[i]->iNextEventIndex;
	}
	m_nEventIndex=0;
	m_nLastX=m_nX;
	m_nLastY=m_nY;
	AlterVector(m_nVectorX, m_nVectorY, m_nX, m_nY, m_pEventList[m_nEventIndex]->iTargetX, m_pEventList[m_nEventIndex]->iTargetY, m_pEventList[m_nEventIndex]->iVectorSpeed);

}


void CObject::AlterVector(float &vectorx, float &vectory, int current_x, int current_y, int target_x, int target_y, float vector_speed)
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

BOOL CObject::ReachedDestination(int dest_x, int dest_y, int current_x, int current_y, int orig_x, int orig_y)
{
//	float orig_x_dist, orig_y_dist;
//	float x_dist, y_dist;

	if(current_x==dest_x&&current_y==dest_y)
		return TRUE;

//hmmm.. maybe add prev..

	return FALSE;
}

void CObject::Draw(LPDIRECTDRAWSURFACE surface)
{
	//if not dying or dead
	if((!m_bDying)&&(!m_bDead))
	{
		m_pSprite->draw(m_nCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
		if(GameState!=0) //if not in main menu
		{
			//perform animation
			int t=m_nFrameInterval;//m_nFrameInterval/(1+abs(m_nVectorX));
			if(m_nFrameCount>1&&Timer.Elapsed(m_nLastFrameTime,t))
				if(m_bPivotAnimation) 
				{
					if(!m_bAnimateBack) 
					{
						if(++m_nCurrentFrame>=m_nFrameCount) 
						{
							m_bAnimateBack=TRUE;
							m_nCurrentFrame-=2;
						}
					}
					else 
					{
						if(--m_nCurrentFrame<0) 
						{
							m_bAnimateBack=FALSE;
							m_nCurrentFrame+=2;
						}
					}
				}
				else 
				{
					if(++m_nCurrentFrame>=m_nFrameCount) 
					{
						m_nCurrentFrame=0;
					}
				}
		}
	}
	//if object is in dying sequence
	else if(m_bDying)
	{
		m_pDyingSprite->draw(m_nDyingCurrentFrame, Viewpoint.screen(m_nX), m_nY, surface);
		if(GameState!=0) //if not in main menu
		{
			//perform animation
			int t=m_nDyingFrameInterval;
			if(m_nDyingFrameCount>1&&Timer.Elapsed(m_nDyingLastFrameTime,t))
				if(++m_nDyingCurrentFrame>=m_nDyingFrameCount) 
				{
					m_nDyingCurrentFrame--;
					m_bDead=TRUE;
				}
		}
	}
}

void CObject::Kill()
{
	m_bDying=TRUE;
	m_nDyingLastFrameTime=Timer.GameTime();
	SoundManager->Play(BLIP_DIE_SOUND);
}


void CObject::Move()
{
	const int XSCALE=16;
	const int YSCALE=16;
	
	int xdelta, ydelta;
	int time=Timer.GameTime();
	int tfactor;
	
	
	//decide if to move or not
	switch(m_enObjectType)
	{
		case BLIP_OBJECT:
			if(m_bDying||m_bDead)
				return;
			break;
	}

	//move object
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

	
	//evaluate new position
	switch(m_enObjectType)
	{
		case OBJECT_COUNT:	//remove this case once another is found! junk
			break;
		default:
			if(m_nX<0)m_nX=WORLD_WIDTH-1;
			if(m_nX>WORLD_WIDTH-1)m_nX=0;
			if(m_nY<0)m_nY=0;
			if(m_nY>SCREEN_HEIGHT-1)m_nY=SCREEN_HEIGHT-1;

			break;
	}


	//evaluate current event stats..
	
	//specific to MoveToPosition event type
	if(m_pEventList[m_nEventIndex]->bMoveToPosition)
	{
		if(ReachedDestination(m_pEventList[m_nEventIndex]->iTargetX, m_pEventList[m_nEventIndex]->iTargetY,
			m_nX, m_nY, m_nLastX, m_nLastY))
		{
			m_nLastX=m_nX;
			m_nLastY=m_nY;
			m_nEventIndex=m_pEventList[m_nEventIndex]->iNextEventIndex;
		}
		AlterVector(m_nVectorX, m_nVectorY, m_nX, m_nY, m_pEventList[m_nEventIndex]->iTargetX, m_pEventList[m_nEventIndex]->iTargetY, m_pEventList[m_nEventIndex]->iVectorSpeed);
	}
}


ObjectType CObject::GetObjectType() { return m_enObjectType; }
int	CObject::GetX() { return m_nX; }
int CObject::GetY() { return  m_nY; }
float CObject::GetVectorX() { return m_nVectorX; }
float CObject::GetVectorY() { return m_nVectorY; }
BOOL CObject::Dying() { return m_bDying; }
BOOL CObject::Dead() { return m_bDead; }
int CObject::GetColliWidth() { return m_nColliWidth; }
int CObject::GetColliHeight() { return m_nColliHeight; }
