#include "AStar.h"

void AddNPC(LPNPC npc) {
	if (NUM_NPC>=MAX_NUM_NPC)
		return;
	int i,ind=-1;
	for (i=0; i<MAX_NUM_NPC; i++) {
		if (npcs[i]==NULL) {ind=i;break;}
	}
	if (ind==-1)
		return;
	// ind pts to a free slot in npcs arr
	npcs[ind]=npc;
	NUM_NPC++;
}

LPNPC CreateNPC(int type, int x, int y, LPWORLD world, int worldX, int worldY, int worldZ,
				int level, int energy) {
	const int numframe=8;
	int i;
	LPNPC npc=(LPNPC)malloc(sizeof(NPC));

	Load_Bitmap_File(&bitmap8bit, "ART/TSCHAR01_8B.BMP");
	/////////////////////////////////////////////////////
	//create the bob
	Create_BOB(&npc->bob,0,0,PC_SPRITEW,PC_SPRITEH,numframe,
			   BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM,
			   DDSCAPS_SYSTEMMEMORY);
	//load frames
	for (i=0; i<numframe; i++)
		Load_Frame_BOB(&npc->bob,&bitmap8bit,i,i%8,i/8,BITMAP_EXTRACT_MODE_CELL);
	//set animations sequences up
	int walkup[2]={0,1};
	int walkdn[2]={2,3};
	int walkrt[2]={4,5};
	int walklt[2]={6,7};
	Load_Animation_BOB(&npc->bob,0,2,walkup);
	Load_Animation_BOB(&npc->bob,1,2,walkdn);
	Load_Animation_BOB(&npc->bob,2,2,walkrt);
	Load_Animation_BOB(&npc->bob,3,2,walklt);
	ListInit(&npc->invent);
	npc->width=32;
	npc->height=32;
	npc->level=level;
	npc->energy=energy;
	//SetDirection(npc,PC_FACE_S);
	npc->facing=-1;
	npc->mappos.x=worldX;
	npc->mappos.y=worldY;
	npc->mappos.z=worldZ;
	npc->world=world;
	npc->aiAttackRadSqrd=AI_ATTACKRAD_DEF;
	npc->x=x;
	npc->y=y;
	npc->vx=0;
	npc->vy=0;
	npc->path=NULL;
	npc->moving=0;
	npc->scriptTmp=0;
	npc->currScriptStep=0;
	npc->numScriptSteps=0;
	npc->aiState=AI_STATE_NORM;
	npc->mapposArea=GetWorldArea(world,worldX,worldY,worldZ);
	npc->currEquip=NULL;
	npc->currWep=NULL;
	npc->fireDelay=0;
	npc->invicibleTicks=0;
	SetPosition(npc,npc->x,npc->y);
	Set_Anim_Speed_BOB(&npc->bob,PC_ANIMSPEED);
	//////////////////////////////////////////////////////
	//unload file
	Unload_Bitmap_File(&bitmap8bit);
	return npc;
}

void AddScriptInstruct(LPNPC npc, unsigned char instruction) {
	npc->script[npc->numScriptSteps++]=instruction;
}

void SetPosition(LPNPC npc, int newX, int newY) {
	if (newX!=-1) npc->x=newX;
	if (newY!=-1) npc->y=newY;
	Set_Pos_BOB(&npc->bob,npc->x-PC_SPRITEW/2,npc->y-PC_SPRITEH+16);
}

void SetDirection(LPNPC npc, int dir) {
	if (dir==npc->facing) return;	//dont reset animation
	npc->facing=dir;
	switch (dir) {
	case PC_FACE_N:
		Set_Animation_BOB(&npc->bob,0);
		break;
	case PC_FACE_S:
		Set_Animation_BOB(&npc->bob,1);
		break;
	case PC_FACE_W:
		Set_Animation_BOB(&npc->bob,3);
		break;
	case PC_FACE_E:
		Set_Animation_BOB(&npc->bob,2);
		break;
	default: break;
	}
}

//a* shortest path weight funcs
int SuccessorCost(PathNode* start,PathNode* dest)
{
	if (start->x==dest->x || start->y==dest->y)
		return 2;
	else
		return 3;
}
int EstimateCost(PathNode* start,PathNode* dest)
{
	return 2*Fast_Distance_2D(dest->x-start->x,dest->y-start->y);
}

