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

extern int GameState;
extern LPDIRECTDRAWPALETTE lpPalette;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern CSoundManager *SoundManager;
extern CTimer Timer;
extern CRandom Random;
extern CAlpha Alpha;
extern CViewPoint Viewpoint;
extern double nFramesDrawn;
extern int nFPS;

//extern void GetKeystate();
//extern KeyboardHandler();
extern void StartTitle();

#define LOGO_CHAR_COUNT 31
#define LOGO_WAIT_BREAK 2400

DWORD nLogoStartTime;
DWORD nLogoTime;
char *cLogoText[LOGO_CHAR_COUNT];
int nLogoTextX[LOGO_CHAR_COUNT];
int nLogoTextY[LOGO_CHAR_COUNT];
float nLogoTextVectorX[LOGO_CHAR_COUNT];
float nLogoTextVectorY[LOGO_CHAR_COUNT];
int nLogoTextLastXMovedTime[LOGO_CHAR_COUNT];
int nLogoTextLastYMovedTime[LOGO_CHAR_COUNT];
BOOL bLogoTextDead[LOGO_CHAR_COUNT];

void StartLogo()
{
	GameState=LOGO_SCREEN;
	
	cLogoText[0]="L";
	cLogoText[1]="I";
	cLogoText[2]="T";
	cLogoText[3]="E";
	cLogoText[4]=" ";
	cLogoText[5]="B";
	cLogoText[6]="L";
	cLogoText[7]="U";
	cLogoText[8]="E";
	cLogoText[9]=" ";
	cLogoText[10]="E";
	cLogoText[11]="N";
	cLogoText[12]="T";
	cLogoText[13]="E";
	cLogoText[14]="R";
	cLogoText[15]="T";
	cLogoText[16]="A";
	cLogoText[17]="I";
	cLogoText[18]="N";
	cLogoText[19]="M";
	cLogoText[20]="E";
	cLogoText[21]="N";
	cLogoText[22]="T";
	cLogoText[23]="P";
	cLogoText[24]="R";
	cLogoText[25]="E";
	cLogoText[26]="S";
	cLogoText[27]="E";
	cLogoText[28]="N";
	cLogoText[29]="T";
	cLogoText[30]="S";

	for(int i=0; i<LOGO_CHAR_COUNT; i++)
	{
		if(i<23)
		{
			nLogoTextX[i]=364+(i*16);
			nLogoTextY[i]=215;
		}
		else
		{
			nLogoTextX[i]=484+((i-23)*16);
			nLogoTextY[i]=245;
		}
		nLogoTextVectorX[i]=float(Random.number(22,166)/10);
		if(Random.number(0,1)) nLogoTextVectorX[i]=-nLogoTextVectorX[i];
		nLogoTextVectorY[i]=float(Random.number(22,166)/10);
		if(Random.number(0,1)) nLogoTextVectorY[i]=-nLogoTextVectorY[i];
		nLogoTextLastXMovedTime[i]=LOGO_WAIT_BREAK;
		nLogoTextLastYMovedTime[i]=LOGO_WAIT_BREAK;
		bLogoTextDead[i]=FALSE;
	}
	nLogoStartTime=Timer.GameTime();
	nLogoTime=Timer.GameTime()-nLogoStartTime;
	SoundManager->Play(ALIEN_INTRO_SOUND);
}

void ComposeLogoFrame()
{
	const int XSCALE=16;
	const int YSCALE=16;
	int xdelta, ydelta;
	int tfactor;
	int nCount=0;

	//increment frame counter, calculate fps
	nFramesDrawn++;
	nFPS=int(nFramesDrawn/(Timer.GameTime()/1000));
	
	//clear the draw surface (when using no bmp'd background)
	DDBLTFX ddbltfx;
	ddbltfx.dwSize=sizeof(DDBLTFX);
	ddbltfx.dwFillColor=0;
	HRESULT ddrval=lpSecondary->Blt(NULL,NULL,NULL,DDBLT_WAIT|DDBLT_COLORFILL,&ddbltfx);

	//set viewpoint position to 540 (220+320)
	//set to 959 for right edge viewing
	Viewpoint.set_position(540);

	//for each letter, blit it
	for(int i=0; i<LOGO_CHAR_COUNT; i++)
		if(!bLogoTextDead[i])
			Alpha.Write(lpSecondary, nLogoTextX[i], nLogoTextY[i], cLogoText[i], BLUE_ALPHA_SPRITE);
	
	//animate letters	
	nLogoTime=Timer.GameTime()-nLogoStartTime;
	if(nLogoTime>LOGO_WAIT_BREAK)
	{
		for(i=0; i<LOGO_CHAR_COUNT; i++)
		{
			//move letter relative to time and vector
			tfactor=nLogoTime-nLogoTextLastXMovedTime[i];
			xdelta=int((nLogoTextVectorX[i]*tfactor)/XSCALE);
			nLogoTextX[i]+=xdelta;
			if(xdelta||nLogoTextVectorX[i]==0)
				nLogoTextLastXMovedTime[i]=nLogoTime;

			tfactor=nLogoTime-nLogoTextLastYMovedTime[i];
			ydelta=int((nLogoTextVectorY[i]*tfactor)/YSCALE);
			nLogoTextY[i]+=ydelta;
			if(ydelta||nLogoTextVectorY[i]==0)
				nLogoTextLastYMovedTime[i]=nLogoTime;

			//accerate letter based on current vector
			nLogoTextVectorX[i]=nLogoTextVectorX[i]+(nLogoTextVectorX[i]/33);
			nLogoTextVectorY[i]=nLogoTextVectorY[i]+(nLogoTextVectorY[i]/33);

			//check if its off screen
			if((nLogoTextX[i]<200)||(nLogoTextX[i]>880)||(nLogoTextY[i]<-20)||(nLogoTextY[i]>500))
				bLogoTextDead[i]=TRUE;
			else
				nCount++;
		}
		if(nCount<=0)
		{
			GameState++;
			StartTitle();
		}
	}
	
	//just in case
	if(nLogoTime>10000)
	{
		GameState++;
		StartTitle();
	}
}


