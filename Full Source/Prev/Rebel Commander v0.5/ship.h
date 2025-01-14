#ifndef __SHIP_H__
#define __SHIP_H__

#include "timer.h"
#include "bsprite.h"
#include "csprite.h"
#include "random.h"
#include "view.h"
#include "objman.h"
#include "sound.h"

class CShip
{
	private:
		int m_nX, m_nY;
		float m_nVectorX, m_nVectorY;
		float m_nVectorSpeed;
		int m_nDirection;
		int m_nLastXMoveTime, m_nLastYMoveTime;
		int m_nWidth, m_nHeight;
		int m_nBottomBoundary;
		int m_nColliWidth, m_nColliHeight;
		BOOL m_bDying, m_bDead;
		CClippedSprite *m_pSprite;
		CClippedSprite *m_pUpSprite;
		CClippedSprite *m_pDownSprite;
		CClippedSprite *m_pDyingSprite;
		int m_nCurrentFrame;
		int m_nFrameCount;
		int m_nLastFrameTime;
		int m_nFrameInterval;
		int m_nTurnCurrentFrame;
		BOOL m_bTurnAnimateBack;
		int m_nTurnFrameCount;
		int m_nTurnLastFrameTime;
		int m_nTurnFrameInterval;
		int m_nDyingCurrentFrame;
		int m_nDyingFrameCount;
		int m_nDyingLastFrameTime;
		int m_nDyingFrameInterval;
	public:
		CShip();
		void Draw(LPDIRECTDRAWSURFACE surface);
		void Create(int x, int y);
		void SetDirection(int direction);
		void SetBottomBoundary(int y);
		void Move();
		void Kill();
		int GetX(), GetY();
		float GetVectorX(), GetVectorY();
		int GetDirection();
		int GetBottomBoundary();
		BOOL Dying(), Dead();
};

#endif