//calculates/contructs shortest path to position in area
int MoveTo(LPNPC npc, int x, int y)
{
	PathNode* node=NULL,*lastNode;
	int i=0, sx=npc->x/TILEW, sy=npc->y/TILEH, dx=x/TILEW, dy=y/TILEH;

	AStarBestPath* pathFinder = new AStarBestPath((int*)npc->mapposArea->areaEffects,AREA_WIDTH,AREA_HEIGHT);
	//set cost funcs
	pathFinder->setCostCalculationFunc(SuccessorCost);
	pathFinder->setCostEstimationFunc(EstimateCost);
	//find best path from current tile npc is in, to tile containing (x,y)
	pathFinder->findBestPath(sx+sy*AREA_WIDTH,dx+dy*AREA_WIDTH);
	//
	if (pathFinder->getBestPath()==NULL) {
		delete pathFinder;
		return 0;	//failed, no path to desired coordinate
	}
	while (pathFinder->getBestPath()->length()>0) {
		i++;
		node=pathFinder->getBestPath()->pop();
		if (pathFinder->getBestPath()->length()==0) {
			//AddPathNode(npc,x,y);
		}
		else if (i>1) {
			/*//cheesy check to remove co-linear area cells
			if (lastNode->x==node->x)
				lastNode->y=node->y;
			else if (lastNode->y==node->y)
				lastNode->x=node->x;
			else*/ {
				AddPathNode(npc,node->x*TILEW+TILEW/2,node->y*TILEH+TILEH/2);
				lastNode=node;
			}
		}
		else { //dont add first node (should be the grid cell npc in started in)
			lastNode=node;
		}
	}
	delete pathFinder;
	return 1;
}
//moves npc straight toward a position in area
int MoveToward(LPNPC npc, int destX, int destY) {
	const int MIN_DIST = 2;
	int dx,dy,x,y,newX=-1,newY=-1;
	float f,len;
	LPAREA area;

	dx=destX-npc->x;
	dy=destY-npc->y;
	len=(float)Fast_Distance_2D(dx,dy);
	//len=sqrt(dx*dx+dy*dy);
	if (fabs(len)<MIN_DIST) return 0;
	f=1.0/len;
	newX=npc->x+NPC_ANIM_MOVE_DX*dx*f;
	newY=npc->y+NPC_ANIM_MOVE_DY*dy*f;
	//check for valid new coords
	int movedPastEdge=0, ax=npc->mappos.x, ay=npc->mappos.y, az=npc->mappos.z;
	if (newX<0) { ax--; movedPastEdge=1; }
	if (newY<0) { ay--; movedPastEdge=1; }
	if (newX>=VP_WIDTH) { ax++; movedPastEdge=1; }
	if (newY>=VP_HEIGHT) { ay++; movedPastEdge=1; }
	if (movedPastEdge && (area=GetWorldArea(npc->world,ax,ay,az))!=NULL) {
		ClearPath(npc);
		npc->mapposArea=area;
		npc->mappos.x=ax;
		npc->mappos.y=ay;
		npc->mappos.z=az;
		if (newX<0) { newX=VP_WIDTH-1; }
		if (newY<0) { newY=VP_HEIGHT-1; }
		if (newX>=VP_WIDTH) { newX=0; }
		if (newY>=VP_HEIGHT) { newY=0; }
	}
	//else if (movedPastEdge) {}  //commented out since NPCs may walk outside of edges (if needed for some reason)
	else {
		//the buffer zone
		x=newX;
		y=newY;
		switch (npc->facing) {
		case PC_FACE_N: {y-=0; break;}
		case PC_FACE_S: {y+=12; break;}
		case PC_FACE_W: {x-=12; break;}
		case PC_FACE_E: {x+=12; break;}
		}
		if (x>=0 && x<VP_WIDTH &&
			y>=0 && y<VP_HEIGHT &&
			(npc->mapposArea->areaEffects[y/TILEH][x/TILEW] & AE_NOTPASSABLE)) {
			return 0;
		}
	}
	//face proper movement direction
	if (abs(dx)>abs(dy)) {
		if (dx<0) SetDirection(npc,PC_FACE_W);
		else SetDirection(npc,PC_FACE_E);
	}
	else {
		if (dy<0) SetDirection(npc,PC_FACE_N);
		else SetDirection(npc,PC_FACE_S);
	}
	//animate & update pos
	Animate_BOB(&npc->bob);
	SetPosition(npc,newX,newY);
	return 1;
}

