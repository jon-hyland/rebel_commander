/*
 * TO-DO:
 *  - add yes/no "are u sure u want to quit" dialog and a "credits" dialog
 */

#include "game.h"	//include any public definitions in game.h header...
#include "_game.h"	//...now include all private game stuff
// the world
#include "_mapdata.h"
#include "_mapdata_global.h"
#include "_gamedata.h"	//...include all game data

// Globals ////////////////////////////////////////////////////////////////////
extern WORLD world;
extern GOAL goal;
extern PLAYER pc;
int gameState;
const int TRIG_NUM_EVAL = 20;
float tblCOS[TRIG_NUM_EVAL];
float tblSIN[TRIG_NUM_EVAL];
LPPOPMSG currentMsg=NULL;	//currently showing message (if any)
void (*ProcessInputKeyboard)(LPPLAYER , unsigned char* );
GUIWindow* guiMenu=NULL;
GUIWindow* guiInstructions=NULL;
IDirectDrawSurface4* lpddsScreen;
IDirectDrawSurface4* lpddsSnapShot;
BOB areaMapTiles[AREA_HEIGHT][AREA_WIDTH];			// holds tiles of currently viewed area [rows][cols]
BOB areaMapTilesLayer1[AREA_HEIGHT_L1][AREA_WIDTH_L1];
// palettes, so we don't have to reload all the time
PALETTEENTRY game_palette[256]; // holds the main game palette
// projectiles
const int MAX_PROJ=50;
int numProjectilesAllocd=0;
PROJ bullets[MAX_PROJ];
// main screen vars
BITMAP_IMAGE botBG;
// inventory screen vars
BITMAP_IMAGE inventBG;
PALETTEENTRY inventPalette[256];
// NPCs
int NUM_NPC=0;
const int MAX_NUM_NPC = 100;
LPNPC npcs[MAX_NUM_NPC];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// GAME ROUTINES ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----- code includes -----//
#include "snddefs.h"
#include "game_fileio.h"
#include "list.h"
#include "input.h"
#include "proj.h"
#include "world.h"
#include "goals.h"
#include "ai.h"
#include "msg.h"
//-------------------------//

INLINE int round(float x)  { return (int)(x+0.5); }
INLINE int round(double x) { return (int)(x+0.5); }

void InitLUTs()
{
	int i;
	const double PI = 3.1415926535897932384626433832795;
	const double f = PI/(TRIG_NUM_EVAL-1);
	
	for (i=0; i<TRIG_NUM_EVAL; i++) {
		tblCOS[i]=(float)cos(i*f);
		tblSIN[i]=(float)sin(i*f);
	}
}

void InitGlobalSounds()
{
	//background midi
	snd_ambact1 = DMusic_Load_MIDI("SND/AMBIENT_ACTION1.MID");
	snd_music[0] = DMusic_Load_MIDI("SND/MUSIC1.MID");
	snd_music[1] = DMusic_Load_MIDI("SND/MUSIC2.MID");
	snd_music[2] = DMusic_Load_MIDI("SND/MUSIC3.MID");
	snd_music[3] = DMusic_Load_MIDI("SND/MUSIC4.MID");
	snd_music[4] = DMusic_Load_MIDI("SND/MUSIC5.MID");
	snd_music[5] = DMusic_Load_MIDI("SND/MUSIC6.MID");
	snd_npcdead = DSound_Load_WAV("SND/THUD.WAV");
	snd_pcdead = DSound_Load_WAV("SND/ARGH.WAV");
	snd_cmdclick = DSound_Load_WAV("SND/CMDCLICK.WAV");
	snd_expld[0] = DSound_Load_WAV("SND/EXPL1.WAV");
	snd_expld[1] = DSound_Load_WAV("SND/EXPL2.WAV");
	snd_shotgun = DSound_Load_WAV("SND/SHOTGUN.WAV");
	//DSound_Set_Freq(snd_shotgun,22050);
	DSound_Set_Volume(snd_shotgun,75);
	snd_handgun = DSound_Load_WAV("SND/HANDGUN2.WAV");
	snd_silence_handgun = DSound_Load_WAV("SND/HANDGUN2.WAV");
	DSound_Set_Freq(snd_silence_handgun,88200);
	DSound_Set_Volume(snd_silence_handgun,80);
	snd_machinegun = DSound_Load_WAV("SND/MACHINEGUN.WAV");
	snd_empty = DSound_Load_WAV("SND/EMPTY.WAV");
	snd_doorslide = DSound_Load_WAV("SND/DOORSLIDE.WAV");
	snd_siren = DSound_Load_WAV("SND/SIREN1.WAV");
	//play intro music
	DMusic_Play(snd_music[0]);
}

