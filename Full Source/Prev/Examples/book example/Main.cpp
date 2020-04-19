//main.cpp

//Copyright Ian Parberry, 2000
//Last updated May 22, 2000

//Interrupting the game engine.  Pause key is F1.  When paused,
//the screen is frozen and the word "paused" is displayed on
//the screen.  Note that sounds are (approximately) paused too.
//Note also the use of the pause in the timer to make sure that
//objects don't move too far when the game is unpaused.

//The help button in the main menu now displays the help files.

//Can also break out of the game engine to see the device menu
//(F2), high score list (F3), help (F4). Frame rate is F5 now.

//system includes
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>

//system defines
#define WIN32_LEAN_AND_MEAN

//custom includes
#include "defines.h" //global definitions
#include "bmp.h" //bmp file reader
#include "gtimer.h" //game timer
#include "csprite.h" //for clipped sprite class
#include "objects.h" //for object class
#include "objman.h" //for object manager
#include "view.h" //for viewpoint class
#include "random.h" //for random number generator
#include "psound.h" //for sound manager
#include "input.h" //for input manager
#include "text.h" //for text sprite manager
#include "score.h" //for score manager
#include "topscore.h" //high score list

//defines
#define MAX_OBJECTS 32 //max number of objects in game

//globals

BOOL ActiveApp; //is this application active?

LPDIRECTDRAW lpDirectDrawObject=NULL; //direct draw object
LPDIRECTDRAWSURFACE lpPrimary=NULL; //primary surface
LPDIRECTDRAWPALETTE lpPrimaryPalette; //its palette
LPDIRECTDRAWSURFACE lpSecondary=NULL; //back buffer
LPDIRECTDRAWPALETTE lpSecondaryPalette; //its palette
LPDIRECTDRAWSURFACE lpBackground=NULL; //background image

CGameTimer Timer; //game timer

CBmpFileReader background; //background image
CBmpFileReader *NewHighScoreBg; //new-high-score background
CBmpSpriteFileReader g_cSpriteImages; //sprite images
CBmpSpriteFileReader g_cFrgndImages; //foreground images
CBmpSpriteFileReader g_cTextImages; //text images

CObjectManager ObjectManager(MAX_OBJECTS); //object manager

CClippedSprite *g_pSprite[NUM_SPRITES]; //sprites

CViewPoint Viewpoint; //player viewpoint
CRandom Random; //random number generator

GamePhaseType GamePhase; //current phase
GamePhaseType NextPhase; //next phase of game
BOOL endphase=FALSE; //should we abort current phase?
int PhaseTime=0; //time in phase
int level=1; //current level

BOOL show_framerate=FALSE; //TRUE to show the frame rate
int framerate_timer=0; //timer for updating frame rate display
int framecount=0; //number of frames in this interval (so far)
int last_framecount=0; //number of frames in last full interval

CSoundManager* SoundManager; //sound manager
CInputManager InputManager; //device input manager
CTextManager *Text; //text manager
CScoreManager ScoreManager; //score manager
CHighScoreList HighScoreList; //high score list manager

int HelpPage; //current page of help to be displayed
BOOL FlipHelpPage; //whether to flip help page
const int NUMHELPPAGES=6; //number of help pages
CClippedSprite *PausedSprite; //display word "Paused"
BOOL GamePaused=FALSE; //TRUE if game paused

//helper functions
LPDIRECTDRAWPALETTE CreatePalette(LPDIRECTDRAWSURFACE surface);
BOOL InitDirectDraw(HWND hwnd);
HWND CreateDefaultWindow(char* name,HINSTANCE hInstance);

BOOL LoadPlaneSprite(){
  BOOL result=TRUE;
  result=result&&g_pSprite[PLANE_OBJECT]->
    load(&g_cSpriteImages,0,1,1);
  result=result&&g_pSprite[PLANE_OBJECT]->
    load(&g_cSpriteImages,1,123,1);
  result=result&&g_pSprite[PLANE_OBJECT]->
    load(&g_cSpriteImages,2,245,1);
  result=result&&g_pSprite[PLANE_OBJECT]->
    load(&g_cSpriteImages,3,367,1);
  result=result&&g_pSprite[PLANE_OBJECT]->
    load(&g_cSpriteImages,4,489,1);
  result=result&&g_pSprite[PLANE_OBJECT]->
    load(&g_cSpriteImages,5,17,74);
  return result;
} //LoadPlaneSprite

