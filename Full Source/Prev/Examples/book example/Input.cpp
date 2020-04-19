//input.cpp: program file for input class

//Copyright Ian Parberry, 1999
//Last updated March 15, 2000

#include <stdio.h> //for sscanf, sprintf

#include "input.h"
#include "objman.h" //for object manager
#include "psound.h" //for sound manager
#include "topscore.h" //for high score list
#include "gtimer.h" //game timer

extern BOOL show_framerate;

extern GamePhaseType GamePhase; //current phase
extern GamePhaseType NextPhase; //next phase of game
extern BOOL endphase; //should we abort current phase?
extern CObjectManager ObjectManager; //object manager
extern CSoundManager* SoundManager; //sound manager
extern CHighScoreList HighScoreList; //high score list manager
extern CGameTimer Timer; //game timer

extern BOOL GamePaused; //TRUE if game is paused
extern BOOL FlipHelpPage; //whether to flip help page

const WPARAM PAUSED_KEY=VK_F1; //pause key

CInputManager::CInputManager(){ //constructor
  init_input_device(); //current input device
  ButtonManager=NULL; //for managing menu buttons
  JoystickManager.initialize(); //set up joystick
  JoystickManager.autorepeat(FALSE); //disable autorepeat
}

CInputManager::~CInputManager(){ //destructor
  delete ButtonManager;
}

BOOL CInputManager::Restore(){ //restore surfaces
  return ButtonManager->Restore();
}

void CInputManager::Release(){ //release surfaces
   ButtonManager->Release();
}

void CInputManager::SetupMainMenuButtons(){ 
//set up main menu button manager
  const int YDELTA=70; //y separation between buttons
  const int BUTTONCOUNT=6; //number of buttons
  SIZE size; //size of buttons
  size.cx=40; size.cy=40; //size of buttons
  POINT point; //top left corner of first button
  point.x=111; point.y=46; //first button location
  delete ButtonManager; //delete any old one
  ButtonManager=new //create new button manager
    CButtonManager(BUTTONCOUNT,size,point,YDELTA,
      "buttons.bmp");
 //set button sounds
  ButtonManager->
    setsounds(BIGCLICK_SOUND,SMALLCLICK_SOUND);
}

void CInputManager::SetupDeviceMenuButtons(){ 
//set up device menu button manager
  const int BUTTONCOUNT=4;
  delete ButtonManager; //if there is one already, delete it
  SIZE size; //size of buttons
  size.cx=40; size.cy=40; //size of buttons
  ButtonManager=new
    CButtonManager(BUTTONCOUNT,size,"buttons.bmp");
  ButtonManager->setsounds(BIGCLICK_SOUND,SMALLCLICK_SOUND);
  ButtonManager->set_radio(); //radio buttons
  //add buttons
  POINT point;
  point.x=209; point.y=130;
  ButtonManager->addbutton(point);
  point.x=209; point.y=210;
  ButtonManager->addbutton(point);
  point.x=209; point.y=291;
  ButtonManager->addbutton(point);
  point.x=209; point.y=372;
  ButtonManager->addbutton(point);
  //initialize image of radio button
  ButtonManager->buttondown(InputDevice,FALSE); //no sound
}

void CInputManager::SetupButtons(GamePhaseType phase){
//set up menu buttons
  switch(phase){
    case MENU_PHASE: SetupMainMenuButtons(); break;
    case DEVICEMENU_PHASE: SetupDeviceMenuButtons(); break;
  }
}

//registry access for current input device

void CInputManager::init_input_device(){ 
//get last used device from registry
  HKEY RegistryKey; //registry key
  const int BUFSIZE=8; //buffer size
  char buffer[BUFSIZE]; //text buffer
  int result=RegCreateKeyEx(HKEY_CURRENT_USER,
    "SOFTWARE\\LARC\\NedFarm",0,NULL,0,KEY_READ,
    NULL,&RegistryKey,NULL);
  DWORD type; //type of registry entry
  if(result==ERROR_SUCCESS){ //entry exists
    unsigned long length=BUFSIZE-1; buffer[0]=0;
    RegQueryValueEx(RegistryKey,"InputDevice",NULL,&type,
       (unsigned char *)buffer,&length); //get value into buffer
    if(length){ //success
      buffer[length]=0; sscanf(buffer,"%d",&InputDevice);
    }
    else InputDevice=KEYBOARD_INPUT; //default to keyboard input
    RegCloseKey(RegistryKey);
  }
}

