#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "defines.h"
#include "fileout.h"
#include "dderror.h"

extern HWND hwnd;
extern LPDIRECTDRAW lpDirectDrawObject;
extern LPDIRECTDRAWSURFACE lpPrimary;
extern LPDIRECTDRAWPALETTE lpPalette;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern LPDIRECTDRAWSURFACE lpBackground;
extern CFileOut FileOut;


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

BOOL InitDirectDraw()
{
	DDSURFACEDESC ddsd;
	HRESULT hr;
	CDDError DDError;
	char *pszErrorDesc;

	//create direct draw object
	FileOut.WriteLine("CREATING DIRECT DRAW OBJECT");
	hr = DirectDrawCreate(NULL,&lpDirectDrawObject,NULL);
	if (FAILED(hr))
	{
		pszErrorDesc = DDError.GetDDErrorString(hr);
		FileOut.WriteLine(pszErrorDesc);
		return FALSE;
	}

	FileOut.WriteLine("SETTING COOP LEVEL");
    if(FULLSCREEN)
	{
		//set cooroperative level
		hr = lpDirectDrawObject->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);
		if (FAILED(hr))
		{
			pszErrorDesc = DDError.GetDDErrorString(hr);
			FileOut.WriteLine(pszErrorDesc);
			return FALSE;
		}

		FileOut.WriteLine("SETTING DISPLAY MODE");
		//set display mode
		hr = lpDirectDrawObject->SetDisplayMode(SCREEN_WIDTH,SCREEN_HEIGHT,COLOR_DEPTH);
		if (FAILED(hr))
		{
			pszErrorDesc = DDError.GetDDErrorString(hr);
			FileOut.WriteLine(pszErrorDesc);
			return FALSE;
		}
	}
	else
	{
		//set cooroperative level
		hr = lpDirectDrawObject->SetCooperativeLevel(hwnd,DDSCL_NORMAL);
		if (FAILED(hr))
		{
			pszErrorDesc = DDError.GetDDErrorString(hr);
			FileOut.WriteLine(pszErrorDesc);
			return FALSE;
		}
	}

	FileOut.WriteLine("CREATING SURFACES");
	//create primary and secondary surfaces, create palette(s)
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	
	if (FULLSCREEN)
	{
		ddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount=1;
		hr = lpDirectDrawObject->CreateSurface(&ddsd,&lpPrimary,NULL);
		if (FAILED(hr))
		{
			pszErrorDesc = DDError.GetDDErrorString(hr);
			FileOut.WriteLine(pszErrorDesc);
			return FALSE;
		}
		
		//pull the palette out for later
		lpPalette=CreatePalette(lpPrimary);

		DDSCAPS ddscaps;
		ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
		
		//get attached surface
		hr = lpPrimary->GetAttachedSurface(&ddscaps,&lpSecondary);
		if (FAILED(hr))
		{
			pszErrorDesc = DDError.GetDDErrorString(hr);
			FileOut.WriteLine(pszErrorDesc);
			return FALSE;
		}
	}
	else
	{
		//create the primary surface
		ddsd.dwFlags=DDSD_CAPS;
		ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
		hr = lpDirectDrawObject->CreateSurface(&ddsd,&lpPrimary,NULL);
		if (FAILED(hr))
		{
			pszErrorDesc = DDError.GetDDErrorString(hr);
			FileOut.WriteLine(pszErrorDesc);
			return FALSE;
		}
		
		//pull the palette out for later
		lpPalette=CreatePalette(lpPrimary);
		
		//create an off-screen back buffer (surface)
		ddsd.dwFlags = DDSD_WIDTH|DDSD_HEIGHT|DDSD_CAPS;
		ddsd.dwWidth = 640;
		ddsd.dwHeight = 480;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		hr = lpDirectDrawObject->CreateSurface(&ddsd,&lpSecondary,NULL);
		if (FAILED(hr))
		{
			pszErrorDesc = DDError.GetDDErrorString(hr);
			FileOut.WriteLine(pszErrorDesc);
			return FALSE;
		}
	}
	
	
	//create and attatch clipper to secondary surface
	LPDIRECTDRAWCLIPPER lpClipper;
	hr = lpDirectDrawObject->CreateClipper(NULL, &lpClipper, NULL);
	if (FAILED(hr))
	{
		pszErrorDesc = DDError.GetDDErrorString(hr);
		FileOut.WriteLine(pszErrorDesc);
		return FALSE;
	}
	
	hr = lpClipper->SetHWnd(NULL,hwnd);
	if (FAILED(hr))
	{
		pszErrorDesc = DDError.GetDDErrorString(hr);
		FileOut.WriteLine(pszErrorDesc);
		return FALSE;
	}
	
	hr = lpSecondary->SetClipper(lpClipper);
	if (FAILED(hr))
	{
		pszErrorDesc = DDError.GetDDErrorString(hr);
		FileOut.WriteLine(pszErrorDesc);
		return FALSE;
	}

	//create surface for background scrolling
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight=SCREEN_HEIGHT;
	ddsd.dwWidth=SCREEN_WIDTH;
	
	hr = lpDirectDrawObject->CreateSurface(&ddsd, &lpBackground, NULL);
	if (FAILED(hr))
	{
		pszErrorDesc = DDError.GetDDErrorString(hr);
		FileOut.WriteLine(pszErrorDesc);
		return FALSE;
	}
	
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue=ddck.dwColorSpaceHighValue=TRANSPARENT_COLOR;
	lpBackground->SetColorKey(DDCKEY_SRCBLT,&ddck);

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

	