#include <windows.h>
#include <windowsx.h>
#include "defines.h"
#include "timer.h"
#include "ship.h"
#include "objman.h"
#include "alpha.h"
#include "Stars.h"
#include "view.h"
#include "fireman.h"
#include "plot.h"
#include "pwrupman.h"

//definition of level event item
struct LevelEventItem
{
	DWORD nLevelTime;
	BOOL bDone;

	BOOL bInstObject;
	ObjectType enObjectType;
	SpriteType enObjectSprite;
	SpriteType enObjectDyingSprite;
	int iObjectX;
	int iObjectY;
	UpType enUpType;
	ObjAction enObjectEventId;
};

//external globals
extern int GameState;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern LPDIRECTDRAWSURFACE lpBackground;
extern LevelEventItem **LevelEventDefs[MAX_LEVEL_EVENTS];
extern DWORD LevelStartTime;
extern int CurrentLevel;
extern CTimer Timer;
extern CShip Ship;
extern CObjectManager ObjectManager;
extern CPowerUpManager UpManager;
extern CSoundManager *SoundManager;
extern CAlpha Alpha;
extern double nFramesDrawn;
extern int nFPS;
extern CStars Stars;
extern CViewPoint Viewpoint;
extern CFireManager FireMan;
extern int Lives;
extern BOOL GameOver;
extern DWORD GameOverTime;
extern double PlayerScore;
extern BOOL Invincible;
extern DWORD nGameTimeSnap;
extern int LevelEventsTotal;
extern int LevelEventsExecuted;


//external functions
extern void ResetGame();



//function prototypes (this page)
int DefineLevelOneEvents();

void StartLevelOne()
{
	SoundManager->Stop(ALIEN_INTRO_SOUND);
	GameState=LEVEL_ONE;
	CurrentLevel=1;
	ObjectManager.Reset();
	FireMan.Reset();
	UpManager.Reset();
	Ship.Create(300,230);	
	LevelStartTime=Timer.GameTime();
	LevelEventsTotal=DefineLevelOneEvents();
	LevelEventsExecuted=0;
	Invincible=FALSE;
	Lives=PLAYER_EXTRA_LIVES;
}

void StartLevelTwo()
{
	SoundManager->Stop(ALIEN_INTRO_SOUND);
	GameState=LEVEL_TWO;
	CurrentLevel=2;
	ObjectManager.Reset();
	FireMan.Reset();
	UpManager.Reset();
	Ship.Create(300,230);	
	LevelStartTime=Timer.GameTime();
	//LevelEventsTotal=DefineLevelTwoEvents();
	LevelEventsExecuted=0;
}

void StartLevelThree()
{
	SoundManager->Stop(ALIEN_INTRO_SOUND);
	GameState=LEVEL_THREE;
	CurrentLevel=3;
	ObjectManager.Reset();
	FireMan.Reset();
	UpManager.Reset();
	Ship.Create(300,230);	
	LevelStartTime=Timer.GameTime();
	//LevelEventsTotal=DefineLevelThreeEvents();
	LevelEventsExecuted=0;
}

void PollLevelEventTime()
{
	DWORD time=Timer.GameTime();

	//check if time is up for any in array
	for(int i=0; i<MAX_LEVEL_EVENTS; i++)
		if((!LevelEventDefs[CurrentLevel-1][i]->bDone)&&(time>=LevelEventDefs[CurrentLevel-1][i]->nLevelTime))
		{
			if(LevelEventDefs[CurrentLevel-1][i]->bInstObject)
			{
				ObjectManager.Create(LevelEventDefs[CurrentLevel-1][i]->enObjectType, 
					LevelEventDefs[CurrentLevel-1][i]->enObjectSprite,
					LevelEventDefs[CurrentLevel-1][i]->enObjectDyingSprite,
					LevelEventDefs[CurrentLevel-1][i]->enUpType,
					LevelEventDefs[CurrentLevel-1][i]->iObjectX, 
					LevelEventDefs[CurrentLevel-1][i]->iObjectY, 
					LevelEventDefs[CurrentLevel-1][i]->enObjectEventId);
			}
			LevelEventDefs[CurrentLevel-1][i]->bDone=TRUE;
			LevelEventsExecuted++;
		}
	//reset level events if sequence is done and none have been created this poll
	if(LevelEventsExecuted>=LevelEventsTotal)
	{
		switch(GameState)
		{
			case LEVEL_ONE:
				//play reset sound
				SoundManager->Play(SELECT_SOUND);
				LevelStartTime=Timer.GameTime();
				LevelEventsTotal=DefineLevelOneEvents();
				LevelEventsExecuted=0;
				break;
			case LEVEL_TWO:
				//LevelEventsTotal=DefineLevelTwoEvents();
				break;
			case LEVEL_THREE:
				//LevelEventsTotal=DefineLevelTwoEvents();
				break;
		}
	}
}