int CreatePlayer(LPPLAYER player, LPWORLD world)
{
	int index;
	//load tileset bitmap and set to use its palette...
	Load_Bitmap_File(&bitmap8bit, "ART/TILESET01_8B.bmp");
	//set the palette to background image palette
	Set_Palette(bitmap8bit.palette);
	//save this palette
	Save_Palette(game_palette);
	//....unload tileset bitmap
	Unload_Bitmap_File(&bitmap8bit);
	//load character bitmap...
	Load_Bitmap_File(&bitmap8bit, "ART/TSCHAR01_8B.bmp");
	int numframe=8;
	//now create the bob
	Create_BOB(&player->bob,0,0,PC_SPRITEW,PC_SPRITEH,numframe,
			   BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM,
			   DDSCAPS_SYSTEMMEMORY);
	//load frames
	for (index=0; index<numframe; index++)
		Load_Frame_BOB(&player->bob,&bitmap8bit,index,(index+8)%8,(index+8)/8,BITMAP_EXTRACT_MODE_CELL);
	//set animations sequences up
	int walkup[2]={0,1};
	int walkdn[2]={2,3};
	int walkrt[2]={4,5};
	int walklt[2]={6,7};
	Load_Animation_BOB(&player->bob,0,2,walkup);
	Load_Animation_BOB(&player->bob,1,2,walkdn);
	Load_Animation_BOB(&player->bob,2,2,walkrt);
	Load_Animation_BOB(&player->bob,3,2,walklt);
	//unload
	Unload_Bitmap_File(&bitmap8bit);
	//set up player struct
	player->state=0;
	player->keyDelay=KEY_DELAY;
	player->energy=50;
	player->score=0;
	player->lives=100;
	player->height=32;
	player->width=32;
	player->facing=PC_FACE_E;
	Set_Animation_BOB(&player->bob,2);
	ListInit(&player->invent);
	player->currEquip=NULL;
	player->currWep=NULL;
	player->mappos.x=0;
	player->mappos.y=0;
	player->x=180;
	player->y=375;
	player->vx=0;
	player->vy=0;
	player->fireDelay=0;
	player->invicibleTicks=0;
	//set area of player pos
	player->mapposArea = GetWorldArea(world,pc.mappos.x,pc.mappos.y,pc.mappos.z);
	//set position
	Set_Pos_BOB(&player->bob,pc.x-PC_SPRITEW/2,pc.y-PC_SPRITEH+16);
	//set animation speed
	Set_Anim_Speed_BOB(&player->bob,PC_ANIMSPEED);
	return SUCCESS;
}

void DrawPlayerStats(LPPLAYER player)
{
	char txt[128],txtfrag[33];

	strcpy(txt,"Position: ");
	strcat(txt,itoa(pc.mappos.x,txtfrag,10));
	strcat(txt,", ");
	strcat(txt,itoa(pc.mappos.y,txtfrag,10));
	strcat(txt,", ");
	strcat(txt,itoa(pc.mappos.z,txtfrag,10));
	strcat(txt,"\n");
	strcat(txt,"Energy=");
	strcat(txt,itoa(pc.energy,txtfrag,10));
	strcat(txt,"\n");
	strcat(txt,"Score=");
	strcat(txt,itoa(pc.score,txtfrag,10));
	Draw_Text_GDI(txt,0,0,0,lpddsback);
	Draw_Text_GDI(txt,1,1,255,lpddsback);
}

void InitHUD() {}
void DrawHUD() {}

void InitBorder()
{
	// load image
	Load_Bitmap_File(&bitmap8bit, "ART/BOTSCREEN.BMP");
	Create_Bitmap(&botBG,0,AREA_HEIGHT*TILEH,800,24);
	Load_Image_Bitmap(&botBG,&bitmap8bit,0,0,BITMAP_EXTRACT_MODE_ABS);
	// unload object bitmap
	Unload_Bitmap_File(&bitmap8bit);
}
void DrawBorder()
{
	//draw bg bottom
	back_buffer=DDraw_Lock_Back_Surface();
	Draw_Bitmap(&botBG,back_buffer, back_lpitch,0);
	DDraw_Unlock_Back_Surface();
}

