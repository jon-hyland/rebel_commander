#include <windows.h>
#include <windowsx.h>
#include <dinput.h>

#include "defines.h"
#include "sound.h"
#include "ship.h"
#include "fireman.h"
#include "timer.h"
#include "alpha.h"	//debug

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

extern int GameState;
extern HWND hwnd;
extern LPDIRECTINPUT8 lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 Keyboard;
extern LPDIRECTINPUTDEVICE8 Joystick;
extern InputStruct InputState;
extern CSoundManager *SoundManager;
extern CShip Ship;
extern CFireManager FireMan;
extern CTimer Timer;
extern int MenuSelection;
extern DWORD LevelStartTime;
extern DWORD LastGameTime;
extern int LastGameState;
extern BOOL Invincible;
extern BOOL MenuActive;
extern BOOL GameOver;
extern DWORD GameOverTime;
extern DWORD nGameTimeSnap;					//debug
extern CAlpha Alpha;						//debug
extern LPDIRECTDRAWSURFACE lpSecondary;		//debug

extern void ResetGame();
extern void StartTitle();
extern void StartLevelOne();
extern void ReenterMenu();
extern void ReexitMenu();
extern void ClearScreenSurfaces();

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

//called once for each joystick, stops at one
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
    HRESULT hr;

    //obtain an interface to the enumerated joystick.
    hr = lpDirectInputObject->CreateDevice(pdidInstance->guidInstance,&Joystick,NULL);

    //if it failed, then we can't use this joystick (maybe the user unplugged it)
    if(FAILED(hr)) 
        return DIENUM_CONTINUE;

	//stop enumeration after first one
    return DIENUM_STOP;
}

//callback function for enumerating objects (axes, buttons, POVs) on a joystick
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext)
{
    HWND hDlg=(HWND)pContext;

    static int nSliderCount=0;  //number of returned slider controls
    static int nPOVCount=0;     //number of returned POV controls

    //for axes that are returned, set the DIPROP_RANGE property for the
    //enumerated axis in order to scale min/max values
    if(pdidoi->dwType & DIDFT_AXIS)
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; //specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        //set the range for the axis
        if(FAILED(Joystick->SetProperty(DIPROP_RANGE, &diprg.diph))) 
            return DIENUM_STOP;
    }

	return DIENUM_CONTINUE;
}


void InitInput()
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

	//set 256 byte array for keyboard
	if(FAILED(Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{ ReleaseDirectInput();	return;	}

	//set keyboard cooperation level
	if(FAILED(Keyboard->SetCooperativeLevel(hwnd,dwCoopFlags)))
	{ ReleaseDirectInput();	return;	}

	//acquire keyboard
	if(FAILED(Keyboard->Acquire()))
	{ ReleaseDirectInput();	return;	}

	//look for simple joystick device, create
	if(FAILED(lpDirectInputObject->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,NULL,DIEDFL_ATTACHEDONLY)))
	{ ; //do nothing, we don't need a joystick 
	}

	//set joystick data format to "simple joystick"
	if(Joystick!=NULL)
		if(FAILED(Joystick->SetDataFormat(&c_dfDIJoystick2)))
		{ ; //do nothing
		}

	//set joystick cooperation level
	if(Joystick!=NULL)
		if(FAILED(Joystick->SetCooperativeLevel(hwnd,dwCoopFlags)))
		{ ; //do nothing
		}

	//enumerate the joysick objects (finds joystick buttons, sets min/max axes)
	if(Joystick!=NULL)
		if(FAILED(Joystick->EnumObjects(EnumObjectsCallback,(VOID*)hwnd,DIDFT_ALL)))
		{ ; //do nothing
		}
	
	//set inputstate last times to current time
	InputState.UpLastTime=time;
	InputState.DownLastTime=time;
	InputState.LeftLastTime=time;
	InputState.RightLastTime=time;
	InputState.SpaceLastTime=time;
	InputState.EscLastTime=time;
	InputState.EnterLastTime=time;
	InputState.ILastTime=time;
	InputState.RLastTime=time;
	InputState.TLastTime=time;
	InputState.JoyALastTime=time;
	InputState.JoyBLastTime=time;
	InputState.JoyXLastTime=time;
	InputState.JoyYLastTime=time;
	InputState.JoyUpLastTime=time;
	InputState.JoyDownLastTime=time;
	InputState.JoyLeftLastTime=time;
	InputState.JoyRightLastTime=time;
	
	//set timer to get input state 100x per second
	//(timer will trigger message pump with WM_TIMER)
	SetTimer(hwnd,0,1000/100,NULL);
}

