#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "timer.h"
#include "bsprite.h"
#include "csprite.h"
#include "random.h"
#include "view.h"
#include "objman.h"
#include "sound.h"

class CPowerUp
{
	private:
		int m_nX, m_nY;
		int m_nWidth, m_nHeight;
		int m_nColliWidth, m_nColliHeight;
		UpType m_enUpType;
		BOOL m_bDead;
		CClippedSprite *m_pSprite;
		int m_nCurrentFrame;
		int m_nFrameCount;
		int m_nLastFrameTime;
		int m_nFrameInterval;
	public:
		CPowerUp();
		void Draw(LPDIRECTDRAWSURFACE surface);
		void Create(UpType up_type, int x, int y);
		void Kill();
		int GetX(), GetY();
		int GetColliWidth(), GetColliHeight();
		UpType GetUpType();
		BOOL Dead();
};

#endif