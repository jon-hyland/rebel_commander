#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dsound.h>

#include "csprite.h"
#include "sound.h"
#include "alpha.h"
#include "timer.h"
#include "random.h"
#include "defines.h"
#include "ship.h"

extern int GameState;
extern LPDIRECTDRAWPALETTE lpPalette;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern CSoundManager *SoundManager;
extern CTimer Timer;
extern CRandom Random;
extern CAlpha Alpha;
extern CViewPoint Viewpoint;
extern CShip Ship;
extern int MenuSelection;
extern int LastGameState;
extern DWORD LastGameTime;
extern double nFramesDrawn;
extern int nFPS;

//extern void GetKeystate();
//extern KeyboardHandler();
extern void StartLevelOne();
extern void StartLevelTwo();
extern void StartLevelThree();
extern void ComposeLevelFrame();
extern void ResetInputStateTimes();
extern BOOL MenuActive;

void ReenterMenu()
{
	SoundManager->Stop(ENGINE_LOOP_SOUND);
	SoundManager->Play(MENU_UP_SOUND);
	LastGameState=GameState;
	LastGameTime=Timer.GameTime();
	MenuActive=TRUE;
	MenuSelection=0;
	GameState=MAIN_MENU;
}

void ReexitMenu()
{
	if(!Ship.Dead())
		SoundManager->Play(ENGINE_LOOP_SOUND,TRUE);
	Timer.SetGameTime(LastGameTime);
	GameState=LastGameState;
	MenuActive=FALSE;
	ResetInputStateTimes();
}

void ComposeMenuFrame()
{
	//increment frame counter, calculate fps
	nFramesDrawn++;
	nFPS=int(nFramesDrawn/(Timer.GameTime()/1000));
	
	if(LastGameState>0)
	{
		ComposeLevelFrame();
	}
	else
	{
		//clear the draw surface (when using no bmp'd background)
		DDBLTFX ddbltfx;
		ddbltfx.dwSize = sizeof(DDBLTFX);
		ddbltfx.dwFillColor = 0;
		HRESULT ddrval = lpSecondary->Blt(NULL, NULL, NULL,DDBLT_WAIT |
		DDBLT_COLORFILL, &ddbltfx);
	}

	//set viewpoint position to 540 (220+320)
	//set to 959 for right edge viewing
	Viewpoint.set_position(540);

	//draw menu letters, coloring based on selection
	//start/resume
	if(MenuSelection==0)
	{
		if(LastGameState==0)
			Alpha.Write(lpSecondary,540,126,"start game",BLUE_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,126,"resume game",BLUE_ALPHA_SPRITE);
	}
	else
	{
		if(LastGameState==0)
			Alpha.Write(lpSecondary,540,126,"start game",RED_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,126,"resume game",RED_ALPHA_SPRITE);
	}
	
	//reset
	if(MenuSelection==1) 
	{
		Alpha.Write(lpSecondary,540,145,"reset game",BLUE_ALPHA_SPRITE);
	}
	else
	{
		Alpha.Write(lpSecondary,540,145,"reset game",RED_ALPHA_SPRITE);
	}
	
	//sound on/off
	if(MenuSelection==2) 
	{
		if(SoundManager->SoundOn())
			Alpha.Write(lpSecondary,540,183,"sound on",BLUE_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,183,"sound off",BLUE_ALPHA_SPRITE);
	}
	else
	{
		if(SoundManager->SoundOn())
			Alpha.Write(lpSecondary,540,183,"sound on",RED_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,183,"sound off",RED_ALPHA_SPRITE);
	}

	//control type
	if(MenuSelection==3) 
	{
		Alpha.Write(lpSecondary,540,202,"control type",BLUE_ALPHA_SPRITE);
	}
	else
	{
		Alpha.Write(lpSecondary,540,202,"control type",RED_ALPHA_SPRITE);
	}

	//jump to level four
	if(MenuSelection==4) 
	{
		Alpha.Write(lpSecondary,540,240,"jump to level two",GRAY_ALPHA_SPRITE);
	}
	else
	{
		Alpha.Write(lpSecondary,540,240,"jump to level two",GRAY_ALPHA_SPRITE);
	}

	//jump to level three
	if(MenuSelection==5) 
	{
		Alpha.Write(lpSecondary,540,259,"jump to level three",GRAY_ALPHA_SPRITE);
	}
	else
	{
		Alpha.Write(lpSecondary,540,259,"jump to level three",GRAY_ALPHA_SPRITE);
	}
	
	//end game
	if(MenuSelection==6) 
	{
		Alpha.Write(lpSecondary,540,297,"end game",BLUE_ALPHA_SPRITE);
	}
	else
	{
		Alpha.Write(lpSecondary,540,297,"end game",RED_ALPHA_SPRITE);
	}

	
	
	
	
}