void AddPathNode(LPNPC npc, int x, int y) {
	//create node
	LPPATHNODE tmpNode=(LPPATHNODE)malloc(sizeof(PATHNODE));
	tmpNode->x=x;
	tmpNode->y=y;
	tmpNode->next=NULL;
	//continue to end
	LPPATHNODE lastNode=npc->path;
	if (lastNode!=NULL) {
		while (lastNode->next!=NULL) {
			lastNode=lastNode->next;
		}
		lastNode->next=tmpNode;
	}
	else {	//no path nodes exist
		npc->path=tmpNode;
	}
}
void FollowPath(LPNPC npc) {
	LPPATHNODE tmpNode;
	if (!MoveToward(npc,npc->path->x,npc->path->y)) {
		tmpNode=npc->path;
		npc->path=npc->path->next;
		free(tmpNode);
	}
}
void ClearPath(LPNPC npc) {
	LPPATHNODE node=npc->path,tmpNode;
	npc->path=NULL;
	while (node!=NULL) {
		tmpNode=node;
		node=node->next;
		free(tmpNode);
	}
}

int InLineOfSight(LPNPC npc, LPPLAYER player) {
	const int DIST_INTERVAL = 8;
	int sx,sy,gx,gy;	//start,goal cell positions
	int dx,dy,numSeg;
	int x,y;
	sx=npc->x;
	sy=npc->y;
	gx=player->x;
	gy=player->y;
	x=sx;
	y=sy;
	numSeg=(int)(((float)Fast_Distance_2D(gx-sx,gy-sy)/DIST_INTERVAL)+0.5);
	if (numSeg==0) return 1;
	dx=(gx-sx)/numSeg;
	dy=(gy-sy)/numSeg;
	for (int i=0; i<numSeg; i++) {
		x+=dx;
		y+=dy;
		if (npc->mapposArea->areaEffects[y/TILEH][x/TILEW] & AE_NOTPASSABLE) {
			return 0;
		}
	}
	return 1;
}
int InLineOfSight_NoPerif(LPNPC npc, LPPLAYER player) {
	int sx,sy,gx,gy;	//start,goal cell positions
	sx=npc->x/TILEW;
	sy=npc->y/TILEH;
	gx=player->x/TILEW;
	gy=player->y/TILEH;
	if (abs(gx-sx)<2 &&
		InLineOfSight(npc,player)) {
		if (player->y-npc->y<0 && npc->facing==PC_FACE_N) return 1;
		else if (player->y-npc->y>=0 && npc->facing==PC_FACE_S) return 1;
	}
	else if (abs(gy-sy)<2 &&
			 InLineOfSight(npc,player)) {
		if (player->x-npc->x<0 && npc->facing==PC_FACE_W) return 1;
		else if (player->x-npc->x>=0 && npc->facing==PC_FACE_E) return 1;
	}
	return 0;
}

//right now only attack is (unlimited) bullets...
void Attack(LPNPC npc, LPPLAYER player)
{
	const float StartRadius = 32;	//radius from npc where projectile starts
	const int FireDelay = 20;
	float f,len;
	int x,y,dx,dy;

	x=npc->x;
	y=npc->y-PC_SPRITEH/2;
	dx=player->x-x;
	dy=player->y+PROJECTILE_HEIGHT_OFFSET-y;
	//face player
	if (abs(dx)>abs(dy)) {
		if (dx<0) SetDirection(npc,PC_FACE_W);
		else SetDirection(npc,PC_FACE_E);
	}
	else {
		if (dy<0) SetDirection(npc,PC_FACE_N);
		else SetDirection(npc,PC_FACE_S);
	}
	//
	len=Fast_Distance_2D(dx,dy);
	f=1.0/len;
	x+=dx*f*StartRadius;
	y+=dy*f*StartRadius;
	if (npc->fireDelay==0) {
		AddProjectile(PT_BULLET,0,x,y,dx*PT_BULLET_VEL*f,dy*PT_BULLET_VEL*f,PT_BULLET_RNG,npc->mapposArea);
		npc->fireDelay=FireDelay;
	}
}

void Evade(LPNPC npc, LPPLAYER player)
{
	//not implemented//
}

//alerts any NPCs on area
void AlertNPCs(LPAREA area)
{
	for (int i=0; i<MAX_NUM_NPC; i++) {
		if (npcs[i]!=NULL && npcs[i]->mapposArea==area && npcs[i]->aiState==AI_STATE_NORM) {
			npcs[i]->aiState=AI_STATE_ALERTED;
		}
	}
}

