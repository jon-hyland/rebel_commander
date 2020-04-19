
void ProcessInputKeyboard_Menu(LPPLAYER player, unsigned char* keystate)
{
	//if (keystate[DIK_ESCAPE]) {
	//	gameState=GS_EXIT;
	//}
}

void ProcessInputKeyboard_Message(LPPLAYER player, unsigned char* keystate)
{
	if (player->keyDelay==0) {
		if (keystate[DIK_SPACE]) {
			if (currentMsg->bmpImage!=NULL) {	//release if last msg was an image
				Destroy_Bitmap(currentMsg->bmpImage);
				free(currentMsg->bmpImage);
				currentMsg->bmpImage=NULL;
			}
			currentMsg->currMsg++;	//incr msg count until last msg has been displayed for current message
			player->keyDelay=KEY_DELAY;
		}
	} else if(player->keyDelay>0) player->keyDelay--;
}

void ProcessInputKeyboard_Invent(LPPLAYER player, unsigned char* keystate)
{
	if (player->keyDelay==0) {
		if (keystate[DIK_I] || keystate[DIK_ESCAPE]) {
			gameState=GS_RUNNING;
			ProcessInputKeyboard=ProcessInputKeyboard_Normal;
			player->keyDelay=KEY_DELAY;
		}
	} else if(player->keyDelay>0) player->keyDelay--;
}

