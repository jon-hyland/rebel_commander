#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "objects.h"

#ifndef __OBJMAN_H__
#define __OBJMAN_H__

class CObjectManager
{
	private:
		int m_nCount;
		int m_nMaxCount;
		int m_nCurrentObject;
	public:
		CObject **m_pObjectList; // moved to public for easy access
		CObjectManager(int max);
		~CObjectManager();
		int create(ObjectType object, int x, int y, int xspeed, int yspeed);
		void animate(LPDIRECTDRAWSURFACE surface);
		void accelerate(int xdelta, int ydelta=0);
		void set_current(int index);
		int speed();
};

#endif