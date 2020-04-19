#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "defines.h"

extern LPDIRECTDRAW lpDirectDrawObject;
extern LPDIRECTDRAWSURFACE lpPrimary;
extern LPDIRECTDRAWPALETTE lpPrimaryPalette;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern LPDIRECTDRAWPALETTE lpSecondaryPalette;
extern LPDIRECTDRAWSURFACE lpBackground;

LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface)
{
	PALETTEENTRY pe[COLORS];
	LPDIRECTDRAWPALETTE lpDDPalette;
	for(int i=0; i<COLORS; i++)
		pe[i].peRed=pe[i].peGreen=pe[i].peBlue=0;
	if(FAILED(lpDirectDrawObject->CreatePalette(DDPCAPS_8BIT,pe,&lpDDPalette,NULL)))
		return NULL;
	surface->SetPalette(lpDDPalette);
	return lpDDPalette;
}

BOOL InitDirectDraw(HWND hwnd)
{
	if(FAILED(DirectDrawCreate(NULL,&lpDirectDrawObject,NULL)))
		return FALSE;
	if(FAILED(lpDirectDrawObject->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN)))
		return FALSE;
	if(FAILED(lpDirectDrawObject->SetDisplayMode(SCREEN_WIDTH,SCREEN_HEIGHT,COLOR_DEPTH)))
		return FALSE;
	DDSURFACEDESC ddsd;
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount=1;
	if(FAILED(lpDirectDrawObject->CreateSurface(&ddsd,&lpPrimary,NULL)))
		return FALSE;
	lpPrimaryPalette=CreatePalette(lpPrimary);
	DDSCAPS ddscaps;
	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	if(FAILED(lpPrimary->GetAttachedSurface(&ddscaps,&lpSecondary)))
		return FALSE;
	lpSecondaryPalette=CreatePalette(lpSecondary);

	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight=SCREEN_HEIGHT;
	ddsd.dwWidth=SCREEN_WIDTH;
	if(FAILED(lpDirectDrawObject->CreateSurface(&ddsd, &lpBackground, NULL)))
		return FALSE;

	LPDIRECTDRAWCLIPPER lpClipper;
	if(FAILED(lpDirectDrawObject->CreateClipper(NULL, &lpClipper, NULL)))
		return FALSE;
	if(FAILED(lpClipper->SetHWnd(NULL,hwnd)))
		return FALSE;
	if(FAILED(lpSecondary->SetClipper(lpClipper)))
		return FALSE;

	return TRUE;
}

long CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND CreateDefaultWindow(char* name, HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=WindowProc;
	wc.cbClsExtra=wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=NULL;
	wc.lpszMenuName=NULL;
	wc.lpszClassName=name;
	RegisterClass(&wc);
	return CreateWindowEx(WS_EX_TOPMOST,name,name,WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hInstance,NULL);
}

	