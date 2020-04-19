#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <math.h>
#include "timer.h"
#include "bsprite.h"
#include "csprite.h"
#include "random.h"
#include "view.h"

struct ObjEventItem
{
	BOOL bMoveToPosition;
	int iTargetX, iTargetY;
	float iVectorSpeed;	
	int iNextEventIndex;
};

class CObject
{
friend class CObjectManager;
private:
	ObjectType m_enObjectType;
	int m_nX, m_nY;
	int m_nLastX, m_nLastY;
	float m_nVectorX, m_nVectorY;
	int m_nLastXMoveTime, m_nLastYMoveTime;
	int m_nWidth, m_nHeight;
	BOOL m_bDying, m_bDead;
	CBaseSprite *m_pSprite;
	int m_nCurrentFrame;
	int m_nFrameCount;
	int m_nLastFrameTime;
	int m_nFrameInterval;
	BOOL m_bPivotAnimation;
	BOOL m_bAnimateBack;
	int m_nDyingCurrentFrame;
	int m_nDyingFrameCount;
	int m_nDyingLastFrameTime;
	int m_nDyingFrameInterval;
	ObjEventItem **m_pEventList;
	int m_nEventIndex;
	void AlterVector(float &vectorx, float &vectory, int current_x, int current_y, int target_x, int target_y, float vector_speed);
	BOOL ReachedDestination(int dest_x, int dest_y, int current_x, int current_y, int orig_x, int orig_y);
public:
	CObject(ObjectType object, SpriteType object_sprite, SpriteType dying_sprite, int x, int y, ObjEventItem **event_list);
	void Draw(LPDIRECTDRAWSURFACE surface);
	void Move();
	void Kill();
	ObjectType GetObjectType();
	int	GetX(), GetY();
	float GetVectorX(), GetVectorY();
	BOOL Dying(); BOOL Dead();
};

#endif