void MoveLevelObjects()
{
	//poll event seq and create new objects/events as necessary
	PollLevelEventTime();

	//move stars
	Stars.Move();

	//move ship
	Ship.Move();

	//move each enemy object
	ObjectManager.MoveAll();

	//move fire objects
	FireMan.MoveAll();

	//if game over, reset game if wait time is up
	if((GameOver)&&(Timer.GameTime()>GameOverTime+GAME_OVER_WAIT_TIME))
		ResetGame();
}

void DrawGauges(DDSURFACEDESC *ddsd)
{
	int forcolor, backcolor;
	

	//set speed gauge colors
	switch(Ship.GetSpeedUpCount())
	{
		case 0:
			forcolor=253;
			backcolor=253;
			break;
		case 1:
			forcolor=111;
			backcolor=253;
			break;
		case 2:
			forcolor=135;
			backcolor=111;
			break;
		case 3:
			forcolor=123;
			backcolor=135;
			break;
		case 4:
			forcolor=185;
			backcolor=123;
			break;
		default:
			forcolor=185;
			backcolor=185;
			break;
	}
			
	//draw speed gauge
	for(int x=0; x<80; x++)
	{

		if(x<2)
		{
			PutPx(x+756,20,255,ddsd);
			PutPx(x+756,21,255,ddsd);
			PutPx(x+756,22,255,ddsd);
			PutPx(x+756,23,255,ddsd);
			PutPx(x+756,24,255,ddsd);
			PutPx(x+756,25,255,ddsd);
		}
		else if(x<4)
		{
			PutPx(x+756,18,255,ddsd);
			PutPx(x+756,19,255,ddsd);
			if(x>=int(Ship.GetSpeedUpPercentRemaining()*.74+1))
			{
				PutPx(x+756,20,backcolor,ddsd);
				PutPx(x+756,21,backcolor,ddsd);
				PutPx(x+756,22,backcolor,ddsd);
				PutPx(x+756,23,backcolor,ddsd);
				PutPx(x+756,24,backcolor,ddsd);
				PutPx(x+756,25,backcolor,ddsd);
			}
			else
			{
				PutPx(x+756,20,forcolor,ddsd);
				PutPx(x+756,21,forcolor,ddsd);
				PutPx(x+756,22,forcolor,ddsd);
				PutPx(x+756,23,forcolor,ddsd);
				PutPx(x+756,24,forcolor,ddsd);
				PutPx(x+756,25,forcolor,ddsd);
			}
			PutPx(x+756,26,255,ddsd);
			PutPx(x+756,27,255,ddsd);
		}
		else if(x<6)
		{
			PutPx(x+756,16,255,ddsd);
			PutPx(x+756,17,255,ddsd);
			if(x>=int(Ship.GetSpeedUpPercentRemaining()*.74+1))
			{
				PutPx(x+756,18,backcolor,ddsd);
				PutPx(x+756,19,backcolor,ddsd);
				PutPx(x+756,20,backcolor,ddsd);
				PutPx(x+756,21,backcolor,ddsd);
				PutPx(x+756,22,backcolor,ddsd);
				PutPx(x+756,23,backcolor,ddsd);
				PutPx(x+756,24,backcolor,ddsd);
				PutPx(x+756,25,backcolor,ddsd);
				PutPx(x+756,26,backcolor,ddsd);
				PutPx(x+756,27,backcolor,ddsd);
			}
			else
			{
				PutPx(x+756,18,forcolor,ddsd);
				PutPx(x+756,19,forcolor,ddsd);
				PutPx(x+756,20,forcolor,ddsd);
				PutPx(x+756,21,forcolor,ddsd);
				PutPx(x+756,22,forcolor,ddsd);
				PutPx(x+756,23,forcolor,ddsd);
				PutPx(x+756,24,forcolor,ddsd);
				PutPx(x+756,25,forcolor,ddsd);
				PutPx(x+756,26,forcolor,ddsd);
				PutPx(x+756,27,forcolor,ddsd);
			}
			PutPx(x+756,28,255,ddsd);
			PutPx(x+756,29,255,ddsd);
		}
		else if(x<74)
		{
			PutPx(x+756,16,255,ddsd);
			PutPx(x+756,17,255,ddsd);
			if(x>=int(Ship.GetSpeedUpPercentRemaining()*.74+1))
			{
				PutPx(x+756,18,backcolor,ddsd);
				PutPx(x+756,19,backcolor,ddsd);
				PutPx(x+756,20,backcolor,ddsd);
				PutPx(x+756,21,backcolor,ddsd);
				PutPx(x+756,22,backcolor,ddsd);
				PutPx(x+756,23,backcolor,ddsd);
				PutPx(x+756,24,backcolor,ddsd);
				PutPx(x+756,25,backcolor,ddsd);
				PutPx(x+756,26,backcolor,ddsd);
				PutPx(x+756,27,backcolor,ddsd);
			}
			else
			{
				PutPx(x+756,18,forcolor,ddsd);
				PutPx(x+756,19,forcolor,ddsd);
				PutPx(x+756,20,forcolor,ddsd);
				PutPx(x+756,21,forcolor,ddsd);
				PutPx(x+756,22,forcolor,ddsd);
				PutPx(x+756,23,forcolor,ddsd);
				PutPx(x+756,24,forcolor,ddsd);
				PutPx(x+756,25,forcolor,ddsd);
				PutPx(x+756,26,forcolor,ddsd);
				PutPx(x+756,27,forcolor,ddsd);
			}
			PutPx(x+756,28,255,ddsd);
			PutPx(x+756,29,255,ddsd);
			PutPx(x+756,30,174,ddsd);
			PutPx(x+756,31,174,ddsd);
		}
		else if(x<76)
		{
			PutPx(x+756,18,255,ddsd);
			PutPx(x+756,19,255,ddsd);
			if(x>=int(Ship.GetSpeedUpPercentRemaining()*.74+1))
			{
				PutPx(x+756,20,backcolor,ddsd);
				PutPx(x+756,21,backcolor,ddsd);
				PutPx(x+756,22,backcolor,ddsd);
				PutPx(x+756,23,backcolor,ddsd);
				PutPx(x+756,24,backcolor,ddsd);
				PutPx(x+756,25,backcolor,ddsd);
			}
			else
			{
				PutPx(x+756,20,forcolor,ddsd);
				PutPx(x+756,21,forcolor,ddsd);
				PutPx(x+756,22,forcolor,ddsd);
				PutPx(x+756,23,forcolor,ddsd);
				PutPx(x+756,24,forcolor,ddsd);
				PutPx(x+756,25,forcolor,ddsd);
			}
			PutPx(x+756,26,255,ddsd);
			PutPx(x+756,27,255,ddsd);
			PutPx(x+756,28,174,ddsd);
			PutPx(x+756,29,174,ddsd);
		}
		else if(x<78)
		{
			PutPx(x+756,20,255,ddsd);
			PutPx(x+756,21,255,ddsd);
			PutPx(x+756,22,255,ddsd);
			PutPx(x+756,23,255,ddsd);
			PutPx(x+756,24,255,ddsd);
			PutPx(x+756,25,255,ddsd);
			PutPx(x+756,26,174,ddsd);
			PutPx(x+756,27,174,ddsd);
		}
		else
		{
			PutPx(x+756,22,174,ddsd);
			PutPx(x+756,23,174,ddsd);
			PutPx(x+756,24,174,ddsd);
			PutPx(x+756,25,174,ddsd);
		}
	}
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

	//lock surface and PLOT background pixels
	DDSURFACEDESC ddsd;
	memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
	{
		//draw any needed background pixels using ddsd reference
		Stars.Draw(&ddsd);
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

	//draw enemy objects
	ObjectManager.DrawAll(lpSecondary);

	//draw power ups
	UpManager.DrawAll(lpSecondary);	
	
	//draw status bar
	Alpha.Write(lpSecondary,540,8,"SCORE   LIVES   KILLS   WEAPON  SPEED",BLUE_ALPHA_SPRITE);
	Alpha.Write(lpSecondary,292,24,int(PlayerScore),RED_ALPHA_SPRITE);
	Alpha.Write(lpSecondary,420,24,Lives,RED_ALPHA_SPRITE);
	Alpha.Write(lpSecondary,548,24,Ship.GetShipKills(),RED_ALPHA_SPRITE);
	switch(Ship.GetFireType())
	{
		case SHIP_LASER_ONE:
			Alpha.Write(lpSecondary,684,24,"1",RED_ALPHA_SPRITE);
			break;
	}
	//Alpha.Write(lpSecondary,804,24,Ship.GetSpeedUpPercentRemaining(),RED_ALPHA_SPRITE);

	//draw blit fire objects
	FireMan.DrawAllBlit(lpSecondary);
	
	//draw game over if game over
	if(GameOver)
		Alpha.Write(lpSecondary,540,220,"GAME OVER",BLUE_ALPHA_SPRITE);

	//draw time snap
	if(nGameTimeSnap)
		Alpha.Write(lpSecondary,540,220,nGameTimeSnap,RED_ALPHA_SPRITE);
	
	//lock surface and PLOT top level pixels
	if (IDirectDrawSurface_Lock(lpSecondary, NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK)
	{
		//draw plot fire objects
		FireMan.DrawAllPlot(&ddsd);

		//draw gauges
		DrawGauges(&ddsd);
		
		IDirectDrawSurface_Unlock(lpSecondary, NULL);
	}	
}



//defines level one events
int DefineLevelOneEvents()
{
	DWORD time=Timer.GameTime();

	for(int i=0; i<LEVEL_COUNT; i++)
	{
		LevelEventDefs[i]=new LevelEventItem*[MAX_LEVEL_EVENTS];
		for(int ii=0; ii<MAX_LEVEL_EVENTS; ii++)
		{
			LevelEventDefs[i][ii]=new LevelEventItem;
			LevelEventDefs[i][ii]->bDone=TRUE;
		}
	}

	//reset index counter
	i=0;
	
/*	//temp
	//spark comes in here
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=0;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=SPARK_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=SPARK_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SPARK_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++; */

/*	//temp
	//gold slith comes in here
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=1000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GOLD_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GOLD_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWELVE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++; */

/*	//temp
	//gold slith comes in here again
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=1000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GOLD_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GOLD_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++; */

	//GREEN_BLIP, PAIR OF TWO - OBJECT_ACTION_TWO, OBJECT_ACTION_THREE
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=2800;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWO;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=2800;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_FOUR;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=3000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWO;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=3000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_FOUR;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=3200;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWO;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=3200;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_FOUR;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=3400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWO;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=3400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_FOUR;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=SPEED_UP;	i++;


	//ORANGE_BLIP, FIRST SET - OBJECT_ACTION_ONE
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=12000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=12200;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=12400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=12600;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=12800;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//SPARK - OBJECT_ACTION_SEVEN
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=12800;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=SPARK_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=SPARK_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SPARK_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;


	//ORANGE_BLIP, SECOND SET - OBJECT_ACTION_ONE
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=14000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=14200;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=14400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=14600;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=14800;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ONE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//SPARK - OBJECT_ACTION_SEVEN
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=14800;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=SPARK_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=SPARK_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SPARK_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//BLUE_SLITH - OBJECT_ACTION_SIX - FIRES
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=21500;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=BLUE_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=BLUE_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SIX;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=21700;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=BLUE_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=BLUE_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SIX;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=21900;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=BLUE_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=BLUE_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SIX;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=LIFE_UP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=22100;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=BLUE_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=BLUE_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SIX;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=22300;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=BLUE_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=BLUE_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SIX;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//SPARK - OBJECT_ACTION_SEVEN
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=22300;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=SPARK_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=SPARK_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SPARK_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;
	
	//ORANGE_BLIP - OBJECT_ACTION_FIVE
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=28000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ELEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=28200;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ELEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=28400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ELEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=SPEED_UP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=28600;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_ELEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//SPARK - OBJECT_ACTION_SEVEN
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=28600;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=SPARK_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=SPARK_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SPARK_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//GREEN_BLIP - OBJECT_ACTION_THREE
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_THREE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33100;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_THREE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33200;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_THREE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=SPEED_UP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33300;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_THREE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GREEN_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GREEN_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=GREEN_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_THREE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//SPARK - OBJECT_ACTION_SEVEN
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=SPARK_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=SPARK_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SPARK_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_SEVEN;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//ORANGE_BLIP - OBJECT_ACTION_TWELVE
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33000;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWELVE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33200;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWELVE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33400;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWELVE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=SPEED_UP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=33600;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=ORANGE_BLIP_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=ORANGE_BLIP_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=ORANGE_BLIP_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_TWELVE;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	//GOLD_SLITH - OBJECT_ACTION_EIGHT
	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=36250;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GOLD_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GOLD_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_EIGHT;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=36500;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GOLD_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GOLD_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_EIGHT;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;

	LevelEventDefs[LEVEL_ONE-1][i]->nLevelTime=36750;
	LevelEventDefs[LEVEL_ONE-1][i]->bDone=FALSE;
	LevelEventDefs[LEVEL_ONE-1][i]->bInstObject=TRUE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectType=GOLD_SLITH_OBJECT;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectSprite=GOLD_SLITH_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectDyingSprite=SLITH_DYING_SPRITE;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectX=1255;
	LevelEventDefs[LEVEL_ONE-1][i]->iObjectY=200;
	LevelEventDefs[LEVEL_ONE-1][i]->enObjectEventId=OBJECT_ACTION_EIGHT;
	LevelEventDefs[LEVEL_ONE-1][i]->enUpType=NOUP;	i++;


	//bump each create time up (by game time)
	for(int ii=0; ii<MAX_LEVEL_EVENTS; ii++)
		LevelEventDefs[CurrentLevel-1][ii]->nLevelTime+=time;
	LevelEventsExecuted=0;
	
	//return count
	return i;
}
