
#ifndef GAME_H
#define GAME_H

// Includes ////////////////////////////////////////
#include <windows.h>   // include important windows stuff
#include <windowsx.h>
#include <mmsystem.h>
#include <iostream.h> // include important C/C++ stuff
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

#include <ddraw.h>  // directX includes
#define DSBCAPS_CTRLDEFAULT		DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>

#include "T3DLIB1.h" // game library includes
#include "T3DLIB2.h"
#include "T3DLIB3.h"

#include "LinkedList.h"

#include "GUI.H"

// Externals ///////////////////////////////////////

extern int gameState;

// Defines and Constants ///////////////////////////

//GAME STATES
const int GS_EXIT				=	1;
const int GS_WAITING_FOR_EXIT	=	2;
const int GS_RUNNING			=	3;
const int GS_INVENTSCREEN		=	4;
const int GS_COMMSCREEN			=	5;
const int GS_TRANS_MSGPOPUP		=	6;
const int GS_MSGPOPUP			=	7;
const int GS_MENU				=	8;
const int GS_PLAYERDEAD			=	9;
//player stuff
// direction
const int PC_FACE_N				=	0;
const int PC_FACE_S				=	1;
const int PC_FACE_W				=	2;
const int PC_FACE_E				=	3;
// movement/animation
const int PC_ANIMSPEED			=	4;		//anim ticks per frame of animation
const int PC_ANIM_MOVE_DX		=	6;		//pixels per frame of movement in x dir
const int PC_ANIM_MOVE_DY		=	6;		//pixels per frame of movement in y dir
const int NPC_ANIM_MOVE_DX		=	3;		//pixels per frame of movement in x dir
const int NPC_ANIM_MOVE_DY		=	3;		//pixels per frame of movement in y dir
// sprite stuff
const int PC_SPRITEW			=	64;
const int PC_SPRITEH			=	64;
// states
const int PC_STATE_NOISY	=	0x00000001;
//defines for generic function ret vals
const int SUCCESS		=	1;
const int FAILURE		=	0;
//area grafx defs
const int AREA_WIDTH		= 24;	//width,height in tile
const int AREA_HEIGHT		= 18;
const int AREA_WIDTH_L1		= AREA_WIDTH/2;	//width,height in tile
const int AREA_HEIGHT_L1	= AREA_HEIGHT/2;
const int TILEW				= 32;	//width,height of a tile in pixels
const int TILEH				= 32;
const int TILEW_L1			= 64;	//width,height of a 2nd layer (L1) tile in pixels
const int TILEH_L1			= 64;
const int ITEMW				= 32;
const int ITEMH				= 32;
const int VP_WIDTH			= AREA_WIDTH*TILEW;		//width,height of viewport (game area) in pixels (the corner of vp is assumed to be at 0,0)
const int VP_HEIGHT			= AREA_HEIGHT*TILEH;
//item "class" ids
const int ID_DAMAGE			=	0x10000000;
const int ID_TARGET			=	0x20000000;
const int ID_WEP			=	0x01000000;
const int ID_EQUIP			=	0x02000000;
const int ID_AMMO			=	0x04000000;
const int ID_TRANS			=	0x08000000;
//item "subclass" ids
const int ID_TRANS_DOOR		=	0x00010000 | ID_TRANS;	//door type, used in actor that is a door
//item ids
const int ID_WEP_GUN			=	1 | ID_WEP;
const int ID_ITEM_AID			=	2 | ID_EQUIP;
const int ID_TRANS_DOORN		=	3 | ID_TRANS_DOOR;
const int ID_TRANS_DOORS		=	4 | ID_TRANS_DOOR;
const int ID_TRANS_DOORE		=	5 | ID_TRANS_DOOR;
const int ID_TRANS_DOORW		=	6 | ID_TRANS_DOOR;
const int ID_ITEM_KEYCARD		=	7 | ID_EQUIP;
const int ID_AMMO_BULLETS		=	8 | ID_AMMO;
const int ID_WEP_SHOTGUN		=	9 | ID_WEP;
const int ID_WEP_GRENADELAUNCH	=	10 | ID_WEP;
const int ID_WEP_MACHINEGUN		=	11 | ID_WEP;
const int ID_AMMO_GRENADES		=	12 | ID_AMMO;
const int ID_DAMAGE_EXPL		=	13 | ID_DAMAGE;
const int ID_ITEM_SILENCER		=	14 | ID_EQUIP;
const int ID_WEP_EXPLOSIVES		=	15 | ID_WEP;
const int ID_WEP_EXPLOSIVE_ARMED=	16 | ID_WEP;
const int ID_ITEM_TARGETAREA	=	17 | ID_TARGET;
const int ID_HOSTAGE			=	18;
// attr flags for ID_TARGET objects
const int ID_TARGET_TAKE_PIERCE_DAMAGE	=	0x00000001;
const int ID_TARGET_TAKE_SHOCK_DAMAGE	=	0x00000002;
const int ID_TARGET_TAKE_FIRE_DAMAGE	=	0x00000004;
//object defs
const int AMMO_GRENADE_WIDTH	=	64;
const int AMMO_GRENADE_HEIGHT	=	64;
const int AMMO_GRENADE_NF		=	9;
const int AMMO_GRENADE_SF		=	0;
const int DAMAGE_EXPL_WIDTH		=	64;
const int DAMAGE_EXPL_HEIGHT	=	64;
const int DAMAGE_EXPL_NF		=	19;
const int DAMAGE_EXPL_SF		=	0;
const int TRANS_DOOR_WIDTH		=	64;
const int TRANS_DOOR_HEIGHT		=	128;
const int TRANS_DOOR_NF			=	9;
const int TRANS_DOORW_SF		=	0;
const int TRANS_DOORE_SF		=	9;
//AI defs
// NPC AI state
const int AI_STATE_NORM					=	0;
const int AI_STATE_ALERTED				=	1;
const int AI_STATE_ALERTED_FOLLOW		=	2;
const int AI_STATE_ALERTED_ATTACK		=	3;
const int AI_STATE_ALERTED_EVADE		=	4;
// script defs
const int AI_SCRIPT_MOVELEFT64	=	1;
const int AI_SCRIPT_MOVERIGHT64	=	2;
const int AI_SCRIPT_MOVEUP64	=	3;
const int AI_SCRIPT_MOVEDOWN64	=	4;
const int AI_SCRIPT_LOOKLEFT	=	5;
const int AI_SCRIPT_LOOKRIGHT	=	6;
const int AI_SCRIPT_LOOKUP		=	7;
const int AI_SCRIPT_LOOKDOWN	=	8;
const int AI_SCRIPT_WAIT30		=	9;
// other AI stuff
const int AI_ATTACKRAD_DEF	=	(VP_WIDTH+VP_HEIGHT)/8;
//various state defs
const int ST_DOOROPEN	=	1;
const int ST_DOORCLOSED	=	2;
//proj attribs
const int PROJ_ATTR_PLAYER		=	0x00000001;
const int PROJECTILE_HEIGHT_OFFSET = -PC_SPRITEH/2;		//offset (in y dir) that adjusts an objects position that coincides w/ projectile
//area effects (mostly terrain cost from now on...)
const int AE_NOTPASSABLE		=	0x00000001;		// cannot occupy any portion of this tile
// some global area effects defs (can mean that area is inside,outdoors,forest,underground cave,etc....)
const int AE_OUTSIDE_DEF		=   0;
const int AE_INSIDE_DEF			=   1;
//
const int LIST_INIT_SIZE		=	10;
const int LIST_INC_SIZE			=	10;
//projectile defs
const int PT_BULLET				=	1;		//proj type=bullet
	const int PT_BULLET_VEL		=	8;
	const int PT_BULLET_RNG		=	(int)(VP_WIDTH*0.75);
