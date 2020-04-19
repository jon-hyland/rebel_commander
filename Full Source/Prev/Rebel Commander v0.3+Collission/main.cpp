#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN

#include "defines.h"
#include "bmp.h"
#include "timer.h"
#include "csprite.h"
#include "stars.h"
#include "random.h"
#include "plot.h"
#include "ship.h"
#include "objman.h"
#include "view.h"
#include "convert.h"
#include "fireman.h"

BOOL ActiveApp;
LPDIRECTDRAW lpDirectDrawObject=NULL;
LPDIRECTDRAWSURFACE lpPrimary=NULL;
LPDIRECTDRAWPALETTE lpPrimaryPalette;
LPDIRECTDRAWSURFACE lpSecondary=NULL;
LPDIRECTDRAWPALETTE lpSecondaryPalette;
LPDIRECTDRAWSURFACE lpBackground=NULL;
LPDIRECTINPUT lpDirectInputObject=NULL;
LPDIRECTINPUTDEVICE mKeyboard;
unsigned char keystate[256];
CTimer Timer;
CStars stars(STAR_COUNT);
CBmpFileReader background;
CBmpSpriteFileReader g_cShipSprites;
CBmpSpriteFileReader g_cBlipSprites;
CBmpSpriteFileReader g_cGroundCannonSprites;
CShip Ship;
CObjectManager ObjectManager;
CFireManager FireMan;
CClippedSprite *g_pSprite[SPRITE_COUNT];
CViewPoint Viewpoint;
CRandom Random;
double nFramesDrawn=0;
int nFPS;

//ddsetup.cpp function prototypes
LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface);
BOOL InitDirectDraw(HWND hwnd);
HWND CreateDefaultWindow(char *name, HINSTANCE hInstance);

//disetup.cpp function prototypes
BOOL InitDirectInput(HWND hwnd);
void GetKeystate();


BOOL LoadShipSprite()
{
	BOOL result=TRUE;
	result=result&&g_pSprite[SHIP_SPRITE]->load(&g_cShipSprites,0,1,1);
	result=result&&g_pSprite[SHIP_SPRITE]->load(&g_cShipSprites,1,66,1);
	result=result&&g_pSprite[SHIP_UP_SPRITE]->load(&g_cShipSprites,0,131,1);
	result=result&&g_pSprite[SHIP_UP_SPRITE]->load(&g_cShipSprites,1,196,1);
	result=result&&g_pSprite[SHIP_DOWN_SPRITE]->load(&g_cShipSprites,0,261,1);
	result=result&&g_pSprite[SHIP_DOWN_SPRITE]->load(&g_cShipSprites,1,1,34);
	result=result&&g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,0,66,34);
	result=result&&g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,1,131,34);
	result=result&&g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,2,196,34);
	result=result&&g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,3,261,34);
	return result;
}


BOOL LoadBlipSprite()
{
	BOOL result=TRUE;

	result=result&&g_pSprite[BLIP_SPRITE]->load(&g_cBlipSprites,0,1,1);
	result=result&&g_pSprite[BLIP_SPRITE]->load(&g_cBlipSprites,1,34,1);
	result=result&&g_pSprite[BLIP_SPRITE]->load(&g_cBlipSprites,2,67,1);
	result=result&&g_pSprite[BLIP_SPRITE]->load(&g_cBlipSprites,3,100,1);
	result=result&&g_pSprite[BLIP_DYING_SPRITE]->load(&g_cBlipSprites,0,1,34);
	result=result&&g_pSprite[BLIP_DYING_SPRITE]->load(&g_cBlipSprites,1,34,34);
	result=result&&g_pSprite[BLIP_DYING_SPRITE]->load(&g_cBlipSprites,2,67,34);
	return result;
}


BOOL LoadGroundCannonSprite()
{
	BOOL result=TRUE;
	result=result&&g_pSprite[GROUND_CANNON_SPRITE]->load(&g_cGroundCannonSprites,0,3,3);
	return result;
}