BOOL LoadDamagedPlaneSprite(){
  BOOL result=TRUE;
  result=result&&g_pSprite[DAMAGEDPLANE_OBJECT]->
    load(&g_cSpriteImages,0,387,288);
  result=result&&g_pSprite[DAMAGEDPLANE_OBJECT]->
    load(&g_cSpriteImages,1,509,288);
  return result;
} //LoadDamagedPlaneSprite

BOOL LoadExplodingPlaneSprite(){
  BOOL result=TRUE;
  result=result&&g_pSprite[EXPLODINGPLANE_OBJECT]->
    load(&g_cSpriteImages,0,139,74);
  result=result&&g_pSprite[EXPLODINGPLANE_OBJECT]->
    load(&g_cSpriteImages,1,261,74);
  result=result&&g_pSprite[EXPLODINGPLANE_OBJECT]->
    load(&g_cSpriteImages,2,383,74);
  result=result&&g_pSprite[EXPLODINGPLANE_OBJECT]->
    load(&g_cSpriteImages,3,505,74);
  return result;
} //LoadExplodingPlaneSprite

BOOL LoadDeadPlaneSprite(){
  return g_pSprite[DEADPLANE_OBJECT]->
    load(&g_cSpriteImages,0,120,237);
} //LoadDeadPlaneSprite

BOOL LoadCrowSprite(){
  BOOL result=TRUE;
  result=result&&g_pSprite[CROW_OBJECT]->
    load(&g_cSpriteImages,0,256,183); //frame 0
  result=result&&g_pSprite[CROW_OBJECT]->
    load(&g_cSpriteImages,1,320,183); //frame 1
  result=result&&g_pSprite[CROW_OBJECT]->
    load(&g_cSpriteImages,2,256,237); //frame 2
  result=result&&g_pSprite[CROW_OBJECT]->
    load(&g_cSpriteImages,3,323,237); //frame 3
  return result;
} //LoadCrowSprite

BOOL LoadFrgndSprites(){ //load foreground sprites
  BOOL result=TRUE;
  result=result&&g_pSprite[FARM_OBJECT]->
    load(&g_cFrgndImages,0,0,0); //load farm sprite
  result=result&&g_pSprite[FIELD_OBJECT]->
    load(&g_cFrgndImages,0,640,0); //load field sprite
  return result;
} //LoadFrgndSprites

BOOL LoadDeadCrowSprite(){ //load dead crow
  return g_pSprite[DEADCROW_OBJECT]->
    load(&g_cSpriteImages,0,453,230);
} //LoadDeadCrowSprite

BOOL LoadExplodingCrowSprite(){ //load exploding crow
  BOOL result=TRUE;
  result=result&&g_pSprite[EXPLODINGCROW_OBJECT]->
    load(&g_cSpriteImages,0,257,294);
  result=result&&g_pSprite[EXPLODINGCROW_OBJECT]->
    load(&g_cSpriteImages,1,321,294);
  result=result&&g_pSprite[EXPLODINGCROW_OBJECT]->
    load(&g_cSpriteImages,2,386,162);
  result=result&&g_pSprite[EXPLODINGCROW_OBJECT]->
    load(&g_cSpriteImages,3,453,162);
  result=result&&g_pSprite[EXPLODINGCROW_OBJECT]->
    load(&g_cSpriteImages,4,386,230);
  result=result&&g_pSprite[EXPLODINGCROW_OBJECT]->
    load(&g_cSpriteImages,5,453,230);
  return result;
} //LoadExplodingCrowSprite

BOOL LoadBulletSprite(){ //load bullet
  return g_pSprite[BULLET_OBJECT]->
    load(&g_cSpriteImages,0,5,123);
} //LoadBulletSprite

BOOL LoadTextSprites(){ //load text sprites
  return Text->load(&g_cTextImages,1,102);
} //LoadTextSprites