const int PT_GRENADE			=	2;
	const int PT_GRENADE_VEL	=	3;
	const int PT_GRENADE_RNG	=	(int)(PT_BULLET_RNG*0.35);
const int PT_					=	3;
//misc defines
const int AMMO_DEFAULTAMNT	=	50;
const int KEY_DELAY			=	10;
const int SCORE_KILL_NPC	=	10;
// Structs /////////////////////////////////////////

typedef struct VECTOR2DI_TYPE {
	int x,y;
} VECTOR2DI;
typedef struct VECTOR3DI_TYPE {
	int x,y,z;
} VECTOR3DI;

typedef struct PATHNODE_TYPE {
	int x,y;
	PATHNODE_TYPE* next;
} PATHNODE, *LPPATHNODE;

typedef struct OBJECT_TYPE {	//objects can be weapons, equipment, doors, etc.
	char* name;				//readable decriptive name
	int type;				//type of item: wep, item, actor, etc
	int uid;				// -- NOT USED -- //
	int *attr;				//info here depends on type
	int state;				//state of the object depends on type
	int animate;			//is this item being animated?
	int canTake;			//duh
	int quantity;			//quantity of item (if 0 then can only have one instance of it)
	int x,y;				//position on screen
	int width,height;		//effective width,height
	int imgWidth,imgHeight;	//width,height of internal bitmap
	int visible,enabled;	//visible/enabled
	BOB bob;
} OBJECT, * LPOBJECT;
typedef struct OBJECTCOL_TYPE {
	int size,sizeAllocd; //sizeAllocd is privately used
	OBJECT** list;
} OBJECTCOL, * LPOBJECTCOL;

typedef struct AREA_TYPE {
	DWORD areaMap[AREA_HEIGHT][AREA_WIDTH];			//encoded graphical area descrip floor layer 0
	DWORD areaMapL1[AREA_HEIGHT_L1][AREA_WIDTH_L1];		//encoded graphical area descrip layer 1
	DWORD areaEffects[AREA_HEIGHT][AREA_WIDTH];		//attr that affect a block of the area
	DWORD areaEffectsGlobal;						//attr that affect entire area
	OBJECTCOL* invent;
} AREA, * LPAREA;
typedef struct WORLD_TYPE {
	char* name;
	int id,attr;
	int sizeX,sizeY,sizeZ;
	AREA** worldMap;		//ptr to a bunch of ptrs to AREAs [level][area_row][area_col]
} WORLD, * LPWORLD;

