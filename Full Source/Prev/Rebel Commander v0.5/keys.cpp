#include <windows.h>
#include <windowsx.h>
#include <dinput.h>

#include "defines.h"
#include "sound.h"
#include "ship.h"
#include "fireman.h"
#include "timer.h"

struct KeystateStruct
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
};

extern int GameState;
extern HWND hwnd;
extern LPDIRECTINPUT8 lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 Keyboard;
extern KeystateStruct Keystate;
extern CSoundManager *SoundManager;
extern CShip Ship;
extern CFireManager FireMan;
extern CTimer Timer;
extern int MenuSelection;
extern DWORD LastGameTime;
extern int LastGameState;

extern void ResetTitle();
extern void StartLevelOne();
extern void ReenterMenu();
extern void ReexitMenu();

void ReleaseDirectInput()
{
	//kill direct input objects
	if(Keyboard)
		Keyboard->Unacquire();
	SAFE_RELEASE(Keyboard);
	SAFE_RELEASE(lpDirectInputObject);

	//kill the timer if exists
	KillTimer(hwnd, 0);
}

void InitKeyboard()
{
	DWORD dwCoopFlags;
	DWORD time=Timer.GameTime();

	//relese any previous timer
	KillTimer(hwnd, 0);

	//set keyboard device params
	dwCoopFlags = DISCL_EXCLUSIVE|DISCL_FOREGROUND;

	//create direct input device
	if(FAILED(DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,IID_IDirectInput8,(VOID**)&lpDirectInputObject,NULL)))
	{ ReleaseDirectInput();	return;	}

	//create keyboard device
	if(FAILED(lpDirectInputObject->CreateDevice(GUID_SysKeyboard,&Keyboard,NULL)))
	{ ReleaseDirectInput();	return;	}

	//set 256 byte array
	if(FAILED(Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{ ReleaseDirectInput();	return;	}

	//set cooperation level
	if(FAILED(Keyboard->SetCooperativeLevel(hwnd,dwCoopFlags)))
	{ ReleaseDirectInput();	return;	}

	//acquire keyboard
	if(FAILED(Keyboard->Acquire()))
	{ ReleaseDirectInput();	return;	}

	//set keystate last times to current time
	Keystate.UpLastTime=time;
	Keystate.DownLastTime=time;
	Keystate.LeftLastTime=time;
	Keystate.RightLastTime=time;
	Keystate.SpaceLastTime=time;
	Keystate.EscLastTime=time;
	Keystate.EnterLastTime=time;
	
	//set timer to get keyboard state 100x per second
	//(timer will trigger main windows callback loop)
	SetTimer(hwnd,0,1000/100,NULL);
}

void SetKeystateDelays()
{
	//set min key delays based on game state
	switch(GameState)
	{
		case LOGO_SCREEN: case TITLE_SCREEN:
			Keystate.UpDelay=200;
			Keystate.DownDelay=200;
			Keystate.LeftDelay=200;
			Keystate.RightDelay=200;
			Keystate.SpaceDelay=200;
			Keystate.EscDelay=200;
			Keystate.EnterDelay=200;
			break;
		case MAIN_MENU:
			Keystate.UpDelay=160;
			Keystate.DownDelay=160;
			Keystate.LeftDelay=200;
			Keystate.RightDelay=200;
			Keystate.SpaceDelay=200;
			Keystate.EscDelay=200;
			Keystate.EnterDelay=200;
			break;
		case LEVEL_ONE: case LEVEL_TWO: case LEVEL_THREE:
			Keystate.UpDelay=0;
			Keystate.DownDelay=0;
			Keystate.LeftDelay=0;
			Keystate.RightDelay=0;
			Keystate.SpaceDelay=200;
			Keystate.EscDelay=200;
			Keystate.EnterDelay=200;
			break;
	}
}

void ResetKeystateTimes()
{
	Keystate.UpLastTime=0;
	Keystate.DownLastTime=0;
	Keystate.LeftLastTime=0;
	Keystate.RightLastTime=0;
	Keystate.SpaceLastTime=0;
	Keystate.EscLastTime=0;
	Keystate.EnterLastTime=0;
}

void GetKeystate()
{
	BYTE diks[256];	//direct input keystate buffer
	DWORD time=Timer.GameTime();

	//read the state of all keys
	ZeroMemory(&diks,sizeof(diks));	//clear it
	if(FAILED(Keyboard->GetDeviceState(sizeof(diks),&diks)))
	{ ReleaseDirectInput();	return;	}

	//reset keystate
	Keystate.Up=FALSE;
	Keystate.Down=FALSE;
	Keystate.Left=FALSE;
	Keystate.Right=FALSE;
	Keystate.Space=FALSE;
	Keystate.Esc=FALSE;
	Keystate.Enter=FALSE;

	//set key delays for game state
	SetKeystateDelays();	

	//set keystate based on buffer
	for(int i=0; i<256; i++)
	{
		if(diks[i]&0x80)
		{
			switch(i)
			{
				case 200:
					if(time>=Keystate.UpLastTime+Keystate.UpDelay)
					{
						Keystate.Up=TRUE;
						Keystate.UpLastTime=time;
					}
					break;
				case 208:	
					if(time>=Keystate.DownLastTime+Keystate.DownDelay)
					{
						Keystate.Down=TRUE;
						Keystate.DownLastTime=time;
					}
					break;
				case 203:	
					if(time>=Keystate.LeftLastTime+Keystate.LeftDelay)
					{
						Keystate.Left=TRUE; 
						Keystate.LeftLastTime=time;
					}
					break;
				case 205:	
					if(time>=Keystate.RightLastTime+Keystate.RightDelay)
					{
						Keystate.Right=TRUE; 
						Keystate.RightLastTime=time;
					}
					break;
				case 57:	
					if(time>=Keystate.SpaceLastTime+Keystate.SpaceDelay)
					{
						Keystate.Space=TRUE; 
						Keystate.SpaceLastTime=time;
					}
					break;
				case 1:		
					if(time>=Keystate.EscLastTime+Keystate.EscDelay)
					{
						Keystate.Esc=TRUE; 
						Keystate.EscLastTime=time;
					}
					break;
				case 28:	
					if(time>=Keystate.EnterLastTime+Keystate.EnterDelay)
					{
						Keystate.Enter=TRUE; 
						Keystate.EnterLastTime=time;
					}
					break;
			}
		
		}
		else //if key not pressed
		{
			switch(i)
			{
				case 200:
					if(Keystate.UpLastTime+Keystate.UpDelay>time)Keystate.UpLastTime-=Keystate.UpDelay;
					break;
				case 208:
					if(Keystate.DownLastTime+Keystate.DownDelay>time)Keystate.DownLastTime-=Keystate.DownDelay;
					break;
				case 203:
					if(Keystate.LeftLastTime+Keystate.LeftDelay>time)Keystate.LeftLastTime-=Keystate.LeftDelay;
					break;
				case 205:
					if(Keystate.RightLastTime+Keystate.RightDelay>time)Keystate.RightLastTime-=Keystate.RightDelay;
					break;
				case 57:
					if(Keystate.SpaceLastTime+Keystate.SpaceDelay>time)Keystate.SpaceLastTime-=Keystate.SpaceDelay;
					break;
				case 1:
					if(Keystate.EscLastTime+Keystate.EscDelay>time)Keystate.EscLastTime-=Keystate.EscDelay;
					break;
				case 28:
					if(Keystate.EnterLastTime+Keystate.EnterDelay>time)Keystate.EnterLastTime-=Keystate.EnterDelay;
					break;
			}
		}
	}
}

void HandleKeystate()
{
	//everything based on game state
	switch(GameState)
	{
		case LOGO_SCREEN:
			//if user hits key skip logo screen
			if((Keystate.Esc)||(Keystate.Enter)||(Keystate.Space))
			{
				GameState++;
				ResetTitle();
			}
			break;


		case TITLE_SCREEN:
			//if user hits key skip title screen
			if((Keystate.Esc)||(Keystate.Enter)||(Keystate.Space))
			{
				GameState++;
			}
			break;


		case MAIN_MENU:
			if(Keystate.Up)
			{
				SoundManager->Play(SELECT_SOUND);
				if(--MenuSelection<0)MenuSelection=5;
			}
			if(Keystate.Down)
			{
				SoundManager->Play(SELECT_SOUND);
				if(++MenuSelection>5)MenuSelection=0;
			}
			if((Keystate.Enter)||(Keystate.Space))
			{
				switch(MenuSelection)
				{
					case 0:
						SoundManager->Play(OK_SOUND);
						if(LastGameState==0)
						{
							StartLevelOne();
						}
						else
						{
							ReexitMenu();
						}
						break;
					case 1:
						if(SoundManager->SoundOn())
							SoundManager->TurnOff();
						else
							SoundManager->TurnOn();
						SoundManager->Play(OK_SOUND);
						break;
					case 2:
						SoundManager->Play(OK_SOUND);
						break;
					case 3:
						SoundManager->Play(OK_SOUND);
						break;
					case 4:
						SoundManager->Play(OK_SOUND);
						break;
					case 5:
						SoundManager->Stop();
						DestroyWindow(hwnd);
						break;
				}
			}
			if(Keystate.Esc)
			{
				SoundManager->Stop();
				DestroyWindow(hwnd);
			}
			break;

		
		case LEVEL_ONE:
			if(Keystate.Esc)
			{
				ReenterMenu();
			}
			if(Keystate.Space)
			{
				if(!Ship.Dying()&&!Ship.Dead())
					FireMan.Create(SHIP_LASER_1, Ship.GetX(), Ship.GetY(), 859, Ship.GetY());
			}
			if(Keystate.Up) Ship.SetDirection(8);
			if(Keystate.Down) Ship.SetDirection(2);
			if(Keystate.Left) Ship.SetDirection(4);
			if(Keystate.Right) Ship.SetDirection(6);
			if((Keystate.Up)&&(Keystate.Left)) Ship.SetDirection(7);
			if((Keystate.Up)&&(Keystate.Right)) Ship.SetDirection(9);
			if((Keystate.Down)&&(Keystate.Left)) Ship.SetDirection(1);
			if((Keystate.Down)&&(Keystate.Right)) Ship.SetDirection(3);
			if((!Keystate.Up)&&(!Keystate.Down)&&(!Keystate.Left)&&(!Keystate.Right)) Ship.SetDirection(0);
			break;
	}			
}