BOOL LoadPausedSprite(){
  return PausedSprite->load(&g_cSpriteImages,0,399,403);
} //LoadPausedSprite

BOOL LoadImages(){ //load graphics from files to surfaces
  //get the background image
  if(!background.load("bckgnd.bmp"))return FALSE; //read from file
  background.draw(lpBackground); //draw to background surface
  //set palettes in all surfaces
  if(!background.setpalette(lpPrimaryPalette))return FALSE;
  if(!background.setpalette(lpSecondaryPalette))return FALSE;
  //load the text sprites
  if(!g_cTextImages.load("text.bmp"))return FALSE;//read from file
  Text=new CTextManager(19,30); //text manager
  LoadTextSprites();
  //load the sprites...
  if(!g_cSpriteImages.load("sprites.bmp"))return FALSE;
  //...the plane
  g_pSprite[PLANE_OBJECT]=new CClippedSprite(6,121,72);
  if(!LoadPlaneSprite())return FALSE; //load plane images
  //...the damaged plane
  g_pSprite[DAMAGEDPLANE_OBJECT]=new CClippedSprite(2,121,72);
  LoadDamagedPlaneSprite();
  //...the exploding plane
  g_pSprite[EXPLODINGPLANE_OBJECT]=new CClippedSprite(4,121,72);
  LoadExplodingPlaneSprite();
  //...the dead plane
  g_pSprite[DEADPLANE_OBJECT]=new CClippedSprite(1,119,54);
  LoadDeadPlaneSprite();
  //...the crow
  g_pSprite[CROW_OBJECT]=new CClippedSprite(4,58,37);
  if(!LoadCrowSprite())return FALSE; //load crow images
  //..the dead crow
  g_pSprite[DEADCROW_OBJECT]=new CClippedSprite(1,62,53);
  LoadDeadCrowSprite(); //load dead crow images
  //...the exploding crow
  g_pSprite[EXPLODINGCROW_OBJECT]=new CClippedSprite(6,62,53);
  LoadExplodingCrowSprite(); //load exploding crow images
  //...the bullet
  g_pSprite[BULLET_OBJECT]=new CClippedSprite(1,5,3);
  LoadBulletSprite(); //load bullet images
  //...the paused sprite
  PausedSprite=new CClippedSprite(1,220,64);
  LoadPausedSprite();
  //...the foreground sprites
  if(!g_cFrgndImages.load("farm.bmp"))return FALSE;
  g_pSprite[FARM_OBJECT]=new CClippedSprite(1,640,162);
  g_pSprite[FIELD_OBJECT]=new CClippedSprite(1,640,162);
  if(!LoadFrgndSprites())return FALSE; //load foreground
  return TRUE;
} //LoadImages

void CreateObjects(){
  const int reserved=6; //reserved positions for nonvermin
  int crowcount=5+level*3; //number of crows per level
  int i;
  ObjectManager.create(FARM_OBJECT,0,SCREEN_HEIGHT-1,0,0);
  ObjectManager.create(FIELD_OBJECT,SCREEN_WIDTH,
    SCREEN_HEIGHT-1,0,0);
  //compute numbers of objects
  if(crowcount>MAX_OBJECTS-reserved)
    crowcount=MAX_OBJECTS-reserved;
  if(crowcount<0)crowcount=0;
  //create objects
  for(i=0; i<crowcount/2; i++)
    ObjectManager.create(CROW_OBJECT,
      Random.number(0,WORLD_WIDTH-1),
      Random.number(100,400),-1,0);
  ObjectManager.set_current(
    ObjectManager.create(PLANE_OBJECT,320,271,-1,0));
  for(i=0; i<crowcount-crowcount/2; i++)
    ObjectManager.create(CROW_OBJECT,
      Random.number(0,WORLD_WIDTH-1),
      Random.number(100,400),-1,0);
} //CreateObjects

