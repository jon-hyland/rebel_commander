#ifndef __BMP_H__
#define __BMP_H__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "defines.h"

class CBmpFileReader{
protected:
	BITMAPFILEHEADER m_BMPFileHead;
	BITMAPINFOHEADER m_BMPFileInfo;
	RGBQUAD m_rgbPalette[COLORS];
	BYTE *m_cImage;
public:
	CBmpFileReader();
	~CBmpFileReader();
	BOOL load(char *filename);
	BOOL draw(LPDIRECTDRAWSURFACE surface);
	BOOL setpalette(LPDIRECTDRAWPALETTE palette);
};

#endif