void DoAITick(LPNPC npc, LPPLAYER player) {
	const int ATTACK_RAD_MODIFIER = 128;
	int i,newX=-1,newY=-1;
	int inside, x, y, dx, dy, width, height;
	LPOBJECTCOL areaItemcol=npc->mapposArea->invent;
	LPOBJECT item;

	// check for death :(
	if (npc->energy<=0) {
		//add score to player (no matter how npc died)
		player->score+=SCORE_KILL_NPC*npc->level;
		DSound_Play(snd_npcdead);
		KillNPC(npc);
		return;
	}
	//check for interactions
	for (i=0; i<areaItemcol->size; i++) {
		item=areaItemcol->list[i];
		if (!item->enabled) continue;
		width=item->width;
		height=item->height;
		dx=npc->x-item->x;
		dy=npc->y-item->y;
		//set flag for if we are within radius of object
		if (dx*dx+dy*dy<width*height/4) inside=1;
		else inside=0;
		//...take damage
		if (inside && npc->invicibleTicks==0 && (item->type & ID_DAMAGE)) {
			npc->energy-=item->attr[0];
			npc->invicibleTicks=10;		//make invincible for a bit
		}
	}
	//do movement,script stuff only if player is here
	if (npc->mapposArea==player->mapposArea) {
		switch (npc->aiState) {
		case AI_STATE_NORM:
			// check for a path that needs to be followed
			if (npc->path!=NULL) {
				FollowPath(npc);
				break;
			}
			// script execution, if any
			if (npc->numScriptSteps>0) {
				switch (npc->script[npc->currScriptStep]) {
				case AI_SCRIPT_MOVEUP64:
					if (npc->scriptTmp==0) {
						SetDirection(npc,PC_FACE_N);
						npc->scriptTmp=64;
					}
					newY=npc->y-NPC_ANIM_MOVE_DY;
					npc->scriptTmp-=NPC_ANIM_MOVE_DY;
					Animate_BOB(&npc->bob);
					break;
				case AI_SCRIPT_MOVEDOWN64:
					if (npc->scriptTmp==0) {
						SetDirection(npc,PC_FACE_S);
						npc->scriptTmp=64;
					}
					newY=npc->y+NPC_ANIM_MOVE_DY;
					npc->scriptTmp-=NPC_ANIM_MOVE_DY;
					Animate_BOB(&npc->bob);
					break;
				case AI_SCRIPT_MOVELEFT64:
					if (npc->scriptTmp==0) {
						SetDirection(npc,PC_FACE_W);
						npc->scriptTmp=64;
					}
					newX=npc->x-NPC_ANIM_MOVE_DY;
					npc->scriptTmp-=NPC_ANIM_MOVE_DY;
					Animate_BOB(&npc->bob);
					break;
				case AI_SCRIPT_MOVERIGHT64:
					if (npc->scriptTmp==0) {
						SetDirection(npc,PC_FACE_E);
						npc->scriptTmp=64;
					}
					newX=npc->x+NPC_ANIM_MOVE_DY;
					npc->scriptTmp-=NPC_ANIM_MOVE_DY;
					Animate_BOB(&npc->bob);
					break;
				case AI_SCRIPT_LOOKUP:
					SetDirection(npc,PC_FACE_N);
					break;
				case AI_SCRIPT_LOOKDOWN:
					SetDirection(npc,PC_FACE_S);
					break;
				case AI_SCRIPT_LOOKLEFT:
					SetDirection(npc,PC_FACE_W);
					break;
				case AI_SCRIPT_LOOKRIGHT:
					SetDirection(npc,PC_FACE_E);
					break;
				case AI_SCRIPT_WAIT30:
					if (npc->scriptTmp==0) {
						npc->scriptTmp=30;
					}
					npc->scriptTmp--;
					break;
				default: break;
				}
				if (npc->scriptTmp<=0) {
					npc->scriptTmp=0;
					npc->currScriptStep=(npc->currScriptStep+1)%npc->numScriptSteps;
				}
			}
			// check if npc sees any enemies
			if (InLineOfSight_NoPerif(npc,player) ||
				(player->state & PC_STATE_NOISY)) {
				npc->aiState=AI_STATE_ALERTED;
			}
			// set position
			SetPosition(npc,newX,newY);
			break;
		case AI_STATE_ALERTED:			//aware of player
			//clear any path being followed and move toward player
			ClearPath(npc);
			MoveTo(npc,player->x,player->y);
			//if action sound not playing, then play it
			if (DMusic_Status_MIDI(snd_ambact1)!=MIDI_PLAYING) {
				DMusic_Play(snd_ambact1);
				DSound_Play(snd_siren,DSBPLAY_LOOPING);
			}
			npc->aiState=AI_STATE_ALERTED_FOLLOW;
			break;
		case AI_STATE_ALERTED_FOLLOW:	//stalker mode
			if (npc->path!=NULL) {
				if (InLineOfSight(npc,player))
					ClearPath(npc);
				else
					FollowPath(npc);
			}
			else if (InLineOfSight(npc,player)) {
				x=npc->x;
				y=npc->y;
				MoveToward(npc,player->x,player->y);
				if (npc->x==x && npc->y==y) {	//if pos hasnt changed, then NPC is stuck, plan a path instead
					MoveTo(npc,player->x,player->y);
				}
			}
			else {
				MoveTo(npc,player->x,player->y);	//plan a path
			}
			if (Fast_Distance_2D(npc->x-player->x,npc->y-player->y)<AI_ATTACKRAD_DEF &&
				InLineOfSight(npc,player)) {
				npc->aiState=AI_STATE_ALERTED_ATTACK;
			}
			break;
		case AI_STATE_ALERTED_ATTACK:	//attack
			Attack(npc,player);
			if (Fast_Distance_2D(npc->x-player->x,npc->y-player->y)>AI_ATTACKRAD_DEF+rand()%ATTACK_RAD_MODIFIER ||
				!InLineOfSight(npc,player)) {
				npc->aiState=AI_STATE_ALERTED_FOLLOW;
			}
			break;
		case AI_STATE_ALERTED_EVADE:	//run!!
			Evade(npc,player);
			break;
		default: break;
		}
	}
	else if (npc->aiState==AI_STATE_ALERTED_FOLLOW) { //chase player if in adjacent room
		int ax=npc->mappos.x, ay=npc->mappos.y, az=player->mappos.z;
		// check for a path that needs to be followed
		if (npc->path!=NULL) {
			FollowPath(npc);
		}
		else {
			if (az==player->mappos.z && ax==player->mappos.x && abs(ay-player->mappos.y)==1) {
				if (ay>player->mappos.y) {
					AddPathNode(npc,npc->x,-10);
				}
				else if (ay<player->mappos.y) {
					AddPathNode(npc,npc->x,VP_HEIGHT+10);
				}
			}
			else if (az==player->mappos.z && ay==player->mappos.y && abs(ax-player->mappos.x)==1) {
				if (ax>player->mappos.x) {
					AddPathNode(npc,-10,npc->y);
				}
				else if (ax<player->mappos.x) {
					AddPathNode(npc,VP_WIDTH+10,npc->y);
				}
			}
			else {	//give up the chase!
				//clear out any scripts and add a generic look around script
				npc->numScriptSteps=0;
				AddScriptInstruct(npc,AI_SCRIPT_LOOKLEFT);
				AddScriptInstruct(npc,AI_SCRIPT_LOOKDOWN);
				AddScriptInstruct(npc,AI_SCRIPT_LOOKRIGHT);
				AddScriptInstruct(npc,AI_SCRIPT_LOOKUP);
			}
		}
	}
	//decr invincibility count if >0
	if (npc->invicibleTicks>0) npc->invicibleTicks--;
	//decr fire delay
	if (npc->fireDelay>0) npc->fireDelay--;
}