void InitInvent()
{
	// load image
	Load_Bitmap_File(&bitmap8bit, "ART/INVENTBG.BMP");
	// save the palette
	Copy_Palette(inventPalette, bitmap8bit.palette);
	Create_Bitmap(&inventBG,0,0,800,600);
	Load_Image_Bitmap(&inventBG,&bitmap8bit,0,0,BITMAP_EXTRACT_MODE_ABS);
	// unload object bitmap
	Unload_Bitmap_File(&bitmap8bit);
}
void DrawInvent(LPPLAYER player)
{
	const int ITEM_WIDTH = 32;
	const int ITEM_HEIGHT = 32;
	char txtfrag[50];
	int wepInd=0,equipInd=0;
	int wepX=50, wepY=128;
	int wepCols=1;
	int equipX=400, equipY=128;
	int equipCols=1;
	LPOBJECT item=NULL;
	int i;
	//draw bg
	back_buffer=DDraw_Lock_Back_Surface();
	Draw_Bitmap(&inventBG,back_buffer, back_lpitch,0);
	DDraw_Unlock_Back_Surface();
	//draw items
	for (i=0; i<player->invent->size; i++) {
		item=player->invent->list[i];
		if (item->type & ID_WEP) {
			Draw_BOB(&item->bob,lpddsback,1,wepX,(wepInd/wepCols)*ITEM_HEIGHT+wepY);
			Draw_Text_GDI(item->name,wepX+100,(wepInd/wepCols)*ITEM_HEIGHT+wepY,255,lpddsback);
			wepInd++;
		}
		else if (item->type & ID_AMMO) {
			Draw_BOB(&item->bob,lpddsback,1,wepX,(wepInd/wepCols)*ITEM_HEIGHT+wepY);
			Draw_Text_GDI(itoa(item->quantity,txtfrag,10),wepX,(wepInd/wepCols)*ITEM_HEIGHT+wepY,255,lpddsback);
			Draw_Text_GDI(item->name,wepX+100,(wepInd/wepCols)*ITEM_HEIGHT+wepY,255,lpddsback);
			wepInd++;
		}
		else if (item->type & ID_EQUIP) {
			Draw_BOB(&item->bob,lpddsback,1,equipX,(equipInd/equipCols)*ITEM_HEIGHT+equipY);
			Draw_Text_GDI(item->name,equipX+100,(equipInd/equipCols)*ITEM_HEIGHT+equipY,255,lpddsback);
			equipInd++;
		}
	}
}

// call-backs for cmd buttons //
void cmdInstruck_InstrOK_OnClick()
{
	DSound_Play(snd_cmdclick);
	delete guiInstructions;
	guiInstructions=NULL;
}
void cmdInstruck_Start_OnClick()
{
	DSound_Play(snd_cmdclick);
	static int first = 1;
	gameState=GS_RUNNING;
	ProcessInputKeyboard=ProcessInputKeyboard_Normal;
	if (first) {
		DMusic_Play(snd_music[3]);
		first=0;
	}
}
void cmdInstruck_Quit_OnClick()
{
	DSound_Play(snd_cmdclick);
	gameState=GS_EXIT;
}
void cmdInstruck_Instr_OnClick()
{
	DSound_Play(snd_cmdclick);
	if (guiInstructions==NULL) {	//if window not up
		guiInstructions=new GUIWindow(lpdd,lpddsback,&mouse_state,100,150,700,450,"ART/INSTRUCTIONS.BMP","ART/CURSOR.BMP");
		guiInstructions->addButton(475,250,100,32,0,cmdInstruck_InstrOK_OnClick,"ART/BUTTONOKUP.BMP","ART/BUTTONOKDN.BMP");
	}
}