BOOL RestoreSurfaces(){ //restore all surfaces
  BOOL result=TRUE;
  //primary and secondary surfaces
  if(FAILED(lpPrimary->Restore()))return FALSE;
  if(FAILED(lpSecondary->Restore()))return FALSE;
  //surfaces containing images
  if(SUCCEEDED(lpBackground->Restore())) //if background restored
    result=result&&background.draw(lpBackground); //redraw image
  else return FALSE;
  if(g_pSprite[PLANE_OBJECT]->Restore()) //if plane restored
    result=result&&LoadPlaneSprite(); //redraw image
  else return FALSE;
  if(g_pSprite[DAMAGEDPLANE_OBJECT]->Restore()) //if restored
    result=result&&LoadDamagedPlaneSprite(); //redraw image
  else return FALSE;
  if(g_pSprite[EXPLODINGPLANE_OBJECT]->Restore()) //if restored
    result=result&&LoadExplodingPlaneSprite(); //redraw image
  else return FALSE;
  if(g_pSprite[DEADPLANE_OBJECT]->Restore()) //if restored
    result=result&&LoadDeadPlaneSprite(); //redraw image
  else return FALSE;
  if(g_pSprite[CROW_OBJECT]->Restore()) //if crow restored
    result=result&&LoadCrowSprite(); //redraw image
  else return FALSE;  
  if(g_pSprite[DEADCROW_OBJECT]->Restore()) //if restored
    result=result&&LoadDeadCrowSprite(); //redraw image
  else return FALSE;
  if(g_pSprite[EXPLODINGCROW_OBJECT]->Restore()) //if restored
    result=result&&LoadExplodingCrowSprite(); //redraw image
  else return FALSE;
  if(g_pSprite[BULLET_OBJECT]->Restore()) //if restored
    result=result&&LoadBulletSprite(); //redraw image
  else return FALSE;
  if(PausedSprite->Restore()) //if restored
    result=result&&LoadPausedSprite(); //redraw image
  else return FALSE;
  if(g_pSprite[FARM_OBJECT]->Restore()&& //if farm and ...
    g_pSprite[FIELD_OBJECT]->Restore()) //... field restored
    result=result&&LoadFrgndSprites(); //redraw image
  else return FALSE;
  //input manager has button sprites
  if(!InputManager.Restore())return FALSE;
  //text manager has text sprites
  if(Text->Restore())
    result=result&&LoadTextSprites();
  else return FALSE;
  return result;
} //RestoreSurfaces

void LoadSounds(int level=0){ //load sounds for level
  const int copies=4; //copies of repeatable sounds
  switch(level){
    case 0: //intro sounds
      SoundManager->load("intro.wav");
      SoundManager->load("larc.wav");
      SoundManager->load("credits.wav");
      break;
    case 1: //game engine sounds
      SoundManager->load("caw.wav",copies);
      SoundManager->load("gun.wav",copies);
      SoundManager->load("boom.wav",copies);
      SoundManager->load("thump.wav",copies);
      SoundManager->load("putt0.wav");
      SoundManager->load("putt1.wav");
      SoundManager->load("putt2.wav");
      SoundManager->load("bgclk.wav",2);
      SoundManager->load("smclk.wav",2);
      SoundManager->load("success.wav");
      SoundManager->load("failed.wav");
      SoundManager->load("life.wav");
      SoundManager->load("stings.wav");
      SoundManager->load("smarts.wav");
      SoundManager->load("congrats.wav");
      SoundManager->load("twspace.wav",copies);
      SoundManager->load("twret.wav",copies);
      SoundManager->load("twchar.wav",copies);
      SoundManager->load("pageturn.wav",2);
      break;
  }
}

BOOL PageFlip(){ //return TRUE if page flip succeeds
  if(lpPrimary->Flip(NULL,DDFLIP_WAIT)==DDERR_SURFACELOST)
    return RestoreSurfaces();
  return TRUE;
} //PageFlip

int chars_in_number(int n){ //return number of chars
  if(n==0)return 1; //0 has one character
  int count=0; //count of number of characters
  n=abs(n); //positive numbers only
  while(n>0){n/=10; count++;} //tally number of chars
  return count;
} //chars_in_number

