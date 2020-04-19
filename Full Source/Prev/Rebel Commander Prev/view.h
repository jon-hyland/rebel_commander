#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "defines.h"

#ifndef __VIEW_H__
#define __VIEW_H__

class CViewPoint
{
	private:
		int m_nLastTimeBgDrawn;
	public:
		int m_nBgOffset;		//moved!!
		int m_nX;				//moved!!
		CViewPoint();
		void set_position(int x);
		int screen(int x);
		void normalize(int &x);
		void draw_background(LPDIRECTDRAWSURFACE lpSource, LPDIRECTDRAWSURFACE lpDestination, int speed);
};

#endif __VIEW_H__
