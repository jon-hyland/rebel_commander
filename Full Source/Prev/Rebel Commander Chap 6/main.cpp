#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN

#include "defines.h"
#include "bmp.h"
#include "timer.h"
#include "csprite.h"
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
CBmpSpriteFileReader g_cShipSprites;
CBmpSpriteFileReader g_cBlipSprites;

CObject ship;
CObject blip1, blip2, blip3;
CClippedSprite *g_pSprite[NUM_SPRITES];

LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface);
BOOL InitDirectDraw(HWND hwnd);
HWND CreateDefaultWindow(char *name, HINSTANCE hInstance);

BOOL LoadShipSprite()
{
	return g_pSprite[SHIP_OBJECT]->load(&g_cShipSprites,0,3,3);
}

BOOL LoadBlipSprite()
{
	BOOL result=TRUE;

	result=result&&g_pSprite[BLIP_OBJECT]->load(&g_cBlipSprites,0,3,3);
	result=result&&g_pSprite[BLIP_OBJECT]->load(&g_cBlipSprites,1,42,3);
	result=result&&g_pSprite[BLIP_OBJECT]->load(&g_cBlipSprites,2,81,3);
	result=result&&g_pSprite[BLIP_OBJECT]->load(&g_cBlipSprites,3,120,3);
	return result;
}


BOOL LoadImages()
{
	if(!background.load("256_stars1.bmp")) return FALSE;
	background.draw(lpBackground);
	if(!background.setpalette(lpPrimaryPalette)) return FALSE;
	if(!background.setpalette(lpSecondaryPalette)) return FALSE;
	
	if(!g_cShipSprites.load("256_ship_sprites.bmp")) return FALSE;
	if(!g_cBlipSprites.load("256_blip_sprites.bmp")) return FALSE;

	g_pSprite[SHIP_OBJECT]=new CClippedSprite(1,90,60);
	if(!LoadShipSprite()) return FALSE;
	g_pSprite[BLIP_OBJECT]=new CClippedSprite(4,36,36);
	if(!LoadBlipSprite()) return FALSE;

	return TRUE;
}

void CreateObjects()
{
	ship.create(SHIP_OBJECT,100,220,2,0);
	blip1.create(BLIP_OBJECT,625,100,-1,0);
	blip2.create(BLIP_OBJECT,560,250,-1,0);
	blip3.create(BLIP_OBJECT,640,400,-1,0);
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
	if(g_pSprite[SHIP_OBJECT]->Restore())
		result=result&&LoadShipSprite();
	else return FALSE;
	if(g_pSprite[BLIP_OBJECT]->Restore())
		result=result&&LoadBlipSprite();
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
	RECT rect;
	
	rect.left=0;	
	rect.right=SCREEN_WIDTH;
	rect.top=0;
	rect.bottom=SCREEN_HEIGHT;
	lpSecondary->Blt(&rect,lpBackground,&rect,DDBLT_WAIT,NULL);
	
	ship.move();
	blip1.move();
	blip2.move();
	blip3.move();
	ship.draw(lpSecondary);
	blip1.draw(lpSecondary);
	blip2.draw(lpSecondary);
	blip3.draw(lpSecondary);
	
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
		case VK_ESCAPE: result=TRUE;	break;
		case VK_UP:		ship.accelerate(0,-1);	break;
		case VK_DOWN:	ship.accelerate(0,1);	break;
		case VK_LEFT:	ship.accelerate(-1,0);	break;
		case VK_RIGHT:	ship.accelerate(1,0);	break;
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
				for(int i=0; i<NUM_SPRITES; i++)
				{
					if(g_pSprite[i])
						g_pSprite[i]->Release();
					delete g_pSprite[i];
				}
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

	for(int i=0; i<NUM_SPRITES; i++)
		g_pSprite[i]=NULL;
	
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