void SetInputStateDelays()
{
	//set min key delays based on game state
	switch(GameState)
	{
		case LOGO_SCREEN: case TITLE_SCREEN:
			InputState.UpDelay=200;
			InputState.DownDelay=200;
			InputState.LeftDelay=200;
			InputState.RightDelay=200;
			InputState.SpaceDelay=200;
			InputState.EscDelay=200;
			InputState.EnterDelay=200;
			InputState.IDelay=200;
			InputState.RDelay=200;
			InputState.TDelay=200;
			InputState.JoyADelay=200;
			InputState.JoyBDelay=200;
			InputState.JoyXDelay=200;
			InputState.JoyYDelay=200;
			InputState.JoyUpDelay=200;
			InputState.JoyDownDelay=200;
			InputState.JoyLeftDelay=200;
			InputState.JoyRightDelay=200;
			break;
		case MAIN_MENU:
			InputState.UpDelay=160;
			InputState.DownDelay=160;
			InputState.LeftDelay=200;
			InputState.RightDelay=200;
			InputState.SpaceDelay=200;
			InputState.EscDelay=200;
			InputState.EnterDelay=200;
			InputState.IDelay=200;
			InputState.RDelay=200;
			InputState.TDelay=200;
			InputState.JoyADelay=200;
			InputState.JoyBDelay=200;
			InputState.JoyXDelay=200;
			InputState.JoyYDelay=200;
			InputState.JoyUpDelay=160;
			InputState.JoyDownDelay=160;
			InputState.JoyLeftDelay=200;
			InputState.JoyRightDelay=200;
			break;
		case LEVEL_ONE: case LEVEL_TWO: case LEVEL_THREE:
			InputState.UpDelay=0;
			InputState.DownDelay=0;
			InputState.LeftDelay=0;
			InputState.RightDelay=0;
			InputState.SpaceDelay=200;
			InputState.EscDelay=200;
			InputState.EnterDelay=200;
			InputState.IDelay=200;
			InputState.RDelay=200;
			InputState.TDelay=200;
			InputState.JoyADelay=200;
			InputState.JoyBDelay=200;
			InputState.JoyXDelay=200;
			InputState.JoyYDelay=200;
			InputState.JoyUpDelay=0;
			InputState.JoyDownDelay=0;
			InputState.JoyLeftDelay=0;
			InputState.JoyRightDelay=0;
			break;
	}
}

void ResetInputStateTimes()
{
	InputState.UpLastTime=0;
	InputState.DownLastTime=0;
	InputState.LeftLastTime=0;
	InputState.RightLastTime=0;
	InputState.SpaceLastTime=0;
	//InputState.EscLastTime=0;
	InputState.EnterLastTime=0;
	InputState.ILastTime=0;
	InputState.RLastTime=0;
	InputState.TLastTime=0;
	InputState.JoyALastTime=0;
	InputState.JoyBLastTime=0;
	InputState.JoyXLastTime=0;
	InputState.JoyYLastTime=0;
	InputState.JoyUpLastTime=0;
	InputState.JoyDownLastTime=0;
	InputState.JoyLeftLastTime=0;
	InputState.JoyRightLastTime=0;
}