BOOL LoadImages()
{
	if(!background.load("bottom_grey.bmp")) return FALSE;
	background.draw(lpBackground);
	if(!background.setpalette(lpPrimaryPalette)) return FALSE;
	if(!background.setpalette(lpSecondaryPalette)) return FALSE;

	if(!g_cShipSprites.load("ship_sprites.bmp")) return FALSE;
	if(!g_cBlipSprites.load("blip_sprites.bmp")) return FALSE;
	if(!g_cGroundCannonSprites.load("ground_cannon_sprites.bmp")) return FALSE;

	g_pSprite[SHIP_SPRITE]=new CClippedSprite(2,64,32);
	g_pSprite[SHIP_UP_SPRITE]=new CClippedSprite(2,64,32);
	g_pSprite[SHIP_DOWN_SPRITE]=new CClippedSprite(2,64,32);
	g_pSprite[SHIP_DYING_SPRITE]=new CClippedSprite(4,64,32);
	if(!LoadShipSprite()) return FALSE;
	
	g_pSprite[BLIP_SPRITE]=new CClippedSprite(4,32,32);
	g_pSprite[BLIP_DYING_SPRITE]=new CClippedSprite(3,32,32);
	if(!LoadBlipSprite()) return FALSE;
	
	g_pSprite[GROUND_CANNON_SPRITE]=new CClippedSprite(1,36,32);
	if(!LoadGroundCannonSprite()) return FALSE;

	return TRUE;
}


void CreateObjects()
{
	Ship.Create(300,230);	
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
	if(g_pSprite[SHIP_SPRITE]->Restore())
		result=result&&LoadShipSprite();
	else return FALSE;
	if(g_pSprite[SHIP_UP_SPRITE]->Restore())
		result=result&&LoadShipSprite();
	else return FALSE;
	if(g_pSprite[SHIP_DOWN_SPRITE]->Restore())
		result=result&&LoadShipSprite();
	else return FALSE;
	if(g_pSprite[SHIP_DYING_SPRITE]->Restore())
		result=result&&LoadShipSprite();
	else return FALSE;
	if(g_pSprite[BLIP_SPRITE]->Restore())
		result=result&&LoadBlipSprite();
	else return FALSE;
	if(g_pSprite[BLIP_DYING_SPRITE]->Restore())
		result=result&&LoadBlipSprite();
	else return FALSE;
	if(g_pSprite[GROUND_CANNON_SPRITE]->Restore())
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
		//fps
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"average fps=", strlen("average fps="));
		TextOut(hdc,130,y,int2char(nFPS, cint),
			strlen(int2char(nFPS, cint)));

		//frames drawn
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"frames drawn=", strlen("frames drawn="));
		TextOut(hdc,130,y,int2char(nFramesDrawn, cint),
			strlen(int2char(nFramesDrawn, cint)));
		
		//viewpoint pos
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"viewpoint pos=", strlen("viewpoint pos="));
		TextOut(hdc,130,y,int2char(Viewpoint.m_nX-320, cint),
			strlen(int2char(Viewpoint.m_nX-320, cint)));
		
		//enemy count
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"enemy count=", strlen("enemy count="));
		TextOut(hdc,130,y,int2char(ObjectManager.GetCount(), cint),
			strlen(int2char(ObjectManager.GetCount(), cint)));

		//ship direction
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship direction=", strlen("ship direction="));
		TextOut(hdc,130,y,int2char(Ship.GetDirection(), cint),
			strlen(int2char(Ship.GetDirection(), cint)));

		//fire objects
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"fire objects=", strlen("fire objects="));
		TextOut(hdc,130,y,int2char(FireMan.GetCount(), cint),
			strlen(int2char(FireMan.GetCount(), cint)));

//		//blip x
//		y=0+(line_count++ * 17);
//		TextOut(hdc,0,y,"blip x=", strlen("blip x="));
//		TextOut(hdc,130,y,int2char(ObjectManager.ObjList[0]->GetX(), cint),
//			strlen(int2char(ObjectManager.ObjList[0]->GetX(), cint)));