void CInputManager::set_input_device(){ 
//save last used device to registry
  HKEY RegistryKey; //registry key
  int result=RegCreateKeyEx(HKEY_CURRENT_USER,
    "SOFTWARE\\LARC\\NedFarm",0,NULL,0,KEY_WRITE,
    NULL,&RegistryKey,NULL);
  if(result==ERROR_SUCCESS){ //entry exists
    const int BUFSIZE=16; //buffer size
    char buffer[BUFSIZE]; //text buffer
    sprintf(buffer,"%d",InputDevice); //convert to string
    RegSetValueEx(RegistryKey,"InputDevice",NULL,REG_SZ,
      (unsigned char *)buffer,strlen(buffer)+1); //set value
    RegCloseKey(RegistryKey);
  }
}

//keyboard handlers

void CInputManager::IntroKeyboard(WPARAM keystroke){
//keyboard handler for intro
  endphase=TRUE; //any key ends the phase
}

BOOL CInputManager::MainMenuKeyboard(WPARAM keystroke){ 
//keyboard handler for menu
  BOOL result=FALSE;
  switch(keystroke){
    case VK_ESCAPE:
    case 'Q': //exit the game
      result=TRUE;
      break;
    case 'N': //play new game
      NextPhase=PLAYING_PHASE; endphase=TRUE;
      break;
    case 'D':
      NextPhase=DEVICEMENU_PHASE; endphase=TRUE;
      break;
    case 'L': //see high score list
      NextPhase=HIGHSCORE_PHASE; endphase=TRUE;
      break;
    case 'H': //help
      NextPhase=HELP_PHASE; endphase=TRUE;
      break;
    default: break; //do nothing
  }
  return result;
}

void CInputManager::DeviceMenuKeyboard(WPARAM keystroke){
//keyboard handler //for device menu
  switch(keystroke){
    case VK_ESCAPE:
    case 'D': //exit menu
      set_input_device(); //save new input device to registry
      endphase=TRUE;
      break;
    case 'K': //play using keyboard
      InputDevice=KEYBOARD_INPUT; //set device
      ButtonManager->buttondown(KEYBOARD_INPUT);
      break;
    case 'M': //play using mouse
      InputDevice=MOUSE_INPUT; //set device
      ButtonManager->buttondown(MOUSE_INPUT);
      break;
    case 'J': //play using joystick
      if(JoystickManager.exists()){ //if joystick exists
        InputDevice=JOYSTICK_INPUT;
        ButtonManager->buttondown(JOYSTICK_INPUT);
      }
      break;
    default: break;
  }
}

BOOL CInputManager::letter(WPARAM keystroke){
  return keystroke>='A'&&keystroke<='Z';
}

BOOL CInputManager::number(WPARAM keystroke){
  return keystroke>='0'&&keystroke<='9';
}

void CInputManager::NewhighscoreKeyboard(WPARAM keystroke){
  if(letter(keystroke)){ //enter a letter
    if(HighScoreList.append_to_current(keystroke-'A'+'a'))
      SoundManager->play(TYPE_CHAR_SOUND);
  }
  else if(number(keystroke)){ //enter a numeric char
    if(HighScoreList.append_to_current(keystroke))
      SoundManager->play(TYPE_CHAR_SOUND);
  }
  else if(keystroke==VK_SPACE){ //enter a space
    if(HighScoreList.append_to_current(' '))
      SoundManager->play(TYPE_SPACE_SOUND);
  }
  else if(keystroke==VK_RETURN){ //end of name
    SoundManager->play(TYPE_RETURN_SOUND); endphase=TRUE;
  }
  else if(keystroke==VK_DELETE){ //delete a char
    if(HighScoreList.delete_from_current())
      SoundManager->play(TYPE_SPACE_SOUND);
  }
  else if(keystroke==VK_ESCAPE){ //abort
    HighScoreList.cancel_current(); endphase=TRUE;
  }
}


void CInputManager::GameKeyboard(WPARAM keystroke){ 
//keyboard handler for game play
  //bail if game is paused unless end pause key pressed
  if(GamePaused&&keystroke!=PAUSED_KEY)return;
  //process keystroke
  switch(keystroke){
    //function keys
    case VK_ESCAPE: endphase=TRUE; break;
    case PAUSED_KEY:
      if(GamePaused)unpause(); else pause();
    break;
    case VK_F2: //device menu
      endphase=TRUE; NextPhase=DEVICEMENU_PHASE; pause();
      break;
    case VK_F3: //high score list
      endphase=TRUE; NextPhase=HIGHSCORE_PHASE; pause();
      break;
    case VK_F4: //help
      endphase=TRUE; NextPhase=HELP_PHASE; pause();
      break;
    case VK_F5: show_framerate=!show_framerate; break;
    //regular keys
    case VK_UP: ObjectManager.accelerate(0,-1); break;
    case VK_DOWN: ObjectManager.accelerate(0,1); break;
    case VK_LEFT: ObjectManager.accelerate(-1,0); break;
    case VK_RIGHT: ObjectManager.accelerate(1,0); break;
    case VK_SPACE: ObjectManager.fire_gun(); break;
    default: break;
  }
}