int  Game_Reset(void *params)
{
	//Game_Shutdown();
	//Game_Init();
	return 0;
}
int  Game_Init(void *params)
{
	int index;

	//Open_Error_File("DEBUG.TXT");
	//
	// seed random number generator
	srand(GetTickCount());
	// initialize directdraw
	DDraw_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
	// initialize directsound
	DSound_Init();
	// initialize directmusic
	DMusic_Init();
	// initialize directinput
	DInput_Init();
	// initialize all input devices
	DInput_Init_Keyboard();
	DInput_Init_Mouse();
	// set clipping rectangle to screen extents
	RECT screen_rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	lpddclipper = DDraw_Attach_Clipper(lpddsback,1,&screen_rect);
	// hide the mouse cursor
	ShowCursor(FALSE);
	//---------------------------//
	InitGlobalSounds();
	//make sure projectiles are invalid
	for (index=0; index<MAX_PROJ; index++) {
		bullets[index].x=bullets[index].y=-1;
	}
	// create GUI
	guiMenu=new GUIWindow(lpdd,lpddsback,&mouse_state,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,"ART/MAINMENUBG.BMP","ART/CURSOR.BMP");
	guiMenu->addButton(100,300,128,32,0,cmdInstruck_Start_OnClick,"ART/BUTTONSTARTUP.BMP","ART/BUTTONSTARTDN.BMP");
	guiMenu->addButton(100,364,128,32,0,cmdInstruck_Instr_OnClick,"ART/BUTTONINSTRUP.BMP","ART/BUTTONINSTRDN.BMP");
	guiMenu->addButton(100,430,128,32,0,cmdInstruck_Quit_OnClick,"ART/BUTTONQUITUP.BMP","ART/BUTTONQUITDN.BMP");
	InitLUTs();
	InitNPCs();
	CreateWorld(&world);
	CreatePlayer(&pc,&world);
	InitAreaMap();
	InitInvent();	//initialize inventory screen stuff
	//InitBorder();
	InitHUD();
	CreateAreaMap(pc.mapposArea);
	//---------------------------//
	//gameState=GS_RUNNING;
	gameState=GS_MENU;
	ProcessInputKeyboard=ProcessInputKeyboard_Menu;
	return SUCCESS;
}
int  Game_Shutdown(void *params)
{
	int index;
	//--------------clean up the mess i made---------------//
	// destroy player
	Destroy_BOB(&pc.bob);
	for(index=0; index<pc.invent->size; index++) {
		if (pc.invent->list[index]->attr!=NULL) free(pc.invent->list[index]->attr);
		free(pc.invent->list[index]);
	}
	free(pc.invent);
	//destroy NPCs
	KillNPCs();
	//destroy area map tile BOBs
	for(index=0; index<AREA_WIDTH*AREA_HEIGHT; index++) {
		Destroy_BOB( &(((BOB*)areaMapTiles)[index]) );
	}
	for(index=0; index<AREA_WIDTH_L1*AREA_HEIGHT_L1; index++) {
		Destroy_BOB( &(((BOB*)areaMapTilesLayer1)[index]) );
	}
	FreeWorld(&world);
	//
	delete guiMenu;
	//stop & delete all sounds
	DSound_Stop_All_Sounds();
	DSound_Delete_All_Sounds();
	//stop & delete all music
	DMusic_Delete_All_MIDI();
	//-----------------------------------------------------//
	// shutdown directdraw
	DDraw_Shutdown();
	// shutdown directsound
	DSound_Shutdown();
	// shutdown directmusic
	DMusic_Shutdown();
	// release all input devices
	DInput_Release_Keyboard();
	// shutdown directinput
    DInput_Shutdown();
	//Close_Error_File();
	return SUCCESS;
}
int  Game_Main(void *params)
{
	RECT dest_rect,   // the destination rectangle
		 source_rect; // the source rectangle
	int i;
	int anyNPCAlerted=0;
	static int playerdeadmsg=0;

	switch (gameState) {
	case GS_MENU:
		// clear the drawing surface
		DDraw_Fill_Surface(lpddsback, 0);
		// read all input devices
		DInput_Read_Keyboard();
		DInput_Read_Mouse();
		// process keyboard events
		ProcessInputKeyboard(&pc, keyboard_state);
		// draw GUI
		if (guiInstructions!=NULL) {	//if window is up
			guiInstructions->update();
		}
		else {
			guiMenu->update();
		}
		guiMenu->draw();
		if (guiInstructions!=NULL) guiInstructions->draw();
		// flip the surfaces
		DDraw_Flip();
		break;
	case GS_PLAYERDEAD:
		if (playerdeadmsg==0) {
			//play death sound
			DSound_Play(snd_pcdead);
			//show message
			char **msgs=(char**)malloc(sizeof(char*)*1);
			msgs[0]="* Game Over!! *\nBetter luck next time...";
			currentMsg=CreateMessage(msgs,1,300,200,150,50);
			ShowMessages();
			playerdeadmsg=1;
		}
		else {
			//clear message
			//FREE(currentMsg->bmpImage);
			FREE(currentMsg);
			//exit game
			gameState=GS_EXIT;
		}
		break;
	case GS_RUNNING:
		// start the timing clock
		Start_Clock();
		// clear the drawing surface
		DDraw_Fill_Surface(lpddsback, 0);
		// read all input devices
		DInput_Read_Keyboard();
		// process keyboard events
		ProcessInputKeyboard(&pc, keyboard_state);
		//---------------------------//
		//check if player dead :(
		if (pc.energy<=0) {
			gameState=GS_PLAYERDEAD;
		}
		//
		CheckObjectInteractions(&pc);
		CheckGoalConditions(&goal,&pc);
		CheckInternalConditions(&goal,&pc);
		for (i=0; i<MAX_NUM_NPC; i++)
			if (npcs[i]!=NULL) {
				DoAITick(npcs[i],&pc);
				if (npcs[i]!=NULL && npcs[i]->aiState!=AI_STATE_NORM) {
					anyNPCAlerted=1;
					//alert others in area
					AlertNPCs(npcs[i]->mapposArea);
				}
			}
		//check music
		if (DMusic_Status_MIDI(snd_ambact1)==MIDI_PLAYING && !anyNPCAlerted) {
			DMusic_Stop(snd_ambact1);
			DSound_Stop_Sound(snd_siren);
			DMusic_Play(snd_music[3]);
		}
		//reset any bits of player state after this check
		if (pc.state & PC_STATE_NOISY) {
			pc.state = pc.state & ~PC_STATE_NOISY;
		}
		//
		DrawAreaMap();						//draw area map
		DrawAreaItems(pc.mapposArea, &pc);	//draw all items in players area
		DrawPlayerStats(&pc);				//draw player
		DrawNPCsAndPC(&pc);					//draw NPCs and PC
		AnimateProjectiles();
		DrawProjectiles(&pc);
		//DrawBorder();
		DrawHUD();
		//---------------------------//
		// flip the surfaces
		DDraw_Flip();
		// sync to 30ish fps
		Wait_Clock(33);
		break;
	case GS_INVENTSCREEN:
		// clear the drawing surface
		DDraw_Fill_Surface(lpddsback, 0);
		//draw inventory screen
		DrawInvent(&pc);
		// read all input devices
		DInput_Read_Keyboard();
		//process keyboard events
		ProcessInputKeyboard(&pc, keyboard_state);
		DDraw_Flip();
		break;
	case GS_TRANS_MSGPOPUP:
		//save screen to snapshot bkground
		// fill in the destination rect
		dest_rect.left   = 0;
		dest_rect.top    = 0;
		dest_rect.right  = VP_WIDTH;
		dest_rect.bottom = VP_HEIGHT;
		// fill in the src rect
		source_rect = dest_rect;
		//blt the primary surface onto snapshot surface
		lpddsSnapShot->Blt(&dest_rect,lpddsprimary,&source_rect,(DDBLT_WAIT),NULL);		// | DDBLT_KEYSRC
		gameState=GS_MSGPOPUP;
		break;
	case GS_MSGPOPUP:
		// clear the drawing surface
		DDraw_Fill_Surface(lpddsback, 0);
		// read all input devices
		DInput_Read_Keyboard();
		//process keyboard events
		ProcessInputKeyboard(&pc, keyboard_state);
		//
		if (currentMsg->state==POPMSG_STATE_CLOSED && !currentMsg->visible) {
			//destroy current message
			free(currentMsg);
			currentMsg=NULL;
			//go back to normal game state
			HideMessages();
		}
		//blt the snapshot surface onto back surface
		dest_rect.left=0;
		dest_rect.top=0;
		dest_rect.right=VP_WIDTH;
		dest_rect.bottom=VP_HEIGHT;
		lpddsback->Blt(&dest_rect,lpddsSnapShot,NULL,(DDBLT_WAIT),NULL);// | DDBLT_KEYSRC
		//show msg
		if (currentMsg!=NULL) DrawMessage(currentMsg);
		// flip the surfaces
		DDraw_Flip();
		break;
	case GS_EXIT:
		// this is the exit state, called just once
		PostMessage(main_window_handle, WM_DESTROY,0,0);
		gameState = GS_WAITING_FOR_EXIT;
		// fade to black
		Screen_Transitions(SCREEN_DARKNESS, NULL, 0);
		// clear out buffers
		DDraw_Fill_Surface(lpddsback, 0);
		DDraw_Fill_Surface(lpddsprimary, 0);
		break;
	case GS_WAITING_FOR_EXIT:
		{
		// wait here in safe state
		}
		break;
	}
	return SUCCESS;
}