//		//blip y
//		y=0+(line_count++ * 17);
//		TextOut(hdc,0,y,"blip y=", strlen("blip y="));
//		TextOut(hdc,130,y,int2char(ObjectManager.ObjList[0]->GetY(), cint),
//			strlen(int2char(ObjectManager.ObjList[0]->GetY(), cint)));

		//ship x
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship x=", strlen("ship x="));
		TextOut(hdc,130,y,int2char(Ship.GetX(), cint),
			strlen(int2char(Ship.GetX(), cint)));

		//ship y
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship y=", strlen("ship y="));
		TextOut(hdc,130,y,int2char(Ship.GetY(), cint),
			strlen(int2char(Ship.GetY(), cint)));

		


		
		surface->ReleaseDC(hdc);
	}
}


BOOL KeyboardHandler()
{
	//set ship direction based on keypad arrow state
	if(KeyDown(keystate, DIK_DOWN)&&KeyDown(keystate, DIK_LEFT))
		Ship.SetDirection(1);
	else if(KeyDown(keystate, DIK_DOWN)&&KeyDown(keystate, DIK_RIGHT))
		Ship.SetDirection(3);
	else if(KeyDown(keystate, DIK_UP)&&KeyDown(keystate, DIK_LEFT))
		Ship.SetDirection(7);
	else if(KeyDown(keystate, DIK_UP)&&KeyDown(keystate, DIK_RIGHT))
		Ship.SetDirection(9);
	else if(KeyDown(keystate, DIK_DOWN))
		Ship.SetDirection(2);
	else if(KeyDown(keystate, DIK_LEFT))
		Ship.SetDirection(4);
	else if(KeyDown(keystate, DIK_RIGHT))
		Ship.SetDirection(6);
	else if(KeyDown(keystate, DIK_UP))
		Ship.SetDirection(8);
	else
		Ship.SetDirection(0);

	if(KeyDown(keystate, DIK_ESCAPE))
		return TRUE;

	return FALSE;
}


BOOL ComposeFrame()
{
	//increment frame counter, calculate fps
	nFramesDrawn++;
	nFPS=int(nFramesDrawn/(Timer.time()/1000));
	
	//get the current keystate at beginning of frame draw
	GetKeystate();

	//process keyboard input
	KeyboardHandler();

	//poll timer sequence and create new objects/events as necessary
	Timer.PollSeqList();
	
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
	//set to 959 for right edge viewing
	Viewpoint.set_position(540);

	//draw background at given speed
	Viewpoint.draw_background(lpBackground, lpSecondary, -7);

	//move and draw ship object
	Ship.Move();
	Ship.Draw(lpSecondary);
	
	//move and draw each object
	ObjectManager.MoveAll();
	ObjectManager.DrawAll(lpSecondary);

	//lock surface and draw foreground plot pixels
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
    {
		//draw any needed foreground pixels using ddsd reference
		FireMan.MoveAll();
		FireMan.DrawAll(&ddsd);
		
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


long CALLBACK WindowProc(HWND hwnd,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP: ActiveApp=wParam; break;
		case WM_CREATE:	break;
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					break;
				case VK_SPACE:
					FireMan.Create(SHIP_LASER_1, Ship.GetX(), Ship.GetY(), 800, Ship.GetY());
					break;
			}
			
			
			break;
		case WM_DESTROY:
			//kill direct draw objects
			if(lpDirectDrawObject!=NULL)
			{
				if(lpSecondary!=NULL)
					lpSecondary->Release();
				if(lpPrimary!=NULL)
					lpPrimary->Release();
				if(lpBackground!=NULL)
					lpBackground->Release();
				for(int i=0; i<SPRITE_COUNT; i++)
				{
					if(g_pSprite[i])
						g_pSprite[i]->Release();
					delete g_pSprite[i];
				}
				lpDirectDrawObject->Release();
			}
			//kill direct input objects
			if(mKeyboard)
			{
				mKeyboard->Unacquire();
				mKeyboard->Release();
			}
			if (lpDirectInputObject)
				lpDirectInputObject->Release();
			//return state			
			ShowCursor(TRUE);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd,message,wParam,lParam);
			break;
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

	for(int i=0; i<SPRITE_COUNT; i++)
		g_pSprite[i]=NULL;
	
	BOOL OK=InitDirectDraw(hwnd);
	if(OK)OK=LoadImages();
	if(OK)OK=InitDirectInput(hwnd);
	
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