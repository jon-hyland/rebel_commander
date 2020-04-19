#include <windows.h>
#include <windowsx.h>
#include "defines.h"
#include "timer.h"
#include "ship.h"
#include "objman.h"
#include "alpha.h"
#include "stars.h"
#include "view.h"
#include "fireman.h"
#include "plot.h"

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

extern int GameState;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern LPDIRECTDRAWSURFACE lpBackground;
extern LevelEventItem **LevelEventDefs[MAX_LEVEL_EVENTS];
extern int CurrentLevelEvent;
extern DWORD LevelStartTime;
extern int CurrentLevel;
extern CTimer Timer;
extern CShip Ship;
extern CObjectManager ObjectManager;
extern CSoundManager *SoundManager;
extern CAlpha Alpha;
extern double nFramesDrawn;
extern int nFPS;
extern CStars stars;
extern CViewPoint Viewpoint;
extern CFireManager FireMan;

void StartLevelOne()
{
	GameState=LEVEL_ONE;
	CurrentLevel=1;
	CurrentLevelEvent=0;
	Ship.Create(300,230);	
	LevelStartTime=Timer.GameTime();
}

void StartLevelTwo()
{
	CurrentLevelEvent=0;
	CurrentLevel=2;
	GameState=LEVEL_TWO;
	Ship.Create(300,230);	
	LevelStartTime=Timer.GameTime();
}

void StartLevelThree()
{
	CurrentLevelEvent=0;
	CurrentLevel=3;
	GameState=LEVEL_THREE;
	Ship.Create(300,230);	
	LevelStartTime=Timer.GameTime();
}


void PollLevelEventTime()
{
	if(CurrentLevelEvent<MAX_LEVEL_EVENTS) 
		if(Timer.GameTime()>=LevelStartTime+LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->nLevelTime)
		{
			if(LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->bInstObject)
			{
				ObjectManager.Create(LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->enObjectType, 
					LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->enObjectSprite,
					LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->enObjectDyingSprite,
					LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->iObjectX, 
					LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->iObjectY, 
					LevelEventDefs[CurrentLevel-1][CurrentLevelEvent]->enObjectEventId);
			
				CurrentLevelEvent++;			
			}
		}
}




void DefineLevelEvents()
{
	for(int i=0; i<LEVEL_COUNT; i++)
	{
		LevelEventDefs[i]=new LevelEventItem*[MAX_LEVEL_EVENTS];
		for(int ii=0; ii<MAX_LEVEL_EVENTS; ii++)
			LevelEventDefs[i][ii]=new LevelEventItem;
	}

	LevelEventDefs[LEVEL_ONE-1][0]->nLevelTime=2500;
	LevelEventDefs[LEVEL_ONE-1][0]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][0]->enObjectType=BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][0]->enObjectSprite=BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][0]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][0]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][0]->iObjectY=220;
	LevelEventDefs[LEVEL_ONE-1][0]->enObjectEventId=BLIP_DEFAULT_ACTION;


	LevelEventDefs[LEVEL_ONE-1][1]->nLevelTime=2700;
	LevelEventDefs[LEVEL_ONE-1][1]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][1]->enObjectType=BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][1]->enObjectSprite=BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][1]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][1]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][1]->iObjectY=220;
	LevelEventDefs[LEVEL_ONE-1][1]->enObjectEventId=BLIP_DEFAULT_ACTION;

	LevelEventDefs[LEVEL_ONE-1][2]->nLevelTime=2900;
	LevelEventDefs[LEVEL_ONE-1][2]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][2]->enObjectType=BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][2]->enObjectSprite=BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][2]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][2]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][2]->iObjectY=220;
	LevelEventDefs[LEVEL_ONE-1][2]->enObjectEventId=BLIP_DEFAULT_ACTION;

	LevelEventDefs[LEVEL_ONE-1][3]->nLevelTime=3100;
	LevelEventDefs[LEVEL_ONE-1][3]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][3]->enObjectType=BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][3]->enObjectSprite=BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][3]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][3]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][3]->iObjectY=220;
	LevelEventDefs[LEVEL_ONE-1][3]->enObjectEventId=BLIP_DEFAULT_ACTION;

	LevelEventDefs[LEVEL_ONE-1][4]->nLevelTime=3300;
	LevelEventDefs[LEVEL_ONE-1][4]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][4]->enObjectType=BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][4]->enObjectSprite=BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][4]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][4]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][4]->iObjectY=220;
	LevelEventDefs[LEVEL_ONE-1][4]->enObjectEventId=BLIP_DEFAULT_ACTION;

}

void MoveLevelObjects()
{
	//poll event seq and create new objects/events as necessary
	PollLevelEventTime();

	//move stars
	stars.Move();

	//move ship
	Ship.Move();

	//move each enemy object
	ObjectManager.MoveAll();

	//move fire objects
	FireMan.MoveAll();
}


void ComposeLevelFrame()
{
	//increment frame counter, calculate fps
	nFramesDrawn++;
	nFPS=int(nFramesDrawn/(Timer.GameTime()/1000));
	//clear the draw surface (when using no bmp'd background)
	DDBLTFX ddbltfx;
	ddbltfx.dwSize=sizeof(DDBLTFX);
	ddbltfx.dwFillColor=0;
	HRESULT ddrval=lpSecondary->Blt(NULL, NULL, NULL,DDBLT_WAIT |
	DDBLT_COLORFILL, &ddbltfx);
	//lock surface and draw background plot pixels
	DDSURFACEDESC ddsd;
	memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
	{
		//draw any needed background pixels using ddsd reference
		stars.Draw(&ddsd);
		IDirectDrawSurface_Unlock(lpSecondary, NULL);
	}	
	//set viewpoint position to 540 (220+320)
	//set to 959 for right edge viewing
	Viewpoint.set_position(540);
	//draw background at given speed
	Viewpoint.draw_background(lpBackground, lpSecondary, -7);
	//lock surface and determine ship bottom boundary
	if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
	{
		int boundary=479;
		for(int y=479; y>=400; y--)
		{
			if(GetPx(Ship.GetX(),y,&ddsd)!=0)
				boundary=y;
		}
		Ship.SetBottomBoundary(boundary-1);

		IDirectDrawSurface_Unlock(lpSecondary, NULL);
	}	
	//draw ship object
	Ship.Draw(lpSecondary);
	//draw each object
	ObjectManager.DrawAll(lpSecondary);
	//lock surface and draw foreground plot pixels
	//memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	//ddsd.dwSize = sizeof(DDSURFACEDESC);
	if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
	{
		//draw any needed foreground pixels using ddsd reference
		FireMan.DrawAll(&ddsd);
		
		IDirectDrawSurface_Unlock(lpSecondary, NULL);
	}	
}