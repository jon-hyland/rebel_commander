
//given an area location (x,y,z), this routine returns the ptr to the AREA
INLINE LPAREA GetWorldArea(LPWORLD world, int& x,int& y,int& z)
{
	if (x>=0 && x<world->sizeX &&
		y>=0 && y<world->sizeY &&
		z>=0 && z<world->sizeZ ) {
		return ( world->worldMap[ z*(world->sizeX)*(world->sizeY) +
				y*(world->sizeX) + x ] ) ;
	}
	else { return NULL; }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
LPOBJECT AddItem(LPAREA area, int type, int uid, int x, int y, int* attr, int quantity, int visible, int enabled)
{
	int frame;
	int* explode=NULL;
	int dooropen[9]={0,1,2,3,4,5,6,7,8};
	int doorclose[9]={8,7,6,5,4,3,2,1,0};
	if (area->invent==NULL) {
		return NULL;		//oops
	}
	LPOBJECT pItem=(LPOBJECT)malloc(sizeof(OBJECT));
	pItem->attr=NULL;
	//set props
	pItem->x=x;
	pItem->y=y;
	if (quantity>=0) pItem->quantity=quantity;
	else pItem->quantity=0;
	if (uid<=0) uid=0;
	pItem->uid=uid;
	pItem->enabled=enabled;
	pItem->visible=visible;
	//
	switch (type) {
	case ID_ITEM_KEYCARD:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Key card";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int));	//1 for item key id #
		memcpy(pItem->attr,attr,sizeof(int));
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,3,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_AMMO_BULLETS:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Ammo";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int)*2);	//1 for speed in (pix/frame), 1 for range (in pix)
		pItem->attr[0]=PT_BULLET_VEL;
		pItem->attr[1]=PT_BULLET_RNG;
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,4,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_AMMO_GRENADES:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Grenades";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int)*2);	//1 for speed in (pix/frame), 1 for range (in pix)
		pItem->attr[0]=PT_GRENADE_VEL;
		pItem->attr[1]=PT_GRENADE_RNG;
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,6,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_ITEM_AID:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="First-Aid kit";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=NULL;
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,2,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_ITEM_SILENCER:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Silencer";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=NULL;
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,5,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_WEP_GUN:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Hand gun";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int));
		pItem->attr[0]=10;		//fire delay
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,1,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_WEP_GRENADELAUNCH:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Grenade Launcher";
		pItem->width=65;
		pItem->height=24;
		pItem->imgWidth=65;
		pItem->imgHeight=24;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int));
		pItem->attr[0]=40;		//fire delay
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,67,34,BITMAP_EXTRACT_MODE_ABS);
		break;
	case ID_WEP_SHOTGUN:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Shot Gun";
		pItem->width=65;
		pItem->height=24;
		pItem->imgWidth=65;
		pItem->imgHeight=24;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int));
		pItem->attr[0]=30;		//fire delay
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,1,34,BITMAP_EXTRACT_MODE_ABS);
		break;
	case ID_WEP_MACHINEGUN:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Machine Gun";
		pItem->width=65;
		pItem->height=24;
		pItem->imgWidth=65;
		pItem->imgHeight=24;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int));
		pItem->attr[0]=3;		//fire delay
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,133,34,BITMAP_EXTRACT_MODE_ABS);
		break;
	case ID_WEP_EXPLOSIVES:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Plastic Explosives";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=1;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int));
		pItem->attr[0]=10;
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,10,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_WEP_EXPLOSIVE_ARMED:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");
		pItem->name="Plastic Explosives";
		pItem->imgWidth=pItem->width=ITEMW;
		pItem->imgHeight=pItem->height=ITEMH;
		pItem->type=type;
		pItem->canTake=0;
		pItem->animate=0;
		pItem->attr=NULL;
		pItem->state=0;
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,10,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	case ID_TRANS_DOORE:
		Load_Bitmap_File(&bitmap8bit, "ART/DOORS_8B.BMP");
		pItem->name="!";
		pItem->width=TRANS_DOOR_WIDTH+12;
		pItem->height=TRANS_DOOR_HEIGHT+12;
		pItem->imgWidth=TRANS_DOOR_WIDTH;
		pItem->imgHeight=TRANS_DOOR_HEIGHT;
		pItem->type=type;
		pItem->canTake=0;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int)*6);	//3 for world position, 2 for screen position, 1 for item key to open it (0==none)
		memcpy(pItem->attr,attr,sizeof(int)*6);
		pItem->state=ST_DOORCLOSED;
		//load door animation
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					TRANS_DOOR_NF,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM | BOB_ATTR_ANIM_ONE_SHOT,DDSCAPS_SYSTEMMEMORY);
		for (frame=TRANS_DOORE_SF; frame<TRANS_DOORE_SF+TRANS_DOOR_NF; frame++)
			Load_Frame_BOB(&pItem->bob,&bitmap8bit,frame-TRANS_DOORE_SF,frame%9,frame/9,BITMAP_EXTRACT_MODE_CELL);
		Load_Animation_BOB(&pItem->bob,0,TRANS_DOOR_NF,dooropen);
		Load_Animation_BOB(&pItem->bob,1,TRANS_DOOR_NF,doorclose);
		Set_Animation_BOB(&pItem->bob,0);
		break;
	case ID_TRANS_DOORW:
		Load_Bitmap_File(&bitmap8bit, "ART/DOORS_8B.BMP");
		pItem->name="!";
		pItem->width=TRANS_DOOR_WIDTH+12;
		pItem->height=TRANS_DOOR_HEIGHT+12;
		pItem->imgWidth=TRANS_DOOR_WIDTH;
		pItem->imgHeight=TRANS_DOOR_HEIGHT;
		pItem->type=type;
		pItem->canTake=0;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int)*6);	//3 for world position, 2 for screen position, 1 for item key to open it (0==none)
		memcpy(pItem->attr,attr,sizeof(int)*6);
		pItem->state=ST_DOORCLOSED;
		//load door animation
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
					TRANS_DOOR_NF,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM | BOB_ATTR_ANIM_ONE_SHOT,DDSCAPS_SYSTEMMEMORY);
		for (frame=TRANS_DOORW_SF; frame<TRANS_DOORW_SF+TRANS_DOOR_NF; frame++)
			Load_Frame_BOB(&pItem->bob,&bitmap8bit,frame-TRANS_DOORW_SF,frame%9,frame/9,BITMAP_EXTRACT_MODE_CELL);
		Load_Animation_BOB(&pItem->bob,0,TRANS_DOOR_NF,dooropen);
		Load_Animation_BOB(&pItem->bob,1,TRANS_DOOR_NF,doorclose);
		Set_Animation_BOB(&pItem->bob,0);
		break;
	case ID_DAMAGE_EXPL:
		Load_Bitmap_File(&bitmap8bit, "ART/DAMAGE_8B.BMP");
		pItem->name="!";
		pItem->width=DAMAGE_EXPL_WIDTH;
		pItem->height=DAMAGE_EXPL_HEIGHT;
		pItem->imgWidth=DAMAGE_EXPL_WIDTH;
		pItem->imgHeight=DAMAGE_EXPL_HEIGHT;
		pItem->type=type;
		pItem->canTake=0;
		pItem->animate=1;
		pItem->attr=(int*)malloc(sizeof(int));
		pItem->attr[0]=30;
		pItem->state=0;
		//load door animation
		explode=(int*)malloc(sizeof(int)*DAMAGE_EXPL_NF);
		Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
				   DAMAGE_EXPL_NF,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM | BOB_ATTR_ANIM_ONE_SHOT,DDSCAPS_SYSTEMMEMORY);
		for (frame=DAMAGE_EXPL_SF; frame<DAMAGE_EXPL_SF+DAMAGE_EXPL_NF; frame++) {
			explode[frame-DAMAGE_EXPL_SF]=frame-DAMAGE_EXPL_SF;
			Load_Frame_BOB(&pItem->bob,&bitmap8bit,frame-DAMAGE_EXPL_SF,frame%9,frame/9,BITMAP_EXTRACT_MODE_CELL);
		}
		Load_Animation_BOB(&pItem->bob,0,DAMAGE_EXPL_NF,explode);
		Set_Animation_BOB(&pItem->bob,0);
		//alert any npcs (since explosions are generally loud)
		AlertNPCs(area);
		//play sound
		DSound_Play(snd_expld[rand()%2]);
		break;
	case ID_ITEM_TARGETAREA:
		Load_Bitmap_File(&bitmap8bit, "ART/TSITEM01_8B.BMP");	//load for nothing
		pItem->name="_target_area";
		pItem->width=attr[2];
		pItem->height=attr[3];
		pItem->imgWidth=32;
		pItem->imgHeight=32;
		pItem->type=type;
		pItem->canTake=0;
		pItem->animate=0;
		pItem->attr=(int*)malloc(sizeof(int)*2);
		pItem->attr[0]=attr[0];
		pItem->attr[1]=attr[1];
		pItem->visible=0;	//invisible (no BOB was loaded)
		pItem->state=0;
		//Create_BOB(&pItem->bob,x-pItem->imgWidth/2,y-pItem->imgHeight/2,pItem->imgWidth,pItem->imgHeight,
		//			1,BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,DDSCAPS_SYSTEMMEMORY);
		//Load_Frame_BOB(&pItem->bob,&bitmap8bit,0,10,0,BITMAP_EXTRACT_MODE_CELL);
		break;
	default: return NULL;
	}
	//unload
	Unload_Bitmap_File(&bitmap8bit);
	//add to area
	ListAdd(area->invent,pItem);
	return pItem;
}
LPOBJECT AddItem(LPPLAYER player, LPOBJECT item)
{
	ListAdd(player->invent,item);
	return item;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int CreateArea(LPAREA area,DWORD* srcAreaMap,DWORD* srcAreaMapL1,
			   DWORD* srcAreaMapEffects,DWORD srcAreaMapEffectsGlobal)
{
	memcpy(area->areaEffects,srcAreaMapEffects,sizeof(DWORD)*AREA_HEIGHT*AREA_WIDTH);
	area->areaEffectsGlobal=srcAreaMapEffectsGlobal;
	//init item list
	ListInit(&area->invent);
	//
	memcpy(area->areaMap,srcAreaMap,sizeof(DWORD)*AREA_HEIGHT*AREA_WIDTH);
	memcpy(area->areaMapL1,srcAreaMapL1,sizeof(DWORD)*AREA_HEIGHT_L1*AREA_WIDTH_L1);

	return SUCCESS;
}

int InitAreaMap()
{
	int i,j;
	DDSURFACEDESC2 ddsd;
	//create background tile surface
    memset(&ddsd,0,sizeof(ddsd));
    ddsd.dwSize  = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.dwWidth  = VP_WIDTH;
    ddsd.dwHeight = VP_HEIGHT;
    // set surface to offscreen plain
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;// | DDSCAPS_SYSTEMMEMORY
    // create the surfaces, return failure if problem
    if (FAILED(lpdd->CreateSurface(&ddsd,&lpddsScreen,NULL)))
        return FAILURE;
    if (FAILED(lpdd->CreateSurface(&ddsd,&lpddsSnapShot,NULL)))
        return FAILURE;
    // set color key to color 0
    DDCOLORKEY color_key; // used to set color key
    color_key.dwColorSpaceLowValue  = 0;
    color_key.dwColorSpaceHighValue = 0;
    // now set the color key for source blitting
    lpddsScreen->SetColorKey(DDCKEY_SRCBLT, &color_key);
	lpddsSnapShot->SetColorKey(DDCKEY_SRCBLT, &color_key);
	//create/init all tile bobs
	for (i=0; i<AREA_HEIGHT; i++) {
		for (j=0; j<AREA_WIDTH; j++) {
			Create_BOB(&areaMapTiles[i][j],TILEW*j,TILEH*i,TILEW,TILEH,1,
						BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
						DDSCAPS_SYSTEMMEMORY);
		}
	}
	for (i=0; i<AREA_HEIGHT_L1; i++) {
		for (j=0; j<AREA_WIDTH_L1; j++) {
			Create_BOB(&areaMapTilesLayer1[i][j],TILEW_L1*j,TILEH_L1*i,TILEW_L1,TILEH_L1,1,
						BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
						DDSCAPS_SYSTEMMEMORY);
		}
	}
	return SUCCESS;
}

/* CreateAreaMap
 *   Loads tiles into the 'areaMapTiles' and 'areaMapTilesLayer1' arrays ans blits them onto
 * the lpddsScreen surface which is used to draw the area there after.
 */
void CreateAreaMap(LPAREA area)
{
	const int TS_NUMTILES_X=24, TS_NUMTILES_Y=1;
	const int TS_NUMTILESL1_X=12, TS_NUMTILESL1_Y=1;
	int i,j,cell;
	BITMAP_FILE layer0,layer1;
	layer0.buffer=NULL;		//!!!!!!!!!!!! Load_Bitmap_File() in t3dlib1 was trying to free nonexistent buffer member <<ARGGGHHH>> !!!!!!!!!!!!!!!!!
	layer1.buffer=NULL;

	Load_Bitmap_File(&layer0, "ART/TILESET01_8B.BMP");
	for (i=0; i<AREA_HEIGHT; i++) {
		for (j=0; j<AREA_WIDTH; j++) {
			cell=area->areaMap[i][j]-1;
			Load_Frame_BOB(&areaMapTiles[i][j],&layer0,0,cell%TS_NUMTILES_X,cell/TS_NUMTILES_X,BITMAP_EXTRACT_MODE_CELL);
		}
	}
	Unload_Bitmap_File(&layer0);
	Load_Bitmap_File(&layer1, "ART/TILESET02_8B.BMP");
	for (i=0; i<AREA_HEIGHT_L1; i++) {
		for (j=0; j<AREA_WIDTH_L1; j++) {
			cell=area->areaMapL1[i][j]-1;
			Load_Frame_BOB(&areaMapTilesLayer1[i][j],&layer1,0,cell%TS_NUMTILESL1_X,cell/TS_NUMTILESL1_X,BITMAP_EXTRACT_MODE_CELL);
		}
	}
	Unload_Bitmap_File(&layer1);
	//clear
	DDraw_Fill_Surface(lpddsScreen,0);
	//blit tile bobs onto background screen surface
	for (i=0; i<AREA_HEIGHT; i++) {
		for (j=0; j<AREA_WIDTH; j++) {
			Draw_BOB(&areaMapTiles[i][j],lpddsScreen,0);
		}
	}
	for (i=0; i<AREA_HEIGHT_L1; i++) {
		for (j=0; j<AREA_WIDTH_L1; j++) {
			Draw_BOB(&areaMapTilesLayer1[i][j],lpddsScreen);
		}
	}
}

void DrawAreaMap() {
	RECT dest_rect;   // the destination rectangle

	// fill in the destination rect
	dest_rect.left   = 0;
	dest_rect.top    = 0;
	dest_rect.right  = VP_WIDTH;
	dest_rect.bottom = VP_HEIGHT;
	//blt the background surface
	lpddsback->Blt(&dest_rect,lpddsScreen,NULL,(DDBLT_WAIT),NULL);// | DDBLT_KEYSRC
}

void DrawAreaItems(LPAREA area, LPPLAYER player)
{
	int i;
	LPOBJECT item;
	//blit the items of area
	for (i=0; i<area->invent->size; i++) {
		item=area->invent->list[i];
		if (item->visible) Draw_BOB(&item->bob,lpddsback);
		if (item->enabled && item->animate) {	//if an animated bob...
			Animate_BOB(&item->bob);
			if (item->bob.anim_state==BOB_STATE_ANIM_DONE) {
				item->bob.anim_state=0;	//reset the bobs' animation state
				item->animate=0;		//reset animate flag
				//update state
				if (item->type & ID_TRANS_DOOR) {
					// of door
					if (item->state==ST_DOOROPEN) item->state=ST_DOORCLOSED;
					else item->state=ST_DOOROPEN;
				}
				else if (item->type & ID_DAMAGE) {
					Destroy_BOB(&item->bob);
					ListDelete(area->invent,item);
				}
			}
		}
	}
	//draw current used weapon
	if (player->currWep!=NULL) {
		Draw_BOB(&player->currWep->bob,lpddsback,0,125,0);
	}
	//draw current used equipment
	if (player->currEquip!=NULL) {
		Draw_BOB(&player->currEquip->bob,lpddsback,0,125,50);
	}
}

void CheckObjectInteractions(LPPLAYER player)
{
	int i,j,x,y,dx,dy,width,height;
	int hasKey,inside;
	int ax,ay,az;
	LPOBJECT item,tmpItem;
	LPOBJECTCOL areaItemcol=player->mapposArea->invent;
	LPOBJECTCOL playerItemcol=player->invent;
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////check if player/area has effect/effected by any items or area///////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	// check each object for interaction w/ actors, equipment, weapons, etc
	for (i=0; i<areaItemcol->size; i++) {
		item=areaItemcol->list[i];
		if (!item->enabled) continue;
		width=item->width;
		height=item->height;
		dx=player->x-item->x;
		dy=player->y-item->y;
		//set flag for if we are within radius of object
		if (dx*dx+dy*dy<width*height/4) inside=1;
		else inside=0;
		//...targets
		if (item->type & ID_TARGET) {
			//make sure target is not already destroyed
			if (item->attr[1]>0) {
				for (j=0; j<areaItemcol->size; j++) {
					tmpItem=areaItemcol->list[j];
					if (tmpItem->type & ID_DAMAGE) {
						dx=(item->width+item->height)/4;
						dy=(tmpItem->width+tmpItem->height)/4;
						if (dx*dx+dy*dy>(item->x-tmpItem->x)*(item->x-tmpItem->x)+(item->y-tmpItem->y)*(item->y-tmpItem->y)) {
							item->attr[1]-=tmpItem->attr[0];
						}
					}
				}
			}
			//else {player->energy=-999;}
		}
		//...items to be picked up
		if (inside && item->canTake) {
			tmpItem=ListEnumItemType(playerItemcol,item->type,NULL);
			if (tmpItem!=NULL && item->type!=ID_ITEM_KEYCARD) {
				if (item->quantity>0) {
					tmpItem->quantity+=item->quantity;
					ListDelete(areaItemcol,item);
					item=tmpItem;	//do this so that item still pts to a valid object
				}
			}
			else {
				AddItem(player,ListRemove(areaItemcol,item));
			}
		}
		//...take damage
		if (inside && player->invicibleTicks==0 && (item->type & ID_DAMAGE)) {
			player->energy-=item->attr[0];
			player->invicibleTicks=10;	//make invincible for a bit
		}
		//...doors
		if (item->type & ID_TRANS_DOOR) {
			//check if player is using correct key
			hasKey=0;
			if (player->currEquip!=NULL &&
				player->currEquip->type==ID_ITEM_KEYCARD &&
				player->currEquip->attr[0]==item->attr[5]) hasKey=1;
			if (inside && item->animate==0 &&
				  item->state==ST_DOORCLOSED && hasKey) {
				Set_Animation_BOB(&item->bob,0);
				item->animate=1;
				DSound_Play(snd_doorslide);
			}
			else if (inside && item->state==ST_DOOROPEN) {
				//reset door
				item->state=ST_DOORCLOSED;
				item->animate=0;
				Set_Animation_BOB(&item->bob,0);
				//transport to area and position
				player->mappos.x=ax=item->attr[0];
				player->mappos.y=ay=item->attr[1];
				player->mappos.z=az=item->attr[2];
				player->x=item->attr[3];
				player->y=item->attr[4];
				player->mapposArea=GetWorldArea(&world,ax,ay,az);
				//create the new area map
				CreateAreaMap(player->mapposArea);
			}
			else if (!inside && item->animate==0 &&
					 item->state==ST_DOOROPEN) {
				Set_Animation_BOB(&item->bob,1);
				item->animate=1;
				DSound_Play(snd_doorslide);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void SortNPC()
{
	qsort(npcs,MAX_NUM_NPC,sizeof(LPNPC),SortNPCCompare);
}
int SortNPCCompare(const void* a,const void* b)
{
	LPNPC npc1=(LPNPC)(*(LPNPC*)a), npc2=(LPNPC)(*(LPNPC*)b);
	if (npc1==NULL) return 1;
	if (npc2==NULL) return -1;
	if (npc1->y<npc2->y)
		return -1;
	else
		return 1;
}