typedef struct PROJ_TYPE {
	int x,y;
	int dx,dy;
	int attr;	//atrribs include PROJ_ATTR_PLAYER
	int width,height;
	int imgWidth,imgHeight;
	int type;
	int damage;
	int range,rangeDecr;
	int hasBob;
	LPAREA area;
	BOB bob;
} PROJ;

typedef struct NPC_TYPE {	//non player char
	int aiState;			//state of NPC behavior
	int aiAttackRadSqrd;
	unsigned char script[64];
	int scriptTmp;
	int numScriptSteps;
	int currScriptStep;
	VECTOR3DI mappos;		//area player is in
	LPAREA mapposArea;		//ptr to area
	LPWORLD world;
	int x,y;				//in screen coords
	int vx,vy;				//in screen coords
	int width,height;		//overhead width and height of enemy
	int facing;
	int energy;
	int level;
	int invicibleTicks;
	int moving;				//flag whether NPC is moving (NOT USED YET)
	LPPATHNODE path;
	LPOBJECT currWep,currEquip;
	int fireDelay;
	OBJECTCOL* invent;
	BOB bob;
} NPC, * LPNPC;

typedef struct PLAYER_TYPE {
	int state;
	VECTOR3DI mappos;	//area player is in
	LPAREA mapposArea;
	int x,y;			//in screen coords
	int vx,vy;			//in screen coords
	int width,height;	//overhead width and height of player
	int facing;
	int energy;
	int keyDelay;
	LPOBJECT currWep,currEquip;
	int lives;
	int score;
	int invicibleTicks;
	int fireDelay;
	OBJECTCOL* invent;
	BOB bob;
} PLAYER, * LPPLAYER;

//condition types
const int CONTYPE_TOUCH						=	1;
const int CONTYPE_TIMER						=	2;
const int CONTYPE_LIST_CONTAINS_ITEM		=	3;
const int CONTYPE_TARGET_DESTROYED			=	4;
const int CONTYPE_CONDITIONS_SATISFIED		=	5;	//checks that other conditions are satisfied
const int CONTYPE_NPC_KILLED				=	6;
//result types
const int RESTYPE_ENERGY					=	1;
const int RESTYPE_MAKE_ENABLED_VISIBLE		=	2;
const int RESTYPE_MAKE_DISABLED_INVISIBLE	=	3;
const int RESTYPE_DESTROY_OBJECT			=	4;
const int RESTYPE_SHOW_MESSAGE				=	5;
const int RESTYPE_EXPLOSION					=	6;
const int RESTYPE_ENDGAME					=	7;
//misc
const int MAX_CONDITION_SUCCESSORS			=	4;
typedef struct RESULT_TYPE {
	int type;
	int* data;
	RESULT_TYPE* next;
	LPAREA area;
	LPOBJECTCOL itemcol;
	LPOBJECT item;
	char** messages;
} RESULT, *LPRESULT;
typedef struct CONDITION_TYPE {
	int satisfied;
	LPRESULT result;
	int type;
	int numNextConditions;
	CONDITION_TYPE* nextConditions[MAX_CONDITION_SUCCESSORS];	//arr of ptrs to next sequence of conditions to be met
	int* data;
	LPAREA area;
	LPNPC npc;
	LPOBJECTCOL itemcol;
	LPOBJECT item;
	LinkedList<CONDITION_TYPE*>* conditionList;
} CONDITION, *LPCONDITION;
typedef struct GOAL_TYPE {
	int uid;
	LPCONDITION conditionTree;
	LinkedList<LPCONDITION>* internConditions;
} GOAL, *LPGOAL;

//pop up message defs
//attr
const int POPMSG_STYLE_WINDOWSHADEOPEN		=	0x00000001;
const int POPMSG_STYLE_WINDOWSHADECLOSE		=	0x00000002;
//state
const int POPMSG_STATE_OPEN					=	0x00000001;
const int POPMSG_STATE_CLOSED				=	0x00000002;
const int POPMSG_STATE_OPENING				=	0x00000003;
const int POPMSG_STATE_CLOSING				=	0x00000004;
typedef struct POPMSG_TYPE {
	char** msg;
	int x,y,width,height;
	int numMsg;
	int currMsg;
	int currX,currY,currWidth,currHeight,fade;	//used internally
	BITMAP_IMAGE* bmpImage;	//used internally
	int visible;
	DWORD state;
	DWORD attr;
} POPMSG, *LPPOPMSG;

// Function Prototypes /////////////////////////////

// game console
int Game_Reset(void *params=NULL);
int Game_Init(void *params=NULL);
int Game_Shutdown(void *params=NULL);
int Game_Main(void *params=NULL);

#endif	//GAME_H