void ProcessInputKeyboard_Normal(LPPLAYER player, unsigned char* keystate)
{
	int moved=0;	//player movement tracking var
	int i,j,x,y,dx,dy,cx,cy;	//hold area cell position
	int projType;
	int ax=player->mappos.x, ay=player->mappos.y, az=player->mappos.z;	//hold area position in world
	int randShift;
	LPAREA areaptr,oldareaptr;
	LPOBJECTCOL areaItemcol=player->mapposArea->invent, playerItemcol=player->invent;
	LPOBJECT item,tmpItem;
	LPCONDITION cond;

	if (keystate[DIK_UP]) {
		player->y-=PC_ANIM_MOVE_DY;moved=1;
		if (player->facing!=PC_FACE_N) Set_Animation_BOB(&player->bob,0);
		player->facing=PC_FACE_N;
	}
	else if (keystate[DIK_DOWN]) {
		player->y+=PC_ANIM_MOVE_DY;moved=1;
		if (player->facing!=PC_FACE_S) Set_Animation_BOB(&player->bob,1);
		player->facing=PC_FACE_S;
	}
	else if (keystate[DIK_RIGHT]) {
		player->x+=PC_ANIM_MOVE_DX;moved=1;
		if (player->facing!=PC_FACE_E) Set_Animation_BOB(&player->bob,2);
		player->facing=PC_FACE_E;
	}
	else if (keystate[DIK_LEFT]) {
		player->x-=PC_ANIM_MOVE_DX;moved=1;
		if (player->facing!=PC_FACE_W) Set_Animation_BOB(&player->bob,3);
		player->facing=PC_FACE_W;
	}
	if (keystate[DIK_LCONTROL] && player->fireDelay==0) {
		if (player->currWep!=NULL) {
			switch (player->currWep->type) {
			case ID_WEP_SHOTGUN:
			case ID_WEP_MACHINEGUN:
			case ID_WEP_GUN:
				tmpItem=ListEnumItemType(playerItemcol,ID_AMMO_BULLETS,NULL);
				projType = PT_BULLET;
				break;
			case ID_WEP_GRENADELAUNCH:
				tmpItem=ListEnumItemType(playerItemcol,ID_AMMO_GRENADES,NULL);
				projType = PT_GRENADE;
				break;
			case ID_WEP_EXPLOSIVES:
				tmpItem=player->currWep;
				break;
			default: tmpItem=NULL; break;
			}
			//
			if (tmpItem!=NULL) {
				switch (player->currWep->type) {
				case ID_WEP_GRENADELAUNCH:
				case ID_WEP_GUN:
					if (player->facing==PC_FACE_N)		{AddProjectile(projType,PROJ_ATTR_PLAYER,player->x,player->y-48,0,-tmpItem->attr[0],tmpItem->attr[1],player->mapposArea);}
					else if (player->facing==PC_FACE_S) {AddProjectile(projType,PROJ_ATTR_PLAYER,player->x,player->y-20,0,tmpItem->attr[0],tmpItem->attr[1],player->mapposArea);}
					else if (player->facing==PC_FACE_W) {AddProjectile(projType,PROJ_ATTR_PLAYER,player->x-20,player->y-25,-tmpItem->attr[0],0,tmpItem->attr[1],player->mapposArea);}
					else if (player->facing==PC_FACE_E) {AddProjectile(projType,PROJ_ATTR_PLAYER,player->x+20,player->y-25,tmpItem->attr[0],0,tmpItem->attr[1],player->mapposArea);}
					//decrement amt
					tmpItem->quantity--;
					//check if has silencer (otherwise, sets the bit in player state to indicate they made a noise)
					if (player->currWep->type==ID_WEP_GRENADELAUNCH ||
						!ListItemTypeExists(player->invent,ID_ITEM_SILENCER)) {
						player->state = player->state | PC_STATE_NOISY;
						//sound
						DSound_Play(snd_handgun);
					}
					else {
						//sound
						DSound_Play(snd_silence_handgun);
					}
					break;
				case ID_WEP_SHOTGUN:
					if (player->facing==PC_FACE_N)		{ x=player->x; y=player->y-48; dx=0; dy=-1; }
					else if (player->facing==PC_FACE_S) { x=player->x; y=player->y-20; dx=0; dy=1; }
					else if (player->facing==PC_FACE_W) { x=player->x-20; y=player->y-25; dx=-1; dy=0; }
					else if (player->facing==PC_FACE_E) { x=player->x+20; y=player->y-25; dx=1; dy=0; }
					if (dx==0) {
						for (i=6; i<TRIG_NUM_EVAL-6; i++) {
							if (tmpItem->quantity>0) {
								AddProjectile(projType,PROJ_ATTR_PLAYER,x,y,round(tmpItem->attr[0]*tblCOS[i]),round(dy*tmpItem->attr[0]*tblSIN[i]),
											  tmpItem->attr[1],player->mapposArea);
								//decrement amt
								tmpItem->quantity--;
							}
						}
					}
					else {
						for (i=6; i<TRIG_NUM_EVAL-6; i++) {
							if (tmpItem->quantity>0) {
								AddProjectile(projType,PROJ_ATTR_PLAYER,x,y,round(dx*tmpItem->attr[0]*tblSIN[i]),round(tmpItem->attr[0]*tblCOS[i]),
											  tmpItem->attr[1],player->mapposArea);
								//decrement amt
								tmpItem->quantity--;
							}
						}
					}
					//set the bit in player state to indicate they made a noise
					player->state = player->state | PC_STATE_NOISY;
					//sound
					DSound_Play(snd_shotgun);
					break;
				case ID_WEP_MACHINEGUN:
					randShift=rand()%21-10;
					if (player->facing==PC_FACE_N)		{AddProjectile(projType,PROJ_ATTR_PLAYER,player->x+randShift,player->y-48,0,-tmpItem->attr[0],tmpItem->attr[1],player->mapposArea);}
					else if (player->facing==PC_FACE_S) {AddProjectile(projType,PROJ_ATTR_PLAYER,player->x+randShift,player->y-20,0,tmpItem->attr[0],tmpItem->attr[1],player->mapposArea);}
					else if (player->facing==PC_FACE_W) {AddProjectile(projType,PROJ_ATTR_PLAYER,player->x-20,player->y-25+randShift,-tmpItem->attr[0],0,tmpItem->attr[1],player->mapposArea);}
					else if (player->facing==PC_FACE_E) {AddProjectile(projType,PROJ_ATTR_PLAYER,player->x+20,player->y-25+randShift,tmpItem->attr[0],0,tmpItem->attr[1],player->mapposArea);}
					//decrement amt
					tmpItem->quantity--;
					//set the bit in player state to indicate they made a noise
					player->state = player->state | PC_STATE_NOISY;
					//sound
					if (DSound_Status_Sound(snd_machinegun)==0) {
						DSound_Play(snd_machinegun);
					}
					break;
				case ID_WEP_EXPLOSIVES:
					LPRESULT resRemove,resExplosion;
					item = AddItem(player->mapposArea,ID_WEP_EXPLOSIVE_ARMED,0,player->x,player->y,NULL,0);
					//create an internal condition to current goal
					resRemove=(LPRESULT)malloc(sizeof(RESULT));
					resExplosion=(LPRESULT)malloc(sizeof(RESULT));
					resRemove->type=RESTYPE_DESTROY_OBJECT;
					resRemove->item=item;
					resRemove->itemcol=player->mapposArea->invent;
					resRemove->messages=NULL;
					resRemove->data=NULL;
					resRemove->next=resExplosion;
					resExplosion->type=RESTYPE_EXPLOSION;
					resExplosion->area=player->mapposArea;
					resExplosion->messages=NULL;
					resExplosion->data=(int*)malloc(sizeof(int)*2);
					resExplosion->data[0]=player->x;
					resExplosion->data[1]=player->y-16;
					resExplosion->next=NULL;
					cond=(LPCONDITION)malloc(sizeof(CONDITION));
					cond->type=CONTYPE_TIMER;
					cond->satisfied=0;
					cond->data=(int*)malloc(sizeof(int));
					cond->data[0]=60;
					cond->conditionList=NULL;
					cond->numNextConditions=0;	//no conditions can be linked since this will be destroyed once satisfied
					cond->result=resRemove;
					AddCondition(&goal,cond);
					//decrement amt
					tmpItem->quantity--;
					//player->state = player->state | PC_STATE_NOISY;
					break;
				}
				//check if need to remove if 0 left
				if (tmpItem->quantity==0) {
					if (ListContainsItem(playerItemcol,tmpItem)) {
						ListDelete(playerItemcol,tmpItem);
						if (player->currWep==tmpItem) {	//check if ammo ptr was also the current weapon (eg hand-thrown grenade for ex)
							player->currWep=NULL;
						}
					}
				}
				else {  //reset fire delay
					player->fireDelay=player->currWep->attr[0];
				}
			}
			else { //empty weapon
				DSound_Play(snd_empty);
			}
		}
	}
	else {
		if (player->fireDelay>0) player->fireDelay--;	//decr fire delay for player
		//kill any weapon sounds
		//if (DSound_Status_Sound(snd_handgun)!=0) DSound_Stop_Sound(snd_handgun);
		//if (DSound_Status_Sound(snd_silence_handgun)!=0) DSound_Stop_Sound(snd_silence_handgun);
		if (DSound_Status_Sound(snd_machinegun)!=0) DSound_Stop_Sound(snd_machinegun);
		//if (DSound_Status_Sound(snd_shotgun)!=0) DSound_Stop_Sound(snd_shotgun);
	}
	//
	if (player->keyDelay==0) {
		if (keystate[DIK_ESCAPE]) {
			gameState=GS_MENU;
			ProcessInputKeyboard=ProcessInputKeyboard_Menu;
		}
		if (keystate[DIK_W]) {	//cycle weapons
			player->currWep=ListEnumItemClass(playerItemcol,ID_WEP,player->currWep);
			if (player->currWep!=NULL) player->fireDelay=0;
			player->keyDelay=KEY_DELAY;
		}
		if (keystate[DIK_E]) {	//cycle equipment
			player->currEquip=ListEnumItemClass(playerItemcol,ID_EQUIP,player->currEquip);
			player->keyDelay=KEY_DELAY;
		}
		if (keystate[DIK_I]) {
			gameState=GS_INVENTSCREEN;
			ProcessInputKeyboard=ProcessInputKeyboard_Invent;
			player->keyDelay=KEY_DELAY;
		}
	}
	else if (player->keyDelay>0) player->keyDelay--;	//decr inventory delay
	//
	if (moved==1) {
		//check if player pos is "out of area" (only in x and y dir of course!)
		if (player->x<0) {ax--;moved=2;}
		else if (player->x>=VP_WIDTH) {ax++;moved=2;}
		else if (player->y<0) {ay--;moved=2;}
		else if (player->y>=VP_HEIGHT) {ay++;moved=2;}
		if ( moved==2 && (areaptr=GetWorldArea(&world, ax, ay, az))!=NULL ) {
			oldareaptr=player->mapposArea;
			player->mapposArea=areaptr;
			player->mappos.x=ax;
			player->mappos.y=ay;
			player->mappos.z=az;
			//snap player to opposite edge of screen
			if (player->x<0) player->x=VP_WIDTH-1;
			else if (player->x>=VP_WIDTH) player->x=0;
			else if (player->y<0) player->y=VP_HEIGHT-1;
			else if (player->y>=VP_HEIGHT) player->y=0;
			//create the new area map
			CreateAreaMap(player->mapposArea);
			//play some music depending on area type (if different from last type)
			if (oldareaptr->areaEffectsGlobal!=areaptr->areaEffectsGlobal) {
				switch (areaptr->areaEffectsGlobal) {
				case AE_INSIDE_DEF:
					DMusic_Play( (rand()%2)?snd_music[4]:snd_music[5] );
					break;
				default:
				case AE_OUTSIDE_DEF:
					DMusic_Play( snd_music[rand()%3+1] );
					break;
				}
			}
		}
		else if (moved==2) {		// ...hmm, world ends here, just snap player to edge
			if (player->x<0) player->x=0;
			else if (player->x>=VP_WIDTH) player->x=VP_WIDTH-1;
			else if (player->y<0) player->y=0;
			else if (player->y>=VP_HEIGHT) player->y=VP_HEIGHT-1;
		}
		else {		//(moved==1)
			x=player->x;
			y=player->y;
			// the following adds a "buffer" from player (x,y) to nearby boundary squares, so player cannot overlap too much of object in boundary
			switch (player->facing) {
			case PC_FACE_N: {y-=0; break;}
			case PC_FACE_S: {y+=12; break;}
			case PC_FACE_W: {x-=12; break;}
			case PC_FACE_E: {x+=12; break;}
			}
			// get cell position and act appropriately depending on cell attribs
			cx=x/TILEW;
			cy=y/TILEH;
			if (cx>=0 && cx<AREA_WIDTH &&
				cy>=0 && cy<AREA_HEIGHT &&
				player->mapposArea->areaEffects[cy][cx] & AE_NOTPASSABLE) { //check if we need to "undo" last move
				switch (player->facing) {
				case PC_FACE_N: player->y+=PC_ANIM_MOVE_DY; break;
				case PC_FACE_S: player->y-=PC_ANIM_MOVE_DY; break;
				case PC_FACE_W: player->x+=PC_ANIM_MOVE_DX; break;
				case PC_FACE_E: player->x-=PC_ANIM_MOVE_DX; break;
				}
			}
		}
		//animate player sprite
		Animate_BOB(&player->bob);
	}
	//set new (?) position of player sprite
	Set_Pos_BOB(&player->bob, player->x-PC_SPRITEW/2, player->y-PC_SPRITEH+16);
	//decrement invicibility
	if (player->invicibleTicks>0) player->invicibleTicks--;
}


