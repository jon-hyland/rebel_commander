//input.h: header file for input class

//Copyright Ian Parberry, 1999
//Last updated March 15, 2000

#include <windows.h>
#include <windowsx.h>

#include "defines.h"
#include "buttons.h" //for button manager
#include "joystick.h" //for joystick manager


#ifndef __INPUT_H__
#define __INPUT_H__

class CInputManager{
  private:
    InputDeviceType InputDevice; //current input device
    CButtonManager *ButtonManager; //for managing menu buttons
    CJoystickManager JoystickManager; //joystick manager
    //helper functions
    BOOL is_function_key(WPARAM keystroke); //TRUE if function key
    void decode(LPARAM lparam,POINT &point); //decode mouse click
    void set_plane_speed(POINT point,SIZE extent);
    //for text input
    BOOL letter(WPARAM keystroke); //keystroke is a letter
    BOOL number(WPARAM keystroke); //keystroke is a digit
    //setting of input device from registry
    void init_input_device(); //get last used device
    void set_input_device(); //put last used device
    //set up buttons for menus
    void SetupMainMenuButtons(); //main menu button manager
    void SetupDeviceMenuButtons(); //device menu button manager
    //keyboard handlers
    void IntroKeyboard(WPARAM keystroke); //keyboard handler
    BOOL MainMenuKeyboard(WPARAM keystroke); //keyboard handler
    void DeviceMenuKeyboard(WPARAM keystroke); //keyboard handler
    void NewhighscoreKeyboard(WPARAM keystroke); //keyboard handler
    void GameKeyboard(WPARAM keystroke); //keyboard handler
    void HelpKeyboard(WPARAM keystroke); //keyboard handler
    //left mouse button down handlers
    void MainMenuLMouseDown(POINT point); //mouse down handler
    void DeviceMenuLMouseDown(POINT point); //mouse down handler
    void GameLMouseDown(POINT point); //mouse down handler 
    //left mouse button up handlers
    void IntroLMouseUp(POINT point); //mouse up handler
    BOOL MainMenuLMouseUp(POINT point); //mouse up handler
    void DeviceMenuLMouseUp(POINT point); //mouse up handler
    void HelpLMouseUp(POINT point); //mouse up handler
    //joystick handlers
    void IntroJoystick(); //joystick handler
    void GameJoystick(); //joystick handler
    void HelpJoystick(); //joystick handler
  public:
    CInputManager(); //constructor
    ~CInputManager(); //destructor
    BOOL Restore(); //restore surfaces
    void Release(); //release surfaces
    void SetupButtons(GamePhaseType phase); //set up menu buttons
    BOOL Keyboard(WPARAM keystroke,
      BOOL virtual_keystroke=FALSE); //keyboard handler
    void LMouseDown(LPARAM lparam); //main mouse down handler
    BOOL LMouseUp(LPARAM lparam); //main mouse up handler
    void MouseMove(LPARAM lparam); //handle mouse motion
    void Joystick(); //main joystick handler
    void pause(); //pause the game
    void unpause(); //unpause the game
};

#endif