void draw_number(int n,int center,
    LPDIRECTDRAWSURFACE surface,BOOL middle=FALSE){
  const int HALF_CHAR_WIDTH=10;
  char buffer[64]; //for display on screen
  char buffer2[16]; //temp buffer
  int margin=7;
  int width=chars_in_number(n); //width of number in chars
  for(int i=0; i<center-width/2-1; i++)
    buffer[i]=' '; //shift right
  buffer[i]=0; //end of string
  sprintf(buffer2,"%d",n);
  strcat(buffer,buffer2); //n to buffer
  //adjust to center
  if(!(width&1))margin+=HALF_CHAR_WIDTH;
  if(middle)margin-=HALF_CHAR_WIDTH;
  //draw
  Text->jump(margin,75); //start of line
  Text->draw(buffer,surface); //print number
}

void draw_text_header(LPDIRECTDRAWSURFACE surface){
  const int margin=7;
  Text->jump(margin,40);
  Text->draw("level varmints score health lives",surface);
  draw_number(level,3,surface);
  draw_number(ObjectManager.enemies(),11,surface,TRUE);
  draw_number(ScoreManager.score(),18,surface);
  draw_number(ScoreManager.health(),25,surface,TRUE);
  draw_number(ScoreManager.lives(),31,surface);
}

BOOL ComposeFrame(){ //compose a frame of animation
  Viewpoint.draw_background(lpBackground,lpSecondary,
    ObjectManager.speed()); //draw scrolling background
  ObjectManager.animate(lpSecondary); //draw objects
  draw_text_header(lpSecondary); //draw text header
  if(show_framerate){ //if we need to show the frame count
    char buffer[4];
    if(!GamePaused)framecount++; //count frame
    if(Timer.elapsed(framerate_timer,1000)){
      last_framecount=framecount; framecount=0;
    }
    Text->jump(580,470);
    sprintf(buffer,"%d",last_framecount);
    Text->draw(buffer,lpSecondary);
  }
  //display word "paused" if game is paused
  if(GamePaused)PausedSprite->draw(0,320,200,lpSecondary);
  return TRUE;
} //ComposeFrame

void display_screen(char *filename){ //display bmp file
  CBmpFileReader image; //file reader
  image.load(filename); //load from file
  image.draw(lpSecondary); //draw to back buffer
  image.setpalette(lpPrimaryPalette); //may have custom palette
  PageFlip(); //display it
 //draw to back buffer again for button animation
  image.draw(lpSecondary);
} //display_screen

void display_highscorelist(char *filename){ //display high scores
  display_screen(filename); //display background
  HighScoreList.draw(lpSecondary); //draw high scores
  PageFlip(); //flip to front surface
} //display_highscorelist

void display_newhighscore(){ //display new high score entry page
  Sleep(50); //slow down the animation
  NewHighScoreBg->draw(lpSecondary); //draw background
  //draw text
  Text->jump(163,330); 
  Text->draw(HighScoreList.current_name(),lpSecondary);
  PageFlip(); //display it
} //display_newhighscore

