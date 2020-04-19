#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define DEBUG_OUT FALSE

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
#include "pwrupman.h"

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
	UpType enUpType;
	ObjAction enObjectEventId;
};

//definition of input state struct
struct InputStruct
{
	BOOL	Up;
	DWORD	UpLastTime;
	int		UpDelay;
	BOOL	Down;
	DWORD	DownLastTime;
	int		DownDelay;
	BOOL	Left;
	DWORD	LeftLastTime;
	int		LeftDelay;
	BOOL	Right;
	DWORD	RightLastTime;
	int		RightDelay;
	BOOL	Space;
	DWORD	SpaceLastTime;
	int		SpaceDelay;
	BOOL	Esc;
	DWORD	EscLastTime;
	int		EscDelay;
	BOOL	Enter;
	DWORD	EnterLastTime;
	int		EnterDelay;
	BOOL	I;
	DWORD	ILastTime;
	int		IDelay;
	BOOL	R;
	DWORD	RLastTime;
	int		RDelay;
	BOOL	T;
	DWORD	TLastTime;
	int		TDelay;
	BOOL	JoyA;
	DWORD	JoyALastTime;
	int		JoyADelay;
	BOOL	JoyB;
	DWORD	JoyBLastTime;
	int		JoyBDelay;
	BOOL	JoyX;
	DWORD	JoyXLastTime;
	int		JoyXDelay;
	BOOL	JoyY;
	DWORD	JoyYLastTime;
	int		JoyYDelay;
	BOOL	JoyUp;
	DWORD	JoyUpLastTime;
	int		JoyUpDelay;
	BOOL	JoyDown;
	DWORD	JoyDownLastTime;
	int		JoyDownDelay;
	BOOL	JoyLeft;
	DWORD	JoyLeftLastTime;
	int		JoyLeftDelay;
	BOOL	JoyRight;
	DWORD	JoyRightLastTime;
	int		JoyRightDelay;
};

//globals
HWND hwnd;
BOOL ActiveApp;
int GameState=LOGO_SCREEN;
LPDIRECTDRAW lpDirectDrawObject=NULL;
LPDIRECTDRAWSURFACE lpPrimary=NULL;
LPDIRECTDRAWPALETTE lpPalette;
LPDIRECTDRAWSURFACE lpSecondary=NULL;
LPDIRECTDRAWSURFACE lpBackground=NULL;
LPDIRECTINPUT8 lpDirectInputObject=NULL;
LPDIRECTINPUTDEVICE8 Keyboard;
LPDIRECTINPUTDEVICE8 Joystick;
InputStruct InputState;
CTimer Timer;
CStars stars(STAR_COUNT);
CBmpFileReader background;
CSoundManager *SoundManager;
CAlpha Alpha;
CShip Ship;
CObjectManager ObjectManager;
CFireManager FireMan;
CPowerUpManager UpManager;
CClippedSprite *g_pSprite[SPRITE_COUNT];
CViewPoint Viewpoint;
CRandom Random;
LevelEventItem **LevelEventDefs[MAX_LEVEL_EVENTS];
//int CurrentLevelEvent;
DWORD LevelStartTime;
int CurrentLevel=0;
int MenuSelection=0;
BOOL MenuActive=FALSE;
int LastGameState=0;
DWORD LastGameTime;
int Lives;
double PlayerScore=0;
BOOL Invincible=FALSE;
BOOL GameOver=FALSE;
DWORD GameOverTime;
double nFramesDrawn=0;
int nFPS;
DWORD nGameTimeSnap=0; //debug

//external function prototypes:
//ddsetup.cpp
LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface);
BOOL InitDirectDraw();
HWND CreateDefaultWindow(char *name, HINSTANCE hInstance);
//input.cpp
void ReleaseDirectInput();
void InitInput();
void ResetInputStateTimes();
void GetDInputState();
void HandleInputState();
//loadall.cpp
void LoadSounds();
BOOL LoadGraphics();
BOOL RestoreSurfaces();
//logo.cpp
void StartLogo();
void ComposeLogoFrame();
//title.cpp
void StartTitle();
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
int DefineLevelOneEvents();
void MoveLevelObjects();
void ComposeLevelFrame();
//debugout.cpp
void DebugOut(LPDIRECTDRAWSURFACE surface);


//resets the game
void ResetGame()
{
	//stop any sounds
	SoundManager->Stop();

	//reset some globals
	MenuSelection=0;
	LastGameState=0;
	LastGameTime=Timer.GameTime();
	PlayerScore=0;
	nFramesDrawn=0;
	GameOver=FALSE;

	//show logo and title
	StartLogo();
}

//makes sure palette shifts are not seen when exiting
void ClearScreenSurfaces()
{
	//clear the draw surface (when using no bmp'd background)*/
	DDBLTFX ddbltfx;
	ddbltfx.dwSize=sizeof(DDBLTFX);
	ddbltfx.dwFillColor=0;
	HRESULT ddrval=lpSecondary->Blt(NULL, NULL, NULL,DDBLT_WAIT |
	DDBLT_COLORFILL, &ddbltfx);
	//perform a page flip
	if(lpPrimary->Flip(NULL,DDFLIP_WAIT)==DDERR_SURFACELOST)
		RestoreSurfaces();
}

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
			if(WA_INACTIVE!=wParam&&Joystick)
			{
				//make sure joystick is acquired
				Joystick->Acquire();
			}
			break;
		case WM_TIMER:
			GetDInputState();
			HandleInputState();		
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

	Timer.Start();
	
	for(int i=0; i<SPRITE_COUNT; i++)
		g_pSprite[i]=NULL;
	
	OK=InitDirectDraw();

	if(OK)OK=LoadGraphics();
	InitInput();
	
	if(!OK){
		DestroyWindow(hwnd);
		return FALSE;
	}

	SoundManager=new CSoundManager(hwnd);
	LoadSounds();

	StartLogo();
	StartTitle();
	
	while(TRUE)
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if(!GetMessage(&msg,NULL,0,0)) return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if(ActiveApp)ComposeFrame(); else WaitMessage();
}