void CInputManager::HelpKeyboard(WPARAM keystroke){ 
//help keyboard handler
  switch(keystroke){
    case VK_ESCAPE: endphase=TRUE; break;
    default: FlipHelpPage=TRUE; break;
  }
}

BOOL CInputManager::is_function_key(WPARAM keystroke){
  return keystroke==VK_ESCAPE||
    (keystroke>=VK_F1&&keystroke<=VK_F12);
}

BOOL CInputManager::Keyboard(WPARAM keystroke,
  BOOL virtual_keystroke){ //keyboard handler
  BOOL result=FALSE;
  switch(GamePhase){
    case LOGO_PHASE:
    case TITLE_PHASE:
    case CREDITS_PHASE:
    case HIGHSCORE_PHASE:
    case SUCCESS_PHASE: 
      IntroKeyboard(keystroke);
      break;
    case NEWHIGHSCORE_PHASE: 
      NewhighscoreKeyboard(keystroke);
      break;
    case MENU_PHASE:
      result=MainMenuKeyboard(keystroke);
      break;
    case DEVICEMENU_PHASE:
      DeviceMenuKeyboard(keystroke);
      break;    
    case HELP_PHASE: HelpKeyboard(keystroke); break;
    case PLAYING_PHASE:
      if(InputDevice==KEYBOARD_INPUT||
      is_function_key(keystroke)||virtual_keystroke)
        GameKeyboard(keystroke);
      break;
  }
  return result;
}

//mouse left button down handlers

void CInputManager::MainMenuLMouseDown(POINT point){
//mouse down handler for menu
  if(ButtonManager->hit(point)>=0) //if a valid hit
    ButtonManager->buttondown(point); //animate a button down
}

void CInputManager::DeviceMenuLMouseDown(POINT point){
//mouse down handler for device menu
  int hit=ButtonManager->hit(point);
  switch(hit){
    case 0: Keyboard('K'); break; //keyboard
    case 1: Keyboard('M'); break; //mouse
    case 2: Keyboard('J'); break; //joystick
    case 3:
      ButtonManager->buttondown(hit); //quit button down
      break;
    default: break;
  }
}

void CInputManager::GameLMouseDown(POINT point){
//mouse down handler for game
  Keyboard(VK_SPACE,TRUE);
}

void CInputManager::decode(LPARAM lparam,POINT &point){
//decode mouse click lparam to point
  point.x=LOWORD(lparam); point.y=HIWORD(lparam); 
}

void CInputManager::LMouseDown(LPARAM lparam){
//main mouse left button down handler
  POINT point; //mouse location on screen
  decode(lparam,point); //decode mouse point
  switch(GamePhase){
    case MENU_PHASE:
      MainMenuLMouseDown(point);
      break;
    case DEVICEMENU_PHASE:
      DeviceMenuLMouseDown(point);
      break;
    case PLAYING_PHASE:
      if(InputDevice==MOUSE_INPUT)
        GameLMouseDown(point);
      break;
  }
}

//mouse left button up handlers

void CInputManager::IntroLMouseUp(POINT point){
//mouse left button up handler for intro
  endphase=TRUE;
}

BOOL CInputManager::MainMenuLMouseUp(POINT point){ 
//mouse left button up handler for menu
  int hit=ButtonManager->release(point); //get button hit
  BOOL result=FALSE;
  //take action depending on which button was hit
  switch(hit){ //depending on which button was hit
    case 0: Keyboard('N'); break; //new game
    case 1: Keyboard('S'); break; //saved game
    case 2: Keyboard('D'); break; //devices
    case 3: Keyboard('L'); break; //high score list
    case 4: Keyboard('H'); break; //help
    case 5: result=Keyboard(VK_ESCAPE); break; //quit
    default: break;
  }
  //animate button images
  if(hit>=0)ButtonManager->buttonup(hit); //hit
  else ButtonManager->allbuttonsup(); //nonhit
  return result;
}

