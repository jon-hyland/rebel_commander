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
#include "fireman.h"
#include "sound.h"
#include "alpha.h"

#define DEBUG_OUT TRUE

//definition of level event item
struct LevelEventItem
{
	DWORD nLevelTime;

	BOOL bInstObject;
	ObjectType enObjectType;
	SpriteType enObjectSprite;
	SpriteType enObjectDyingSprite;
	int iObjectX;
	int iObjectY;
	ObjEventId enObjectEventId;
};

//definition of keystate struct
struct KeystateStruct
{
	BOOL	Up;
	DWORD	UpLastTime;
	int		UpDelay;
	BOOL Down;
	DWORD	DownLastTime;
	int		DownDelay;
	BOOL Left;
	DWORD	LeftLastTime;
	int		LeftDelay;
	BOOL Right;
	DWORD	RightLastTime;
	int		RightDelay;
	BOOL Space;
	DWORD	SpaceLastTime;
	int		SpaceDelay;
	BOOL Esc;
	DWORD	EscLastTime;
	int		EscDelay;
	BOOL Enter;
	DWORD	EnterLastTime;
	int		EnterDelay;
};


//globals
HWND hwnd;
BOOL ActiveApp;
int GameState=-2;
LPDIRECTDRAW lpDirectDrawObject=NULL;
LPDIRECTDRAWSURFACE lpPrimary=NULL;
LPDIRECTDRAWPALETTE lpPalette;
LPDIRECTDRAWSURFACE lpSecondary=NULL;
LPDIRECTDRAWSURFACE lpBackground=NULL;
LPDIRECTINPUT8 lpDirectInputObject=NULL;
LPDIRECTINPUTDEVICE8 Keyboard;
KeystateStruct Keystate;
CTimer Timer;
CStars stars(STAR_COUNT);
CBmpFileReader background;
CBmpSpriteFileReader g_cRedAlphaSprites;
CBmpSpriteFileReader g_cBlueAlphaSprites;
CBmpSpriteFileReader g_cShipSprites;
CBmpSpriteFileReader g_cBlipSprites;
CBmpSpriteFileReader g_cGroundCannonSprites;
CSoundManager *SoundManager;
CAlpha Alpha;
CShip Ship;
CObjectManager ObjectManager;
CFireManager FireMan;
CClippedSprite *g_pSprite[SPRITE_COUNT];
CViewPoint Viewpoint;
CRandom Random;
LevelEventItem **LevelEventDefs[MAX_LEVEL_EVENTS];
int CurrentLevelEvent;
DWORD LevelStartTime;
int CurrentLevel=0;
int MenuSelection=0;
int LastGameState=0;
DWORD LastGameTime;
double nFramesDrawn=0;
int nFPS;

//external function prototypes:
//ddsetup.cpp
LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface);
BOOL InitDirectDraw();
HWND CreateDefaultWindow(char *name, HINSTANCE hInstance);
//keys.cpp
void ReleaseDirectInput();
void InitKeyboard();
void ResetKeystateTimes();
void GetKeystate();
void HandleKeystate();
//loadall.cpp
void LoadSounds();
BOOL LoadGraphics();
BOOL RestoreSurfaces();
//logo.cpp
void ResetLogo();
void ComposeLogoFrame();
//title.cpp
void ResetTitle();
void ComposeTitleFrame();
//menu.cpp
void ReenterMenu();
void ReexitMenu();
void ComposeMenuFrame();
//levels.cpp
void StartLevelOne();
void StartLevelTwo();
void StartLevelThree();
void PollLevelEventTime();
void DefineLevelEvents();
void MoveLevelObjects();
void ComposeLevelFrame();
//debugout.cpp
void DebugOut(LPDIRECTDRAWSURFACE surface);


//compose frame callback loop
BOOL ComposeFrame()
{
	switch(GameState)
	{
		case LOGO_SCREEN:
			ComposeLogoFrame();
			break;  //LOGO_SCREEN
		case TITLE_SCREEN:
			ComposeTitleFrame();
			break;	//TITLE_SCREEN;
		case MAIN_MENU:
			ComposeMenuFrame();
			break;	//MAIN_MENU
		case LEVEL_ONE: case LEVEL_TWO: case LEVEL_THREE:
			MoveLevelObjects();
			ComposeLevelFrame();
			break;	//LEVEL_ONE
	}

	//temp code to draw debug info to screen	
	if(DEBUG_OUT) DebugOut(lpSecondary);
	

	//perform a page flip
	if(lpPrimary->Flip(NULL,DDFLIP_WAIT)==DDERR_SURFACELOST)
		return RestoreSurfaces();
	
	return TRUE;
}

	




//program callback loop
long CALLBACK WindowProc(HWND hwnd,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP: ActiveApp=wParam; break;
		case WM_CREATE:	break;
		case WM_ACTIVATE:
			if(WA_INACTIVE!=wParam&&Keyboard)
			{
				//make sure keyboard is acquired, if gaining focus
				Keyboard->Acquire();
			}
			break;
		case WM_TIMER:
			GetKeystate();
			HandleKeystate();		
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
			//kill direct sound
			delete SoundManager;		
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



//program intry (main funciton)
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL OK;
	hwnd=CreateDefaultWindow("Rebel Commander",hInstance);
	if(!hwnd)return FALSE;
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(FALSE);

	for(int i=0; i<SPRITE_COUNT; i++)
		g_pSprite[i]=NULL;
	
	Timer.Start();
	
	OK=InitDirectDraw();

	if(OK)OK=LoadGraphics();
	InitKeyboard();
	
	if(!OK){
		DestroyWindow(hwnd);
		return FALSE;
	}

	SoundManager=new CSoundManager(hwnd);
	LoadSounds();

	DefineLevelEvents();

	ResetLogo();
	ResetTitle();
	
	while(TRUE)
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if(!GetMessage(&msg,NULL,0,0)) return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if(ActiveApp)ComposeFrame(); else WaitMessage();
}