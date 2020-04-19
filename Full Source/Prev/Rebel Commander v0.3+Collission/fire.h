#ifndef __FIRE_H__
#define __FIRE_H__

#include <math.h>
#include "timer.h"
#include "bsprite.h"
#include "csprite.h"
#include "random.h"
#include "view.h"
#include "defines.h"
#include "plot.h"
#include "objman.h"

class CFire
{
friend class CFireManager;
private:
	FireType m_enFireType;
	BOOL m_bShipFire;
	int m_nOriginX;
	int m_nOriginY;
	int m_nX;
	int m_nY;
	int m_nTargetX;
	int m_nTargetY;
	int m_nWidth;
	int m_nHeight;
	int m_nColliWidth, m_nColliHeight;
	int m_nLastXMoveTime;
	int m_nLastYMoveTime;
	int m_nBoundaryTop;
	int m_nBoundaryBottom;
	int m_nBoundaryLeft;
	int m_nBoundaryRiht;
	float m_nVectorSpeed;
	float m_nVectorX;
	float m_nVectorY;
	CBaseSprite *m_pSprite;
	BOOL m_bDead;	
	void AlterVector(float &vectorx, float &vectory, int current_x, int current_y, int target_x, int target_y, float vector_speed);
	BOOL ReachedDestination(int dest_x, int dest_y, int current_x, int current_y, int orig_x, int orig_y);
public:
	CFire(FireType firetype, int orig_x, int orig_y, int target_x, int target_y);
	void Move();
	void Draw(DDSURFACEDESC *ddsd);
};

#endif