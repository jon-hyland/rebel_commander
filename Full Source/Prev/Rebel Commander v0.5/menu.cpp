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

//extern void GetKeystate();
//extern KeyboardHandler();
extern void StartLevelOne();
extern void StartLevelTwo();
extern void StartLevelThree();
extern void ComposeLevelFrame();
extern void ResetKeystateTimes();


void ReenterMenu()
{
	SoundManager->Stop(ENGINE_LOOP_SOUND);
	SoundManager->Play(MENU_UP_SOUND);
	LastGameState=GameState;
	LastGameTime=Timer.GameTime();

	MenuSelection=0;
	GameState=MAIN_MENU;
}

void ReexitMenu()
{
	if(!Ship.Dead())
		SoundManager->Play(ENGINE_LOOP_SOUND,TRUE);
	Timer.SetGameTime(LastGameTime);
	GameState=LastGameState;
	ResetKeystateTimes();
}

void ComposeMenuFrame()
{
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
	if(MenuSelection==0) 
		if(LastGameState==0)
			Alpha.Write(lpSecondary,540,136,"start game",BLUE_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,136,"resume game",BLUE_ALPHA_SPRITE);
	else
		if(LastGameState==0)
			Alpha.Write(lpSecondary,540,136,"start game",RED_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,136,"resume game",RED_ALPHA_SPRITE);
	if(MenuSelection==1) 
		if(SoundManager->SoundOn())
			Alpha.Write(lpSecondary,540,174,"sound on",BLUE_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,174,"sound off",BLUE_ALPHA_SPRITE);
	else
		if(SoundManager->SoundOn())
			Alpha.Write(lpSecondary,540,174,"sound on",RED_ALPHA_SPRITE);
		else
			Alpha.Write(lpSecondary,540,174,"sound off",RED_ALPHA_SPRITE);
	if(MenuSelection==2) 
		Alpha.Write(lpSecondary,540,193,"control type",BLUE_ALPHA_SPRITE);
	else
		Alpha.Write(lpSecondary,540,193,"control type",RED_ALPHA_SPRITE);
	if(MenuSelection==3) 
		Alpha.Write(lpSecondary,540,231,"jump to level two",BLUE_ALPHA_SPRITE);
	else
		Alpha.Write(lpSecondary,540,231,"jump to level two",RED_ALPHA_SPRITE);
	if(MenuSelection==4) 
		Alpha.Write(lpSecondary,540,250,"jump to level three",BLUE_ALPHA_SPRITE);
	else
		Alpha.Write(lpSecondary,540,250,"jump to level three",RED_ALPHA_SPRITE);
	if(MenuSelection==5) 
		Alpha.Write(lpSecondary,540,288,"end game",BLUE_ALPHA_SPRITE);
	else
		Alpha.Write(lpSecondary,540,288,"end game",RED_ALPHA_SPRITE);

	
	
	
	
}