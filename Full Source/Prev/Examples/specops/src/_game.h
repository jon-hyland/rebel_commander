
#ifndef _GAME_H
#define _GAME_H

/* PRIVATE INCLUSION HEADER FILE FOR GAME.CPP */

// Includes

// Defines & Constants ////////////////////////////////////////////////
// Macros /////////////////////////////////////////////////
#define INLINE		__forceinline
#define FREE(x)		if(x!=NULL){free(x);x=NULL;}
#define DEL(x)		if(x!=NULL){delete(x);x=NULL;}
// returns a rand in a range
#define RAND_RANGE(a,b) ((a)+(rand()%((b)-(a)+1)))

// Prototypes /////////////////////////////////////////////
INLINE int round(float x);
INLINE int round(double x);
//sound stuff
void InitGlobalSounds();
//
int  CreateWorld(LPWORLD world);
int  FreeWorld(LPWORLD world);
//
int  SaveWorld(LPWORLD world, LPPLAYER player);
int  LoadWorld(LPWORLD world, LPPLAYER player);
void ProcessInputKeyboard_Normal(LPPLAYER player, unsigned char *keystates);
INLINE LPAREA GetWorldArea(LPWORLD world, int& x,int& y,int& z);
int  CreateArea(LPAREA area,DWORD* srcAreaMap,DWORD* srcAreaMapL1,
			   DWORD* srcAreaMapEffects,DWORD srcAreaMapEffectsGlobal);
void CreateAreaMap(LPAREA area);
int  InitAreaMap();
void DrawAreaMap();
void DrawAreaItems(LPAREA area, LPPLAYER player);
int  CreatePlayer(LPPLAYER player, LPWORLD world);
void DrawPlayerStats(LPPLAYER player);
void AddProjectile(int type, int attr, int posX, int posY, int dx, int dy, int range, LPAREA area);
void AnimateProjectiles();
void DrawProjectiles(LPPLAYER player);
void DrawInvent(LPPLAYER player);
//add items defs
LPOBJECT AddItem(LPAREA area, int type, int uid, int x, int y, int* attr, int quantity, int visible = 1, int enabled = 1);
LPOBJECT AddItem(LPPLAYER player, LPOBJECT item);
//list defs
int ListInit(LPOBJECTCOL* lpItemcol);
int ListAdd(LPOBJECTCOL itemcol, LPOBJECT item);
int ListDelete(LPOBJECTCOL itemcol, LPOBJECT item);
LPOBJECT ListRemove(LPOBJECTCOL itemcol, LPOBJECT item);
INLINE int ListContainsItem(LPOBJECTCOL itemcol, LPOBJECT item);
int ListItemTypeExists(LPOBJECTCOL itemcol, int type);
LPOBJECT ListItemUIDExists(LPOBJECTCOL itemcol, int uid);
LPOBJECT ListEnumItemClass(LPOBJECTCOL itemcol, int typeClass, LPOBJECT startItem);
LPOBJECT ListEnumItemType(LPOBJECTCOL itemcol, int type, LPOBJECT startItem);
//goal/condition defs
// adds an internal condition to a goal
void AddCondition(LPGOAL goal, LPCONDITION condition);
int  CheckCondition(LPCONDITION condition, LPPLAYER player);
void ExecuteResults(LPRESULT result, LPPLAYER player);
void CheckConditions(LPCONDITION start, LPPLAYER player);
void CheckGoalConditions(LPGOAL goal, LPPLAYER player);
void CheckInternalConditions(LPGOAL goal, LPPLAYER player);
//pop-up message  defs
LPPOPMSG CreateMessage(char** msgs, int numMsgs, int x, int y, int width, int height);
void ShowMessages();
void HideMessages();
void DrawMessage(LPPOPMSG msg);
//NPC/AI defs
void AddNPC(LPNPC npc);
LPNPC CreateNPC(int type, int x, int y, LPWORLD world, int worldX, int worldY, int worldZ,
				int level, int energy);
void DoAITick(LPNPC npc, LPPLAYER player);
void DrawNPCsAndPC(LPPLAYER player);
void InitNPCs();
void KillNPC(LPNPC npc);
void KillNPCs();
void AddScriptInstruct(LPNPC npc, unsigned char instruction);
void SetPosition(LPNPC npc, int newX, int newY);
void SetDirection(LPNPC npc, int dir);
int  MoveTo(LPNPC npc, int x, int y);
int  MoveToward(LPNPC npc, int destX, int destY);
void Attack(LPNPC npc, LPPLAYER player);
void Evade(LPNPC npc, LPPLAYER player);
void AlertNPCs(LPAREA area);
void AddPathNode(LPNPC npc, int x, int y);
void ClearPath(LPNPC npc);
int  InLineOfSight_NoPerif(LPNPC npc, LPPLAYER player);
int  InLineOfSight(LPNPC npc, LPPLAYER player);
//
void SortNPC();
int  SortNPCCompare(const void* a,const void* b);
//
void CheckObjectInteractions(LPPLAYER player);

#endif	//_GAME_H


