
#ifndef GUI_H
#define GUI_H

#include <windows.h>	// include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h>	// include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>		// directX includes
#define DSBCAPS_CTRLDEFAULT		DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>

#include "T3DLIB1.h"	// game library includes
#include "T3DLIB2.h"
#include "T3DLIB3.h"

#include "LinkedList.h"

const int CursorWidth = 32;
const int CursorHeight = 32;

const int MouseLeftButton = 0;

const int ButtonStateUnpressed = 0;
const int ButtonStatePressed = 1;

class GUIButton {
public:
	int left,top,right,bottom;
	int state;
	void (*onClick)(void);
	IDirectDraw4* lpdd;
	IDirectDrawSurface4* lpddsBackBuffer;
	IDirectDrawSurface4* lpddsButtonUp;
	IDirectDrawSurface4* lpddsButtonDn;
	GUIButton(int left, int top, int right, int bottom,
			  void (*onClick)(void),
			  IDirectDraw4* lpdd, IDirectDrawSurface4* lpddsBackBuffer,
			  char* buttonImg, char* buttonDnImg);
	~GUIButton();
	void draw();
};

class GUIWindow {
private:
	int left,top,right,bottom;
	int lastx,lasty;
	LinkedList<GUIButton*>* buttons;
	LPDIMOUSESTATE lpdiMouseState;
	IDirectDraw4* lpdd;
	IDirectDrawSurface4* cursor;
	IDirectDrawSurface4* lpddsBackBuffer;
	IDirectDrawSurface4* lpddsBackground;
	GUIButton* defaultCmd;
public:
	GUIWindow(IDirectDraw4* lpDirectDraw4, IDirectDrawSurface4* lpBackBuffer, LPDIMOUSESTATE lpMouseState,
			  int left, int top, int right, int bottom, char* backgroundFile, char* cursorFile);
	~GUIWindow();
	void addButton(int left, int top, int width, int height, int isDefault, void (*onClick)(void),
				   char* buttonUpImg, char* buttonDnImg);
	void update();
	void draw();
};

int LoadSurfaceImage(IDirectDrawSurface4* lpdds,int width,int height,BITMAP_FILE_PTR bitmap,int cx,int cy,int mode);

#endif //GUI_H


