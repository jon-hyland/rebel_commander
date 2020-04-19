#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN

#include "defines.h"
#include "bmp.h"
#include "timer.h"
#include "bsprite.h"
#include "objects.h"

BOOL ActiveApp;

LPDIRECTDRAW lpDirectDrawObject=NULL;
LPDIRECTDRAWSURFACE lpPrimary=NULL;
LPDIRECTDRAWPALETTE lpPrimaryPalette;
LPDIRECTDRAWSURFACE lpSecondary=NULL;
LPDIRECTDRAWPALETTE lpSecondaryPalette;
LPDIRECTDRAWSURFACE lpBackground=NULL;

CTimer Timer;


CBmpFileReader background;
CBmpSpriteFileReader g_cSpriteImages;

CObject plane;
CBaseSprite *planesprite=NULL;

LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface);
BOOL InitDirectDraw(HWND hwnd);
HWND CreateDefaultWindow(char *name, HINSTANCE hInstance);

BOOL LoadPlaneSprite()
{
	return planesprite->load(&g_cSpriteImages,0,3,3);  // SETS FRAME COUNT, START X Y
}


BOOL LoadImages()
{
	if(!background.load("256_stars1.bmp")) return FALSE;
	background.draw(lpBackground);
	if(!background.setpalette(lpPrimaryPalette)) return FALSE;
	if(!background.setpalette(lpSecondaryPalette)) return FALSE;
	if(!g_cSpriteImages.load("256_ship_sprites.bmp")) return FALSE;
	planesprite=new CBaseSprite(1,90,60);	// SETS SIZE, COUNT OF SPRITE
	if(!LoadPlaneSprite()) return FALSE;

	return TRUE;
}

void CreateObjects()
{
	plane.create(320,271,-1,0,planesprite);
}


BOOL RestoreSurfaces()
{
	BOOL result=TRUE;
	if(SUCCEEDED(lpPrimary->Restore()))
		result=result&&background.draw(lpPrimary)&&background.setpalette(lpPrimaryPalette);
	else return FALSE;
	if(SUCCEEDED(lpSecondary->Restore()))
		result=result&&background.draw(lpSecondary)&&background.setpalette(lpSecondaryPalette);
	else return FALSE;
	if(SUCCEEDED(lpBackground->Restore()))
		result=result&&background.draw(lpBackground);
	else return FALSE;
	if(planesprite->Restore())
		result=result&&LoadPlaneSprite();
	else return FALSE;
	return result;
}

BOOL PageFlip()
{
	if(lpPrimary->Flip(NULL,DDFLIP_WAIT)==DDERR_SURFACELOST)
		return RestoreSurfaces();
	return TRUE;
}

BOOL ComposeFrame()
{
	lpSecondary->BltFast(0,0,lpBackground,NULL,DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
	plane.move();
	plane.draw(lpSecondary);
	return TRUE;
}	

BOOL ProcessFrame()
{
	ComposeFrame();
	return PageFlip();
}

BOOL keyboard_handler(WPARAM keystroke)
{
	BOOL result=FALSE;
	switch(keystroke)
	{
		case VK_ESCAPE: result=TRUE; break;
		default: break;
	}
	return result;
}

long CALLBACK WindowProc(HWND hwnd,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP: ActiveApp=wParam; break;
		case WM_CREATE:	break;
		case WM_KEYDOWN:
			if(keyboard_handler(wParam))DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			if(lpDirectDrawObject!=NULL)
			{
				if(lpSecondary!=NULL)
					lpSecondary->Release();
				if(lpPrimary!=NULL)
					lpPrimary->Release();
				if(lpBackground!=NULL)
					lpBackground->Release();
				if(planesprite!=NULL)
					planesprite->Release();
				delete planesprite;
				lpDirectDrawObject->Release();
			}
			ShowCursor(TRUE);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0L;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hwnd;
	hwnd=CreateDefaultWindow("Rebel Commander",hInstance);
	if(!hwnd)return FALSE;
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(FALSE);
	BOOL OK=InitDirectDraw(hwnd);
	if(OK)OK=LoadImages();
	if(!OK){
		DestroyWindow(hwnd);
		return FALSE;
	}

	Timer.start();

	CreateObjects();
	
	while(TRUE)
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if(!GetMessage(&msg,NULL,0,0)) return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if(ActiveApp)ProcessFrame(); else WaitMessage();
}