int drewPlayer;
void DrawNPCsAndPC(LPPLAYER player)
{
	drewPlayer=0;
	SortNPC();
	for (int i=0; i<MAX_NUM_NPC; i++) {
		if (npcs[i]!=NULL && npcs[i]->mapposArea==player->mapposArea) {
			if (!drewPlayer && npcs[i]->y>player->y) {
				Draw_BOB(&player->bob,lpddsback);
				drewPlayer=1;
			}
			Draw_BOB(&npcs[i]->bob,lpddsback);
		}
	}
	if (!drewPlayer) Draw_BOB(&player->bob,lpddsback);
}

void InitNPCs() {
	for (int i=0; i<MAX_NUM_NPC; i++)
		npcs[i]=NULL;
}
void KillNPC(LPNPC npc) {
	int i,j;
	//should be in NPC list, set to NULL if exists
	for (i=0; i<MAX_NUM_NPC; i++) {
		if (npcs[i]==npc) npcs[i]=NULL;
	}
	//kill npc
	Destroy_BOB(&npc->bob);
	for (j=0; j<npc->invent->size; j++) {
		if (npc->invent->list[j]->attr!=NULL) free(npc->invent->list[j]->attr);
		free(npc->invent->list[j]);
	}
	free(npc->invent);
	free(npc);
	NUM_NPC--;
}
void KillNPCs() {
	int i,j;
	for (i=0; i<MAX_NUM_NPC; i++) {
		if (npcs[i]==NULL) continue;
		Destroy_BOB(&npcs[i]->bob);
		for (j=0; j<npcs[i]->invent->size; j++) {
			if (npcs[i]->invent->list[j]->attr!=NULL) free(npcs[i]->invent->list[j]->attr);
			free(npcs[i]->invent->list[j]);
		}
		free(npcs[i]->invent);
		free(npcs[i]);
		npcs[i]=NULL;
	}
	NUM_NPC=0;
}