void GetDInputState()
{
	BYTE	strKeys[256];	//keyboard state buffer
	DIJOYSTATE2 js;			//joystick state buffer
	DWORD time=Timer.GameTime();

	//read the state of all keys
	ZeroMemory(&strKeys,sizeof(strKeys));	//clear it
	if(FAILED(Keyboard->GetDeviceState(sizeof(strKeys),&strKeys)))
	{ ReleaseDirectInput();	return;	}

	//read the state of joystick
	//ZeroMemory(&strJoystick,sizeof(strJoystick));	//clear it
	if(Joystick!=NULL)
	{
		//poll the joystick, try to reaquire once if needed
		if(FAILED(Joystick->Poll()))
		{
			Joystick->Acquire();
			if(FAILED(Joystick->Poll()))
			{; //do nothing
			}
		}

		//get joystick device state
		if(FAILED(Joystick->GetDeviceState(sizeof(DIJOYSTATE2),&js)))
		{; //do nothing
		}
	}

	//reset keystate
	InputState.Up=FALSE;
	InputState.Down=FALSE;
	InputState.Left=FALSE;
	InputState.Right=FALSE;
	InputState.Space=FALSE;
	InputState.Esc=FALSE;
	InputState.Enter=FALSE;
	InputState.I=FALSE;
	InputState.R=FALSE;
	InputState.T=FALSE;
	InputState.JoyA=FALSE;
	InputState.JoyB=FALSE;
	InputState.JoyX=FALSE;
	InputState.JoyY=FALSE;
	InputState.JoyUp=FALSE;
	InputState.JoyDown=FALSE;
	InputState.JoyLeft=FALSE;
	InputState.JoyRight=FALSE;

	//set key delays for game state
	SetInputStateDelays();	

	//set keys state based on key buffer
	for(int i=0; i<256; i++)
	{
		if(strKeys[i]&0x80)
		{
			//debug - also remove surface clear
			//Alpha.Write(lpSecondary,540,200,i,BLUE_ALPHA_SPRITE);
			
			switch(i)
			{
				case 200:
					if(time>=InputState.UpLastTime+InputState.UpDelay)
					{
						InputState.Up=TRUE;
						InputState.UpLastTime=time;
					}
					break;
				case 208:	
					if(time>=InputState.DownLastTime+InputState.DownDelay)
					{
						InputState.Down=TRUE;
						InputState.DownLastTime=time;
					}
					break;
				case 203:	
					if(time>=InputState.LeftLastTime+InputState.LeftDelay)
					{
						InputState.Left=TRUE; 
						InputState.LeftLastTime=time;
					}
					break;
				case 205:	
					if(time>=InputState.RightLastTime+InputState.RightDelay)
					{
						InputState.Right=TRUE; 
						InputState.RightLastTime=time;
					}
					break;
				case 57:	
					if(time>=InputState.SpaceLastTime+InputState.SpaceDelay)
					{
						InputState.Space=TRUE; 
						InputState.SpaceLastTime=time;
					}
					break;
				case 1:		
					if(time>=InputState.EscLastTime+InputState.EscDelay)
					{
						InputState.Esc=TRUE; 
						InputState.EscLastTime=time;
					}
					break;
				case 28:	
					if(time>=InputState.EnterLastTime+InputState.EnterDelay)
					{
						InputState.Enter=TRUE; 
						InputState.EnterLastTime=time;
					}
					break;
				case 23:
					if(time>=InputState.ILastTime+InputState.IDelay)
					{
						InputState.I=TRUE; 
						InputState.ILastTime=time;
					}
					break;
				case 19:
					if(time>=InputState.RLastTime+InputState.RDelay)
					{
						InputState.R=TRUE; 
						InputState.RLastTime=time;
					}
					break;
				case 20:
					if(time>=InputState.TLastTime+InputState.TDelay)
					{
						InputState.T=TRUE; 
						InputState.TLastTime=time;
					}
					break;
			}
		
		}
		else //if key not pressed
		{
			switch(i)
			{
				case 200:
					if(InputState.UpLastTime+InputState.UpDelay>time)InputState.UpLastTime-=InputState.UpDelay;
					break;
				case 208:
					if(InputState.DownLastTime+InputState.DownDelay>time)InputState.DownLastTime-=InputState.DownDelay;
					break;
				case 203:
					if(InputState.LeftLastTime+InputState.LeftDelay>time)InputState.LeftLastTime-=InputState.LeftDelay;
					break;
				case 205:
					if(InputState.RightLastTime+InputState.RightDelay>time)InputState.RightLastTime-=InputState.RightDelay;
					break;
				case 57:
					if(InputState.SpaceLastTime+InputState.SpaceDelay>time)InputState.SpaceLastTime-=InputState.SpaceDelay;
					break;
				case 1:
					if(InputState.EscLastTime+InputState.EscDelay>time)InputState.EscLastTime-=InputState.EscDelay;
					break;
				case 28:
					if(InputState.EnterLastTime+InputState.EnterDelay>time)InputState.EnterLastTime-=InputState.EnterDelay;
					break;
				case 23:
					if(InputState.ILastTime+InputState.IDelay>time)InputState.ILastTime-=InputState.IDelay;
					break;
				case 19:
					if(InputState.RLastTime+InputState.RDelay>time)InputState.RLastTime-=InputState.RDelay;
					break;
				case 20:
					if(InputState.TLastTime+InputState.TDelay>time)InputState.TLastTime-=InputState.TDelay;
					break;
			}
		}
	}

	//if theres a joystick then save state
	if(Joystick!=NULL)
	{
		//set state for joystick directional pad/control stick
		if(js.lY<(-JOYSTICK_AXIS_MIN))
		{
			if(time>=InputState.JoyUpLastTime+InputState.JoyUpDelay)
			{
				InputState.JoyUp=TRUE; 
				InputState.JoyUpLastTime=time;
			}
			if(InputState.JoyDownLastTime+InputState.JoyDownDelay>time)InputState.JoyDownLastTime-=InputState.JoyDownDelay;
		}
		else if(js.lY>JOYSTICK_AXIS_MIN)
		{
			if(time>=InputState.JoyDownLastTime+InputState.JoyDownDelay)
			{
				InputState.JoyDown=TRUE; 
				InputState.JoyDownLastTime=time;
			}
			if(InputState.JoyUpLastTime+InputState.JoyUpDelay>time)InputState.JoyUpLastTime-=InputState.JoyUpDelay;
		}
		else
		{
			if(InputState.JoyDownLastTime+InputState.JoyDownDelay>time)InputState.JoyDownLastTime-=InputState.JoyDownDelay;
			if(InputState.JoyUpLastTime+InputState.JoyUpDelay>time)InputState.JoyUpLastTime-=InputState.JoyUpDelay;
		}
			
		if(js.lX<(-JOYSTICK_AXIS_MIN))
		{
			if(time>=InputState.JoyLeftLastTime+InputState.JoyLeftDelay)
			{
				InputState.JoyLeft=TRUE; 
				InputState.JoyLeftLastTime=time;
			}
			if(InputState.JoyRightLastTime+InputState.JoyRightDelay>time)InputState.JoyRightLastTime-=InputState.JoyRightDelay;
		}
		else if(js.lX>JOYSTICK_AXIS_MIN)
		{
			if(time>=InputState.JoyRightLastTime+InputState.JoyRightDelay)
			{
				InputState.JoyRight=TRUE; 
				InputState.JoyRightLastTime=time;
			}
			if(InputState.JoyLeftLastTime+InputState.JoyLeftDelay>time)InputState.JoyLeftLastTime-=InputState.JoyLeftDelay;
		}
		else
		{
			if(InputState.JoyRightLastTime+InputState.JoyRightDelay>time)InputState.JoyRightLastTime-=InputState.JoyRightDelay;
			if(InputState.JoyLeftLastTime+InputState.JoyLeftDelay>time)InputState.JoyLeftLastTime-=InputState.JoyLeftDelay;
		}

		//set state for joystick button keys
		for(i=0; i<128; i++)
		{
			if(js.rgbButtons[i]&0x80)	//if the joystick key is presses
			{
				//debug - also remove surface clear
				//Alpha.Write(lpSecondary,540,200,i,BLUE_ALPHA_SPRITE);
				
				switch(i)
				{
					case 0:
						if(time>=InputState.JoyALastTime+InputState.JoyADelay)
						{
							InputState.JoyA=TRUE; 
							InputState.JoyALastTime=time;
						}
						break;
					case 1:
						if(time>=InputState.JoyBLastTime+InputState.JoyBDelay)
						{
							InputState.JoyB=TRUE; 
							InputState.JoyBLastTime=time;
						}
						break;
					case 2:
						if(time>=InputState.JoyXLastTime+InputState.JoyXDelay)
						{
							InputState.JoyX=TRUE; 
							InputState.JoyXLastTime=time;
						}
						break;
					case 3:
						if(time>=InputState.JoyYLastTime+InputState.JoyYDelay)
						{
							InputState.JoyY=TRUE; 
							InputState.JoyYLastTime=time;
						}
						break;
				}
			}
			else //if joystick key not pressed
			{
				switch(i)
				{
					case 0:
						if(InputState.JoyALastTime+InputState.JoyADelay>time)InputState.JoyALastTime-=InputState.JoyADelay;
						break;
					case 1:
						if(InputState.JoyBLastTime+InputState.JoyBDelay>time)InputState.JoyBLastTime-=InputState.JoyBDelay;
						break;
					case 2:
						if(InputState.JoyXLastTime+InputState.JoyXDelay>time)InputState.JoyXLastTime-=InputState.JoyXDelay;
						break;
					case 3:
						if(InputState.JoyYLastTime+InputState.JoyYDelay>time)InputState.JoyYLastTime-=InputState.JoyYDelay;
						break;
				}
			}

		}
	}


}

