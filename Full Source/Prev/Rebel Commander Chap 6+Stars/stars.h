#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "defines.h"
#include "plot.h"
#include "random.h"

#ifndef __STARS_H__
#define __STARS_H__

extern LPDIRECTDRAWSURFACE lpSecondary;

enum StarType {SMALL_STAR=0, LARGE_STAR=1};

struct Star
{
	int x;
	int y;
	int x_speed;
	int y_speed;
	BYTE pal_index;
	BYTE startype;
};

class CStars
{
	private:
		Star m_iStarArray[STAR_COUNT];
		int m_iStarCount;
		CRandom randgen;
	public:
		CStars(int starcount);
		~CStars();
		void move();
		void draw(DDSURFACEDESC *ddsd);
};

#endif