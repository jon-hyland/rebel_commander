#ifndef __STARS_H__
#define __STARS_H__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "defines.h"
#include "plot.h"
#include "random.h"

extern LPDIRECTDRAWSURFACE lpSecondary;

enum StarType {SMALL_STAR=0, LARGE_STAR=1};

struct Star
{
	float X;
	float Y;
	float VectorX;
	float VectorY;
	BYTE Pal_Index;
	StarType Star_Type;
};

class CStars
{
	private:
		Star m_iStarArray[STAR_COUNT];
		int m_iStarCount;
		CRandom Random;
	public:
		CStars(int starcount);
		~CStars();
		void Move();
		void Draw(DDSURFACEDESC *ddsd);
};

#endif