void CInputManager::DeviceMenuLMouseUp(POINT point){
//mouse left button up handler for device menu
  int hit=ButtonManager->release(point);
  switch(hit){
    case 3: //quit button up
      ButtonManager->buttonup(hit); //show quit button up
      Keyboard(VK_ESCAPE);
      break;
  }
  if(hit<0)ButtonManager->allbuttonsup(); //no hit
}

void CInputManager::HelpLMouseUp(POINT point){
//mouse left button up handler for help
  Keyboard(VK_SPACE);
}

BOOL CInputManager::LMouseUp(LPARAM lparam){ 
//main mouse left button up handler
  BOOL result=FALSE;
  POINT point; //mouse location on screen
  decode(lparam,point); //decode mouse point
  switch(GamePhase){
    case LOGO_PHASE:
    case TITLE_PHASE:
    case CREDITS_PHASE:
    case SUCCESS_PHASE: 
    case HIGHSCORE_PHASE:
      IntroLMouseUp(point);
      break;
    case MENU_PHASE:
      result=MainMenuLMouseUp(point);
      break;
    case DEVICEMENU_PHASE:
      DeviceMenuLMouseUp(point);
      break;
    case HELP_PHASE:
      HelpLMouseUp(point);
      break;
  }
  return result;
}

//mouse motion handler

void CInputManager::set_plane_speed(POINT point,SIZE extent){
//set plane speed based on point's position in extent
  int xmin,xmax,ymin,ymax; //plane speed limits
  ObjectManager.speed_limits(xmin,xmax,ymin,ymax); //get them
  //bands for speed assignment
  const int XBANDWIDTH=extent.cx/(xmax-xmin+1);
  const int YBANDWIDTH=extent.cy/(ymax-ymin+1);
  int xspeed,yspeed; //speed of plane
  xspeed=point.x/XBANDWIDTH+xmin; //horizontal speed
  yspeed=point.y/YBANDWIDTH+ymin; //vertical speed
  ObjectManager.set_speed(xspeed,yspeed); //pass to plane
}

void CInputManager::MouseMove(LPARAM lparam){
  if(InputDevice!=MOUSE_INPUT)return; //bail if not needed
  if(GamePhase!=PLAYING_PHASE)return; //bail if not playing
  if(GamePaused)return; //bail if game is paused
  POINT point; //mouse location on screen
  decode(lparam,point); //decode mouse point
  //set extent
  SIZE extent; //extent that mouse moves in
  extent.cx=SCREEN_WIDTH;
  extent.cy=SCREEN_HEIGHT; 
  //set plane speed based on point and extent
  set_plane_speed(point,extent); 
}

//joystick handlers

void CInputManager::GameJoystick(){ 
//joystick handler for game play
  if(InputDevice!=JOYSTICK_INPUT)return; //bail if unneeded
  if(GamePaused)return; //bail if game is paused
  //buttons
  if(JoystickManager.button_down(1)) //button 1...
    Keyboard(VK_SPACE,TRUE); //...fires bullets
  //stick
  if(GamePhase!=PLAYING_PHASE)return; //bail if not playing
  POINT point; //coordinates of joystick
  JoystickManager.position(point); //get coordinates
  SIZE extent; //extent that joystick indicator moves in
  extent.cx=100; extent.cy=100; //set extent
  set_plane_speed(point,extent); //set plane speed
}

void CInputManager::IntroJoystick(){ 
//joystick handler for intro
  if(JoystickManager.button_down(1)) //click out
    Keyboard(VK_ESCAPE);
}

void CInputManager::HelpJoystick(){ 
//joystick handler for help
  if(JoystickManager.button_down(1))
    Keyboard(VK_SPACE); //next page
}

void CInputManager::Joystick(){ //main joystick handler
  JoystickManager.poll(); //poll joystick
  switch(GamePhase){ //call joystick handler for phase
    case PLAYING_PHASE: GameJoystick(); break;
    case LOGO_PHASE:
    case TITLE_PHASE:
    case CREDITS_PHASE:
    case SUCCESS_PHASE: 
    case HIGHSCORE_PHASE:
      IntroJoystick(); break;
    case DEVICEMENU_PHASE:
    case MENU_PHASE:
      break; //do nothing
    case HELP_PHASE: HelpJoystick(); break;
  }
}

void CInputManager::unpause(){ //unpause the game
  GamePaused=FALSE;
  Timer.unpause(); //unpause the timer
  ((CGameSoundManager*)SoundManager)->unpause(); //and the sound
}

void CInputManager::pause(){ //unpause the game
  GamePaused=TRUE;
  Timer.pause(); //pause the timer
  ((CGameSoundManager*)SoundManager)->pause(); //and the sound
}

