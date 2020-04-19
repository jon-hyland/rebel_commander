#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <ddraw.h>
#include <dinput.h>

#include "csprite.h"
#include "sound.h"
#include "ship.h"
#include "objman.h"
#include "fireman.h"
#include "timer.h"

extern int GameState;
extern CShip Ship;
extern CObjectManager ObjectManager;
extern CFireManager FireMan;
extern double nFramesDrawn;
extern int nFPS;
extern CTimer Timer;
extern DWORD nGameTimeSnap;
extern DWORD LevelStartTime;


//passes char pointer text through function
char *int2char(int number, char *text)
{
	//for(int i=0; i<128; i++)
	//	text[i] = NULL;
	sprintf (text, "%i", number);
	return text;
}

void DebugOut(LPDIRECTDRAWSURFACE surface)
{
	int line_count=0;
	HDC hdc;
	int y;
	char cint[64];

    if (surface->GetDC(&hdc)==DD_OK)
	{
		//game state
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"game state=", strlen("game state="));
		TextOut(hdc,130,y,int2char(GameState, cint),
			strlen(int2char(GameState, cint)));

		//fps
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"frames/sec=", strlen("frames/sec="));
		TextOut(hdc,130,y,int2char(nFPS, cint),
			strlen(int2char(nFPS, cint)));

		//frame count
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"frame count=", strlen("frame count="));
		TextOut(hdc,130,y,int2char(int(nFramesDrawn), cint),
			strlen(int2char(int(nFramesDrawn), cint)));
		
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

		//ship direction
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship dir=", strlen("ship dir="));
		TextOut(hdc,130,y,int2char(Ship.GetDirection(), cint),
			strlen(int2char(Ship.GetDirection(), cint)));

		//game time
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"game time=", strlen("game time="));
		TextOut(hdc,130,y,int2char(Timer.GameTime(), cint),
			strlen(int2char(Timer.GameTime(), cint)));

		//ship boundary
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"ship bounds=", strlen("ship bounds="));
		TextOut(hdc,130,y,int2char(Ship.GetBottomBoundary(), cint),
			strlen(int2char(Ship.GetBottomBoundary(), cint)));

		//new obj time
		y=0+(line_count++ * 17);
		TextOut(hdc,0,y,"new obj time=", strlen("new obj time="));
		TextOut(hdc,130,y,int2char(nGameTimeSnap-LevelStartTime-1750, cint),
			strlen(int2char(nGameTimeSnap-LevelStartTime-1600, cint)));

//		//soundman online
//		y=0+(line_count++ * 17);
//		TextOut(hdc,0,y,"sound online=", strlen("sound online="));
//		if(SoundManager->Operational())
//			TextOut(hdc,130,y,"yes", 3);
//		else
//			TextOut(hdc,130,y,"no", 2);

		surface->ReleaseDC(hdc);
	}
}