void change_phase(GamePhaseType new_phase){ //go to new phase
  switch(new_phase){
    case LOGO_PHASE:
      display_screen("larc.bmp");
      LoadSounds(); //load sounds for intro sequence
      SoundManager->play(LOGO_SOUND); //signature chord
      break;
    case TITLE_PHASE: 
      display_screen("title.bmp");
      SoundManager->stop(); //silence previous phase
      SoundManager->play(TITLE_SOUND); //title sound
      break;
    case CREDITS_PHASE:
      display_screen("credits.bmp");
      SoundManager->stop(); //silence previous phase
      SoundManager->play(CREDITS_SOUND); //credits sound
      break;
    case MENU_PHASE: 
      //set up buttons for menu
      InputManager.SetupButtons(MENU_PHASE); 
      //sound
      switch(GamePhase){ //depending on previous phase
        case CREDITS_PHASE: //from intro phase
          SoundManager->stop(); //silence previous phase
          SoundManager->clear(); //clear out old sounds
          LoadSounds(1); //set up game sounds
          break;
        case PLAYING_PHASE: //from game engine
          SoundManager->stop(); //silence game engine
          if(ScoreManager.lives()<0)
            SoundManager->play(FAILED_SOUND); 
          break;
        case DEVICEMENU_PHASE: //device menu, do nothing
          break;
        case NEWHIGHSCORE_PHASE:
          delete NewHighScoreBg; //delete background
          break;
      }
      //display background
      display_screen("menu.bmp"); //display main menu
      //mouse cursor
      switch(GamePhase){ //what phase did we come in from?
        case PLAYING_PHASE:
        case CREDITS_PHASE:
        case HIGHSCORE_PHASE: 
        case NEWHIGHSCORE_PHASE:
        case HELP_PHASE:
          ShowCursor(TRUE); //activate the mouse cursor
          break;
      }
      break;
    case DEVICEMENU_PHASE:
      //display button background
      display_screen("dmenu.bmp");
      //set up the button manager
      InputManager.SetupButtons(DEVICEMENU_PHASE);
      if(GamePhase==PLAYING_PHASE)ShowCursor(TRUE);
      break;
    case HELP_PHASE: //display first help page
      if(GamePhase==MENU_PHASE)ShowCursor(FALSE);
      display_screen("help000.bmp"); //show first page
      HelpPage=0; FlipHelpPage=FALSE; //reset to first page
      break;
    case PLAYING_PHASE: //prepare the game engine
      //mouse cursor
      SetCursorPos(SCREEN_WIDTH-1,SCREEN_HEIGHT/2); //throttle
      switch(GamePhase){//depending on previous phase
        case MENU_PHASE:
          SoundManager->stop(); //silence previous phase
          ShowCursor(FALSE); //deactivate the mouse cursor
          level=1; ScoreManager.reset(); //reset score
          //create objects in game engine
          ObjectManager.reset(); //clear object manager
          CreateObjects(); //create new objects
          break;
        case SUCCESS_PHASE:
          SoundManager->stop(); //silence previous phase
          level++; //go to next level
          //add score for level completion
          ScoreManager.add_to_score(LEVEL_BONUS_SCORE);
          //create objects in game engine
          ObjectManager.reset(); //clear object manager
          CreateObjects(); //create new objects
          break;
        case DEVICEMENU_PHASE:
          ShowCursor(FALSE);
          InputManager.unpause();
          break;
        case HIGHSCORE_PHASE:
        case HELP_PHASE:
          InputManager.unpause();
          break;
      }
      //initialize graphics
      background.setpalette(lpPrimaryPalette); //game palette
      ComposeFrame(); PageFlip(); //prime animation pump
      break;
    case SUCCESS_PHASE: //successful completion of level
      SoundManager->stop(); //stop sounds from previous phase
      SoundManager->play(SUCCESS_SOUND); //jubilation
      display_screen("success.bmp"); //display success screen
      break;
    case HIGHSCORE_PHASE: //display high score list
      if(GamePhase==MENU_PHASE)ShowCursor(FALSE); //hide cursor
      display_highscorelist("hscore.bmp"); //display high scores
      break;
    case NEWHIGHSCORE_PHASE: //display high score list
      SoundManager->stop(); //stop sounds from previous phase
      SoundManager->play(CONGRATS_SOUND); //play new sound
      NewHighScoreBg=new CBmpFileReader; //background
      NewHighScoreBg->load("newhsc.bmp"); //read from file
      NewHighScoreBg->draw(lpSecondary); //draw to back surface
      NewHighScoreBg->setpalette(lpPrimaryPalette); //palette
      PageFlip(); //display it
      break;
  }
  //change to new phase
  GamePhase=new_phase; PhaseTime=Timer.time();
  endphase=FALSE;
} //change_phase  

void display_helpscreen(){ 
  //construct help file name
  char filename[16];
  strcpy(filename,"help000.bmp");
  filename[6]='0'+HelpPage;
  //display the help file
  display_screen(filename);
}

