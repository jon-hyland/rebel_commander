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
#include "stars.h"
#include "random.h"
#include "plot.h"
#include "objman.h"
#include "view.h"
#include "convert.h"

#define MAX_OBJECTS 32

BOOL ActiveApp;

LPDIRECTDRAW lpDirectDrawObject=NULL;
LPDIRECTDRAWSURFACE lpPrimary=NULL;
LPDIRECTDRAWPALETTE lpPrimaryPalette;
LPDIRECTDRAWSURFACE lpSecondary=NULL;
LPDIRECTDRAWPALETTE lpSecondaryPalette;
LPDIRECTDRAWSURFACE lpBackground=NULL;

CTimer Timer;

CStars stars(STAR_COUNT);

CBmpFileReader background;
CBmpSpriteFileReader g_cShipSprites;
CBmpSpriteFileReader g_cBlipSprites;
CBmpSpriteFileReader g_cGroundCannonSprites;

CObjectManager ObjectManager(MAX_OBJECTS);

CClippedSprite *g_pSprite[NUM_SPRITES];

CViewPoint Viewpoint;
CRandom Random;

LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface);
BOOL InitDirectDraw(HWND hwnd);
HWND CreateDefaultWindow(char *name, HINSTANCE hInstance);

BOOL LoadShipSprite()
{
	BOOL result=TRUE;
	result=result&&g_pSprite[SHIP_OBJECT]->load(&g_cShipSprites,0,3,3);
	result=result&&g_pSprite[SHIP_OBJECT]->load(&g_cShipSprites,1,96,3);
	return result;
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

BOOL LoadGroundCannonSprite()
{
	BOOL result=TRUE;
	result=result&&g_pSprite[GROUND_CANNON_OBJECT]->load(&g_cGroundCannonSprites,0,3,3);
	return result;
}


BOOL LoadImages()
{
	if(!background.load("256_bottom_grey.bmp")) return FALSE;
	background.draw(lpBackground);
	if(!background.setpalette(lpPrimaryPalette)) return FALSE;
	if(!background.setpalette(lpSecondaryPalette)) return FALSE;

	if(!g_cShipSprites.load("256_ship_sprites.bmp")) return FALSE;
	if(!g_cBlipSprites.load("256_blip_sprites.bmp")) return FALSE;
	if(!g_cGroundCannonSprites.load("256_ground_cannon_sprites.bmp")) return FALSE;

	g_pSprite[SHIP_OBJECT]=new CClippedSprite(2,90,60);
	if(!LoadShipSprite()) return FALSE;
	g_pSprite[BLIP_OBJECT]=new CClippedSprite(4,36,36);
	if(!LoadBlipSprite()) return FALSE;
	g_pSprite[GROUND_CANNON_OBJECT]=new CClippedSprite(1,36,32);
	if(!LoadGroundCannonSprite()) return FALSE;

	return TRUE;
}

void CreateObjects()
{
	ObjectManager.set_current(ObjectManager.create(SHIP_OBJECT,466,220,0,0));
	ObjectManager.create(BLIP_OBJECT,1025,100,-2,0);
	ObjectManager.create(BLIP_OBJECT,960,250,-2,0);
	ObjectManager.create(BLIP_OBJECT,1040,400,-2,0);
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
	if(g_pSprite[GROUND_CANNON_OBJECT]->Restore())
		result=result&&LoadGroundCannonSprite();
	else return FALSE;
	return result;
}

BOOL PageFlip()
{
	if(lpPrimary->Flip(NULL,DDFLIP_WAIT)==DDERR_SURFACELOST)
		return RestoreSurfaces();
	return TRUE;
}


//!!DEBUG DEBUG DEBUG!!
//temp function draws all debug info to the screen
//edit this function to modify debug info
void DebugOut(LPDIRECTDRAWSURFACE surface)
{
	int line_count=0;
	HDC hdc;
	int y;
	char cint[64];

    if (surface->GetDC(&hdc)==DD_OK)
	{
		//viewpoint x
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"viewpoint x=", strlen("viewpoint x="));
		TextOut(hdc,130,y,int2char(Viewpoint.m_nX, cint),
			strlen(int2char(Viewpoint.m_nX, cint)));
		
		//background viewpoint offset
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"background offset=", strlen("background offset="));
		TextOut(hdc,130,y,int2char(Viewpoint.m_nBgOffset, cint),
			strlen(int2char(Viewpoint.m_nBgOffset, cint)));

		//ship x
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship x=", strlen("ship x="));
		TextOut(hdc,130,y,int2char(ObjectManager.m_pObjectList[0]->get_x(), cint),
			strlen(int2char(ObjectManager.m_pObjectList[0]->get_x(), cint)));
		
		//ship y
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship y=", strlen("ship y="));
		TextOut(hdc,130,y,int2char(ObjectManager.m_pObjectList[0]->get_y(), cint),
			strlen(int2char(ObjectManager.m_pObjectList[0]->get_y(), cint)));

		//ship x speed
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship xspeed=", strlen("ship xspeed="));
		TextOut(hdc,130,y,int2char(ObjectManager.m_pObjectList[0]->get_xspeed(), cint),
			strlen(int2char(ObjectManager.m_pObjectList[0]->get_xspeed(), cint)));

		//ship y speed
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship yspeed=", strlen("ship yspeed="));
		TextOut(hdc,130,y,int2char(ObjectManager.m_pObjectList[0]->get_yspeed(), cint),
			strlen(int2char(ObjectManager.m_pObjectList[0]->get_yspeed(), cint)));

		//ObjectManager.speed()
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"obj man speed=", strlen("obj man speed="));
		TextOut(hdc,130,y,int2char(ObjectManager.speed(), cint),
			strlen(int2char(ObjectManager.speed(), cint)));


		//Middle blip x
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"middle blip x=", strlen("middle blip x="));
		TextOut(hdc,130,y,int2char(ObjectManager.m_pObjectList[2]->get_x(), cint),
			strlen(int2char(ObjectManager.m_pObjectList[2]->get_x(), cint)));


		
		surface->ReleaseDC(hdc);
	}
}

BOOL ComposeFrame()
{
	//clear the draw surface (when using no bmp'd background)
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = 0;
	HRESULT ddrval = lpSecondary->Blt(NULL, NULL, NULL,DDBLT_WAIT |
	DDBLT_COLORFILL, &ddbltfx);

	//lock surface and draw background plot pixels
	DDSURFACEDESC ddsd;
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
    {
		//draw any needed background pixels using ddsd reference
		stars.move();
		stars.draw(&ddsd);
		IDirectDrawSurface_Unlock(lpSecondary, NULL);
	}	
	

	//set viewpoint position to 540 (220+320)
	Viewpoint.set_position(540);
	
	//draw background at given speed
	Viewpoint.draw_background(lpBackground, lpSecondary, -4);
	
	//move and draw each object
	ObjectManager.animate(lpSecondary);

	//lock surface and draw foreground plot pixels
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
    {
		//draw any needed foreground pixels using ddsd reference
	
		IDirectDrawSurface_Unlock(lpSecondary, NULL);
	}	
	

	//temp code to draw debug info to screen	
	DebugOut(lpSecondary);	
	

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
		case VK_UP:		ObjectManager.accelerate(0,-1);	break;
		case VK_DOWN:	ObjectManager.accelerate(0,1);	break;
		case VK_LEFT:	ObjectManager.accelerate(-1,0);	break;
		case VK_RIGHT:	ObjectManager.accelerate(1,0);	break;
		default:	break;
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