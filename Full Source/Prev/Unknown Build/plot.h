#ifndef __PLOT_H__
#define __PLOT_H__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "defines.h"

void PutPx(int x, int y, BYTE pixel, DDSURFACEDESC *ddsd);
BYTE GetPx(int x, int y, DDSURFACEDESC *ddsd);

#endif