void HandleInputState()
{
	//everything based on game state
	switch(GameState)
	{
		case LOGO_SCREEN:
			//if user hits key skip logo screen
			if((InputState.Esc)||(InputState.Enter)||(InputState.Space)||(InputState.JoyA))
			{
				GameState++;
				StartTitle();
			}
			break;


		case TITLE_SCREEN:
			//if user hits key skip title screen
			if((InputState.Esc)||(InputState.Enter)||(InputState.Space)||(InputState.JoyA))
			{
				GameState++;
			}
			break;


		case MAIN_MENU:
			if((InputState.Up)||(InputState.JoyUp))
			{
				SoundManager->Play(SELECT_SOUND);
				if(--MenuSelection<0)MenuSelection=6;
			}
			if((InputState.Down)||(InputState.JoyDown))
			{
				SoundManager->Play(SELECT_SOUND);
				if(++MenuSelection>6)MenuSelection=0;
			}
			if((InputState.Enter)||(InputState.Space)||(InputState.JoyA))
			{
				switch(MenuSelection)
				{
					case 0:
						SoundManager->Play(OK_SOUND);
						if(LastGameState==0)
						{
							MenuActive=FALSE;
							StartLevelOne();
						}
						else
						{
							ReexitMenu();
						}
						break;
					case 1:
						ResetGame();						
						break;
					case 2:
						if(SoundManager->SoundOn())
							SoundManager->TurnOff();
						else
							SoundManager->TurnOn();
						SoundManager->Play(OK_SOUND);
						break;
					case 3:
						SoundManager->Play(OK_SOUND);
						break;
					case 4:
						SoundManager->Play(OK_SOUND);
						break;
					case 5:
						SoundManager->Play(OK_SOUND);
						break;
					case 6:
						SoundManager->Stop();
						ClearScreenSurfaces();
						DestroyWindow(hwnd);
						break;
				}
			}
			if((InputState.Esc)||(InputState.JoyX))
			{

				if(LastGameState==0)
				{
					//exit game
					ClearScreenSurfaces();
					SoundManager->Stop();
					DestroyWindow(hwnd);
				}
				else
				{
					//exit menu
					ReexitMenu();
				}

			
			}
			break;

		
		case LEVEL_ONE:
			//menu button
			if((InputState.Esc)||(InputState.JoyX))
			{
				ReenterMenu();
			}
			//fire button
			if((InputState.Space)||(InputState.JoyA))
			{
				if(!GameOver)
				{
					if(!Ship.Dying()&&!Ship.Dead())
						FireMan.Create(Ship.GetFireType(), Ship.GetX(), Ship.GetY(), 859, Ship.GetY(), 20);
				}
				else
				{
					if(Timer.GameTime()>GameOverTime+1000)
						ResetGame();
				}
			}
			//directionals
			if((InputState.Up)||(InputState.JoyUp)) Ship.SetDirection(8);
			if((InputState.Down)||(InputState.JoyDown)) Ship.SetDirection(2);
			if((InputState.Left)||(InputState.JoyLeft)) Ship.SetDirection(4);
			if((InputState.Right)||(InputState.JoyRight)) Ship.SetDirection(6);
			if(((InputState.Up)||(InputState.JoyUp))&&((InputState.Left)||(InputState.JoyLeft))) Ship.SetDirection(7);
			if(((InputState.Up)||(InputState.JoyUp))&&((InputState.Right)||(InputState.JoyRight))) Ship.SetDirection(9);
			if(((InputState.Down)||(InputState.JoyDown))&&((InputState.Left)||(InputState.JoyLeft))) Ship.SetDirection(1);
			if(((InputState.Down)||(InputState.JoyDown))&&((InputState.Right)||(InputState.JoyRight))) Ship.SetDirection(3);
			if((!InputState.Up)&&(!InputState.Down)&&(!InputState.Left)&&(!InputState.Right)&&(!InputState.JoyUp)&&(!InputState.JoyDown)&&(!InputState.JoyLeft)&&(!InputState.JoyRight)) Ship.SetDirection(0);
			//invincible button
			if(InputState.I)
			{
				SoundManager->Play(OK_SOUND);
				Invincible=!Invincible;
			}
			//reset button
			if(InputState.R)
			{
				SoundManager->Play(OK_SOUND);
				StartLevelOne();
			}
			//obj start time (timestamp)
			if(InputState.T)
			{
				SoundManager->Play(OK_SOUND);
				nGameTimeSnap=Timer.GameTime()-LevelStartTime-1800;
			}
			break;
	}			
}
