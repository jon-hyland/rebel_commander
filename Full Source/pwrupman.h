#ifndef __PWRUPMAN_H__
#define __PWRUPMAN_H__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "powerup.h"

class CPowerUpManager
{
	private:
		int m_nCount;
	public:
		CPowerUp **UpList;
		CPowerUpManager();
		~CPowerUpManager();
		int Create(UpType up_type, int x, int y);
		void DrawAll(LPDIRECTDRAWSURFACE surface);
		void Reset();
		int GetCount();
};

#endif