void Redraw(){ //redraw in response to surface loss
  switch(GamePhase){
    case LOGO_PHASE:
      display_screen("larc.bmp");
      break;
    case TITLE_PHASE:
      display_screen("title.bmp");
      break;
    case CREDITS_PHASE:
      display_screen("credits.bmp");
      break;
    case MENU_PHASE:
      display_screen("menu.bmp"); //display main menu
      break;
    case PLAYING_PHASE:
      //do nothing, next frame of animation will catch it
      break;
    case DEVICEMENU_PHASE:
      display_screen("dmenu.bmp");
      InputManager.SetupButtons(DEVICEMENU_PHASE);
      break;
    case SUCCESS_PHASE:
      display_screen("success.bmp"); //display success screen
      break;
    case HIGHSCORE_PHASE:
      display_highscorelist("hscore.bmp"); //display high scores
      break;
    case NEWHIGHSCORE_PHASE:
      NewHighScoreBg->setpalette(lpPrimaryPalette); //palette
      display_newhighscore();
      break;
    case HELP_PHASE:
      display_helpscreen();
      break;
  }
}

void ProcessFrame(){ //process a frame of animation
  const int LOGO_DISPLAY_TIME=8500; //duration of logo
  const int TITLE_DISPLAY_TIME=10000; //duration of title
  const int CREDITS_DISPLAY_TIME=8000; //duration of credits
  const int SUCCESS_DISPLAY_TIME=4500; //duration of success
  InputManager.Joystick(); //process joystick input
  //check for lost surfaces, eg alt+tab
  if(lpPrimary->IsLost()){
    RestoreSurfaces(); Redraw();
  }
  //phase-related processing
  switch(GamePhase){ //what phase are we in?
    case LOGO_PHASE: //displaying logo screen
      Sleep(100); //surrender time to other processes
      if(endphase||Timer.elapsed(PhaseTime,LOGO_DISPLAY_TIME))
        change_phase(TITLE_PHASE); //go to title screen
      break;
    case TITLE_PHASE: //displaying title screen
      Sleep(100); //surrender time to other processes
      if(endphase||Timer.elapsed(PhaseTime,TITLE_DISPLAY_TIME))
        change_phase(CREDITS_PHASE); //go to credits
      break;
    case CREDITS_PHASE: //displaying title screen
      Sleep(100); //surrender time to other processes
      if(endphase||Timer.elapsed(PhaseTime,CREDITS_DISPLAY_TIME))
        change_phase(MENU_PHASE); //go to menu
      break;
    case MENU_PHASE: //main menu
      Sleep(100); //surrender time to other processes
      if(endphase)change_phase(NextPhase); //change phase
      break;
    case DEVICEMENU_PHASE:
      if(endphase)
        if(GamePaused)change_phase(PLAYING_PHASE); //resume
        else change_phase(MENU_PHASE); //go to menu phase
      break;
    case PLAYING_PHASE: //game engine
      if(GamePaused)Sleep(100); //be nice if paused
      ComposeFrame(); //compose a frame in back surface
      PageFlip(); //flip video memory surfaces
      if(ObjectManager.won()) //completed level
        change_phase(SUCCESS_PHASE);
      else if(endphase){ //stop playing
        //mouse to screen center
        SetCursorPos(SCREEN_WIDTH/2,SCREEN_HEIGHT/2); 
        if(GamePaused)change_phase(NextPhase); //in a menu
        else{ //quit or died, check for new high score
         if(ScoreManager.lives()>=0&&
			  HighScoreList.made_the_grade(ScoreManager.score()))
            change_phase(NEWHIGHSCORE_PHASE); //user enters name
          else change_phase(MENU_PHASE); //quit, go to menu
        }
      }
      break;
    case SUCCESS_PHASE: //displaying title screen
      Sleep(100); //surrender time to other processes
      if(endphase|| //if end of phase
      Timer.elapsed(PhaseTime,SUCCESS_DISPLAY_TIME))
        change_phase(PLAYING_PHASE); //start playing
      break;
    case HIGHSCORE_PHASE: //displaying high scores
      Sleep(100); //surrender time to other processes
      if(endphase)
        if(GamePaused)change_phase(PLAYING_PHASE); //back to game
        else change_phase(MENU_PHASE); //quit, go to menu
      break;
    case NEWHIGHSCORE_PHASE:
      display_newhighscore(); //animate user typing in name
      if(endphase){ //insert new high score
        HighScoreList.insert(level,ScoreManager.score());
        change_phase(MENU_PHASE); //go to menu
      }
      break;
    case HELP_PHASE: //display help
      if(FlipHelpPage){ //turn the page
        FlipHelpPage=FALSE; //don't do it again
        if(++HelpPage>=NUMHELPPAGES)endphase=TRUE; //last page
        else{ //next page
          SoundManager->play(PAGETURN_SOUND); //page flip sound
          display_helpscreen(); //display help screen
        }
      }
      if(endphase) //clicked out
        if(GamePaused)change_phase(PLAYING_PHASE); //back to game
        else change_phase(MENU_PHASE); //quit, go to menu
      break;
  }
} //ProcessFrame

