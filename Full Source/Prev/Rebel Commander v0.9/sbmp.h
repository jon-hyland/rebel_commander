#ifndef __SBMP_H__
#define __SBMP_H__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "bmp.h"
#include "defines.h"

class CBmpSpriteFileReader:
	public CBmpFileReader
	{
		public:
			BOOL draw(LPDIRECTDRAWSURFACE surface, int width, int ht, int x, int y);
	};

#endif