
void AddProjectile(int type, int attr, int posX, int posY, int dx, int dy, int range, LPAREA area) {
	int ind=-1,i,rangeDecr=(int)sqrt(dx*dx+dy*dy);
	for (i=0; i<numProjectilesAllocd; i++)
		if (bullets[i].x==-1) {
			ind=i;
			break;
		}
	if (ind==-1)
		if (numProjectilesAllocd<MAX_PROJ)
			ind=numProjectilesAllocd++;
		else return;
	//
	bullets[ind].attr=attr;
	bullets[ind].x=posX;
	bullets[ind].y=posY;
	bullets[ind].dx=dx;
	bullets[ind].dy=dy;
	bullets[ind].type=type;
	bullets[ind].range=range;
	bullets[ind].rangeDecr=rangeDecr;
	bullets[ind].hasBob=0;
	bullets[ind].area=area;
	//
	int numframe, startframeInd, *animSeq=NULL, frameDelay;
	switch (type) {
	case PT_BULLET: {
		bullets[ind].damage=5;
		break;
	}
	case PT_GRENADE: {
		bullets[ind].damage=10;
		Load_Bitmap_File(&bitmap8bit, "ART/PROJ_8B.BMP");
		bullets[ind].hasBob=1;
		numframe=AMMO_GRENADE_NF; startframeInd=AMMO_GRENADE_SF;
		break;
	}
	default: {
		bullets[ind].type=PT_BULLET;
		bullets[ind].damage=5;
		break;
	}}
	// create BOB if needed
	if (bullets[ind].hasBob) {
		frameDelay=round(range/(double)(rangeDecr*numframe))+1;
		animSeq=(int*)malloc(sizeof(int)*numframe);
		for (i=0; i<numframe; i++)
			animSeq[i]=i;
		bullets[ind].width=AMMO_GRENADE_WIDTH;
		bullets[ind].height=AMMO_GRENADE_HEIGHT;
		Create_BOB(&bullets[ind].bob,posX-AMMO_GRENADE_WIDTH/2,posY-AMMO_GRENADE_HEIGHT/2,
					AMMO_GRENADE_WIDTH,AMMO_GRENADE_HEIGHT,numframe,
					BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM,DDSCAPS_SYSTEMMEMORY);
		for (i=startframeInd; i<startframeInd+numframe; i++)
			Load_Frame_BOB(&bullets[ind].bob,&bitmap8bit,i-startframeInd,i%9,i/9,BITMAP_EXTRACT_MODE_CELL);
		Load_Animation_BOB(&bullets[ind].bob,0,numframe,animSeq);
		Set_Animation_BOB(&bullets[ind].bob,0);
		Set_Anim_Speed_BOB(&bullets[ind].bob,frameDelay);
		Unload_Bitmap_File(&bitmap8bit);
	}
	//clean-up
	if (animSeq!=NULL) free(animSeq);
}

void AnimateProjectiles() {
	int i,j,cx,cy;
	int width,height;
	LPAREA area;
	LPOBJECT item;
	for (i=0; i<numProjectilesAllocd; i++) {
		if (bullets[i].x==-1) continue;
		width=bullets[i].width;
		height=bullets[i].height;
		area=bullets[i].area;
		//
		bullets[i].x+=bullets[i].dx;
		bullets[i].y+=bullets[i].dy;
		bullets[i].range-=bullets[i].rangeDecr;
		if (bullets[i].type==PT_BULLET) {		//if bullet, check for hits during flight
			// player
			if (!(bullets[i].attr & PROJ_ATTR_PLAYER) &&
				abs(pc.x-bullets[i].x)<pc.width/2 &&
				abs(pc.y-bullets[i].y+PROJECTILE_HEIGHT_OFFSET)<pc.height/2 &&
				area==pc.mapposArea) {
				pc.energy-=bullets[i].damage;
				bullets[i].x=bullets[i].y=-1;
			}
			// npcs
			for (j=0;j<MAX_NUM_NPC;j++) {
				if (npcs[j]!=NULL && (bullets[i].attr & PROJ_ATTR_PLAYER) &&
					abs(npcs[j]->x-bullets[i].x)<npcs[j]->width/2 &&
					abs(npcs[j]->y-bullets[i].y+PROJECTILE_HEIGHT_OFFSET)<npcs[j]->height/2 &&
					area==npcs[j]->mapposArea) {
					npcs[j]->energy-=bullets[i].damage;
					bullets[i].x=bullets[i].y=-1;
				}
			}
			// check for any targets that can take "PIERCE" damage from bullets in area
			for (j=0; j<area->invent->size; j++) {
				item=area->invent->list[j];
				if ((item->type & ID_TARGET) && (item->attr[0] & ID_TARGET_TAKE_PIERCE_DAMAGE) && item->enabled) {
					if (abs(item->x-bullets[i].x)<item->width/2 &&
						abs(item->y-bullets[i].y+PROJECTILE_HEIGHT_OFFSET)<item->height/2) {
						item->attr[1]-=bullets[i].damage;
					}
				}
			}
		}
		//check if projectile out of sreen OR out of range OR hits impassable part of area
		cx=bullets[i].x/TILEW;
		cy=(bullets[i].y-PROJECTILE_HEIGHT_OFFSET)/TILEH;
		if (bullets[i].range<0 ||
			bullets[i].x<0 || bullets[i].x>=VP_WIDTH || bullets[i].y<0 || bullets[i].y>=VP_HEIGHT ||
			(area->areaEffects[cy][cx] & AE_NOTPASSABLE)) {
			//add a big boom (not if it only goes out of screen)!
			if (bullets[i].type==PT_GRENADE &&
				!(bullets[i].x<0 || bullets[i].x>=VP_WIDTH || bullets[i].y<0 || bullets[i].y>=VP_HEIGHT)) {
				AddItem(area,ID_DAMAGE_EXPL,0,bullets[i].x,bullets[i].y+10,NULL,0);
			}
			//destroy projectile
			bullets[i].x=bullets[i].y=-1;	//negative x or y pos means its not a valid projectile
			if (bullets[i].hasBob) Destroy_BOB(&bullets[i].bob);
		}
		else {
			if (bullets[i].hasBob) {
				Animate_BOB(&bullets[i].bob);
				Set_Pos_BOB(&bullets[i].bob,bullets[i].x-width/2,bullets[i].y-height/2);
			}
		}
	}
}

void DrawProjectiles(LPPLAYER player) {
	int i;
	UCHAR* dest_ptr;
	LPAREA playerArea=player->mapposArea;
	// lock the display surface & assign a pointer to the memory surface for manipulation
	dest_ptr=DDraw_Lock_Back_Surface();
	for (i=0; i<numProjectilesAllocd; i++) {
		if (bullets[i].x!=-1) {
			if (!bullets[i].hasBob && playerArea==bullets[i].area)
				Draw_Pixel(bullets[i].x, bullets[i].y, 255, dest_ptr, back_lpitch);
		}
	}
	DDraw_Unlock_Back_Surface();
	for (i=0; i<numProjectilesAllocd; i++) {
		if (bullets[i].x!=-1) {
			if (bullets[i].hasBob && playerArea==bullets[i].area)
				Draw_BOB(&bullets[i].bob,lpddsback);
		}
	}
}


