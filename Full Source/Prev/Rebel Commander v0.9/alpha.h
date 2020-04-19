#ifndef __ALPHA_H__
#define __ALPHA_H__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <string.h>
#include <stdio.h>

#include "defines.h"
#include "csprite.h"
#include "view.h"

class CAlpha
{
private:
	CClippedSprite *m_pAlphaSprite;
public:
	CAlpha();
	~CAlpha();
	void Write(LPDIRECTDRAWSURFACE surface, int x, int y, char *text, SpriteType sprite_type);
	void Write(LPDIRECTDRAWSURFACE surface, int x, int y, int number, SpriteType sprite_type);
};

#endif