//message handler (window procedure)
long CALLBACK WindowProc(HWND hwnd,UINT message,
                         WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_ACTIVATEAPP:
      ActiveApp=wParam; 
      //pause if swapped out while playing
      if(!ActiveApp&&GamePhase==PLAYING_PHASE&&!GamePaused)
        InputManager.pause();
      break;
    case WM_CREATE: break;
    case WM_KEYDOWN: //keyboard hit
      if(InputManager.Keyboard(wParam))
        DestroyWindow(hwnd);
      break;
    case WM_LBUTTONDOWN: //left mouse button down
      InputManager.LMouseDown(lParam); //handle it
      break;
    case WM_LBUTTONUP: //left mouse button up
      if(InputManager.LMouseUp(lParam))
        DestroyWindow(hwnd);
      break;
    case WM_MOUSEMOVE: //mouse move
      InputManager.MouseMove(lParam);
      break;
    case WM_DESTROY: //end of game
      ObjectManager.reset(); //clear objects
      if(lpDirectDrawObject!=NULL){ //if DD object exists
        if(lpSecondary!=NULL) //if secondary surface exists
          lpSecondary->Release(); //release secondary surface
        if(lpPrimary!=NULL) //if primary surface exists
          lpPrimary->Release(); //release primary surface
        if(lpBackground!=NULL) //if background exists
          lpBackground->Release(); //release background
        for(int i=0; i<NUM_SPRITES; i++){ //for each sprite
          if(g_pSprite[i]) //if sprite exists
            g_pSprite[i]->Release(); //release sprite
          delete g_pSprite[i]; //delete sprite
        }
        InputManager.Release(); //button surfaces
        Text->Release();  //release text manager sprites
        PausedSprite->Release(); delete PausedSprite;
        lpDirectDrawObject->Release(); //release DD object
      }
      delete SoundManager; //reclaim sound manager memory
      delete Text; //reclaim text manager memory
      ShowCursor(TRUE); //show the mouse cursor
      PostQuitMessage(0); //and exit
      break;
    default: //default window procedure
      return DefWindowProc(hwnd,message,wParam,lParam);
  } //switch(message)
  return 0L;
} //WindowProc

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
LPSTR lpCmdLine,int nCmdShow){
  MSG msg; //current message
  HWND hwnd; //handle to fullscreen window
  hwnd=CreateDefaultWindow("directX demo 13",hInstance);
  if(!hwnd)return FALSE;
  //set up window
  ShowWindow(hwnd,nCmdShow); UpdateWindow(hwnd);
  SetFocus(hwnd); //allow input from keyboard
  ShowCursor(FALSE); //hide the cursor
  //init graphics  
  for(int i=0; i<NUM_SPRITES; i++) //null out sprites
    g_pSprite[i]=NULL;
  BOOL OK=InitDirectDraw(hwnd);//initialize DirectDraw
  if(OK)OK=LoadImages(); //load images from disk
  if(!OK){ //bail out if initialization failed
    DestroyWindow(hwnd); return FALSE;
  }
  //start game timer
  Timer.start();
  //init sound
  SoundManager=new CGameSoundManager(hwnd);
  //set initial phase
  change_phase(LOGO_PHASE);
  //message loop
  while(TRUE)
    if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
      if(!GetMessage(&msg,NULL,0,0))return msg.wParam;
      TranslateMessage(&msg); DispatchMessage(&msg);
    }
    else if(ActiveApp)ProcessFrame(); else WaitMessage();
} //WinMain
