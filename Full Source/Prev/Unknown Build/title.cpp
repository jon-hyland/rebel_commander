#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dsound.h>

#include "csprite.h"
#include "sound.h"
#include "alpha.h"
#include "timer.h"
#include "random.h"

extern int GameState;
extern LPDIRECTDRAWPALETTE lpPalette;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern CSoundManager *SoundManager;
extern CTimer Timer;
extern CRandom Random;
extern CAlpha Alpha;
extern CViewPoint Viewpoint;
extern BOOL MenuActive;

//extern void GetKeystate();
//extern KeyboardHandler();

#define TITLE_CHAR_COUNT 30
#define TITLE_WAIT_BREAK 2800

DWORD nTitleStartTime;
DWORD nTitleTime;
char *cTitleText[TITLE_CHAR_COUNT];
int nTitleTextX[TITLE_CHAR_COUNT];
int nTitleTextY[TITLE_CHAR_COUNT];
float nTitleTextVectorX[TITLE_CHAR_COUNT];
float nTitleTextVectorY[TITLE_CHAR_COUNT];
int nTitleTextLastXMovedTime[TITLE_CHAR_COUNT];
int nTitleTextLastYMovedTime[TITLE_CHAR_COUNT];
BOOL bTitleTextDead[TITLE_CHAR_COUNT];
extern double nFramesDrawn;
extern int nFPS;

void StartTitle()
{
	cTitleText[0]="R";
	cTitleText[1]="E";
	cTitleText[2]="B";
	cTitleText[3]="E";
	cTitleText[4]="L";
	cTitleText[5]=" ";
	cTitleText[6]="C";
	cTitleText[7]="O";
	cTitleText[8]="M";
	cTitleText[9]="M";
	cTitleText[10]="A";
	cTitleText[11]="N";
	cTitleText[12]="D";
	cTitleText[13]="E";
	cTitleText[14]="R";
	cTitleText[15]=&PROGRAM_VERSION;
	cTitleText[16]="B";
	cTitleText[17]="Y";
	cTitleText[18]=" ";
	cTitleText[19]="J";
	cTitleText[20]="O";
	cTitleText[21]="H";
	cTitleText[22]="N";
	cTitleText[23]=" ";
	cTitleText[24]="H";
	cTitleText[25]="Y";
	cTitleText[26]="L";
	cTitleText[27]="A";
	cTitleText[28]="N";
	cTitleText[29]="D";

	for(int i=0; i<TITLE_CHAR_COUNT; i++)
	{
		if(i<15)
		{
			nTitleTextX[i]=428+(i*16);
			nTitleTextY[i]=210;
		}
		else if (i==15)
		{
			nTitleTextX[i]=815;
			nTitleTextY[i]=460;
		}
		else
		{
			nTitleTextX[i]=438+((i-16)*16);
			nTitleTextY[i]=236;
		}
		nTitleTextVectorX[i]=float(Random.number(22,166)/10);
		if(Random.number(0,1)) nTitleTextVectorX[i]=-nTitleTextVectorX[i];
		nTitleTextVectorY[i]=float(Random.number(22,166)/10);
		if(Random.number(0,1)) nTitleTextVectorY[i]=-nTitleTextVectorY[i];
		nTitleTextLastXMovedTime[i]=TITLE_WAIT_BREAK;
		nTitleTextLastYMovedTime[i]=TITLE_WAIT_BREAK;
		bTitleTextDead[i]=FALSE;
	}
	nTitleStartTime=Timer.GameTime();
	nTitleTime=Timer.GameTime()-nTitleStartTime;
}

void ComposeTitleFrame()
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
	ddbltfx.dwSize = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = 0;
	HRESULT ddrval = lpSecondary->Blt(NULL, NULL, NULL,DDBLT_WAIT |
	DDBLT_COLORFILL, &ddbltfx);

	//set viewpoint position to 540 (220+320)
	//set to 959 for right edge viewing
	Viewpoint.set_position(540);

	//for each letter, blit it
	for(int i=0; i<TITLE_CHAR_COUNT; i++)
		if(!bTitleTextDead[i])
		{
			if (i < 16)
				Alpha.Write(lpSecondary, nTitleTextX[i], nTitleTextY[i], cTitleText[i], RED_ALPHA_SPRITE);
			else
				Alpha.Write(lpSecondary, nTitleTextX[i], nTitleTextY[i], cTitleText[i], BLUE_ALPHA_SPRITE);
		}
	
	//animate letters	
	nTitleTime=Timer.GameTime()-nTitleStartTime;
	if(nTitleTime>TITLE_WAIT_BREAK)
	{
		for(i=0; i<TITLE_CHAR_COUNT; i++)
		{
			//move letter relative to time and vector
			tfactor=nTitleTime-nTitleTextLastXMovedTime[i];
			xdelta=int((nTitleTextVectorX[i]*tfactor)/XSCALE);
			nTitleTextX[i]+=xdelta;
			if(xdelta||nTitleTextVectorX[i]==0)
				nTitleTextLastXMovedTime[i]=nTitleTime;

			tfactor=nTitleTime-nTitleTextLastYMovedTime[i];
			ydelta=int((nTitleTextVectorY[i]*tfactor)/YSCALE);
			nTitleTextY[i]+=ydelta;
			if(ydelta||nTitleTextVectorY[i]==0)
				nTitleTextLastYMovedTime[i]=nTitleTime;

			//accerate letter based on current vector
			nTitleTextVectorX[i]=nTitleTextVectorX[i]+(nTitleTextVectorX[i]/33);
			nTitleTextVectorY[i]=nTitleTextVectorY[i]+(nTitleTextVectorY[i]/33);


			//check if its off screen
			if((nTitleTextX[i]<200)||(nTitleTextX[i]>880)||(nTitleTextY[i]<-20)||(nTitleTextY[i]>500))
				bTitleTextDead[i]=TRUE;
			else
				nCount++;
		}
		if(nCount<=0)
		{
			GameState++;
			MenuActive=TRUE;
		}
	}
	
	//just in case
	if(nTitleTime>10000)
	{
		GameState++;
		MenuActive=TRUE;
	}
}


