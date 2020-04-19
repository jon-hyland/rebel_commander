#include "objman.h"

extern CTimer Timer;

CObjectManager::CObjectManager()
{
	m_nCount=0;
	ObjList=new CObject*[MAX_OBJECT_INSTANCES];
	for(int i=0; i<MAX_OBJECT_INSTANCES; i++)
		ObjList[i]=NULL;
	DefineObjectSeqs();
}

CObjectManager::~CObjectManager()
{
	for(int i=0; i<MAX_OBJECT_INSTANCES; i++)
		delete ObjList[i];
	delete[]ObjList;
}

int CObjectManager::Create(ObjectType object, SpriteType object_sprite, SpriteType dying_sprite, UpType up_type, int x, int y, ObjAction obj_event_id)
{
	if(m_nCount<MAX_OBJECT_INSTANCES)
	{
		int i=0;
		while(ObjList[i]!=NULL)i++;	//find first free slot
		ObjList[i]=new CObject(object, object_sprite, dying_sprite, up_type, x, y, ObjEventDefs[obj_event_id]);
		m_nCount++;
		return i;
	}
	else return -1;
}


void CObjectManager::MoveAll()
{
	DWORD time=Timer.GameTime();
	
	//delete any dead enemy objects
	for(int i=0; i<MAX_OBJECT_INSTANCES; i++)
		if(ObjList[i]!=NULL)
			if(ObjList[i]->m_bDead)
			{
				m_nCount--;
				delete ObjList[i];
				ObjList[i]=NULL;
			}

	//move each enemy object
	for(i=0; i<MAX_OBJECT_INSTANCES; i++) 
		if(ObjList[i]!=NULL)
			ObjList[i]->Move(time);
}

void CObjectManager::DrawAll(LPDIRECTDRAWSURFACE surface)
{
	DWORD time=Timer.GameTime();
	
	for(int i=0; i<MAX_OBJECT_INSTANCES; i++) 
		if(ObjList[i]!=NULL)
			ObjList[i]->Draw(surface, time);
}

//resets/destroys all objects in object manager
void CObjectManager::Reset()
{
	//delete objects
	for(int i=0; i<MAX_OBJECT_INSTANCES; i++)
		delete ObjList[i];
	delete[]ObjList;

	//recreate objects
	m_nCount=0;
	ObjList=new CObject*[MAX_OBJECT_INSTANCES];
	for(i=0; i<MAX_OBJECT_INSTANCES; i++)
		ObjList[i]=NULL;
	DefineObjectSeqs();
}

int CObjectManager::GetCount() { return m_nCount; }

void CObjectManager::DefineObjectSeqs()
{
	int ii;

	for(int i=0; i<OBJ_EVENT_ID_COUNT; i++)
	{
		ObjEventDefs[i]=new ObjEventItem*[MAX_OBJECT_EVENTS];
		for(ii=0; ii<MAX_OBJECT_EVENTS; ii++)
			ObjEventDefs[i][ii]=new ObjEventItem;
	}

	//definition of OBJECT_ACTION_ONE
	ObjEventDefs[OBJECT_ACTION_ONE][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][0]->iTargetX=1140;
	ObjEventDefs[OBJECT_ACTION_ONE][0]->iTargetY=200;
	ObjEventDefs[OBJECT_ACTION_ONE][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][0]->iNextEventIndex=1;
	
	ObjEventDefs[OBJECT_ACTION_ONE][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][1]->iTargetX=1000;
	ObjEventDefs[OBJECT_ACTION_ONE][1]->iTargetY=50;
	ObjEventDefs[OBJECT_ACTION_ONE][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][1]->iNextEventIndex=2;
	
	ObjEventDefs[OBJECT_ACTION_ONE][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][2]->iTargetX=800;
	ObjEventDefs[OBJECT_ACTION_ONE][2]->iTargetY=50;
	ObjEventDefs[OBJECT_ACTION_ONE][2]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][2]->iNextEventIndex=3;
	
	ObjEventDefs[OBJECT_ACTION_ONE][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][3]->iTargetX=480;
	ObjEventDefs[OBJECT_ACTION_ONE][3]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_ONE][3]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][3]->iNextEventIndex=4;
	
	ObjEventDefs[OBJECT_ACTION_ONE][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][4]->iTargetX=300;
	ObjEventDefs[OBJECT_ACTION_ONE][4]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_ONE][4]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][4]->iNextEventIndex=5;
	
	ObjEventDefs[OBJECT_ACTION_ONE][5]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][5]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][5]->iTargetX=150;
	ObjEventDefs[OBJECT_ACTION_ONE][5]->iTargetY=200;
	ObjEventDefs[OBJECT_ACTION_ONE][5]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][5]->iNextEventIndex=6;
	
	ObjEventDefs[OBJECT_ACTION_ONE][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][6]->iTargetX=300;
	ObjEventDefs[OBJECT_ACTION_ONE][6]->iTargetY=50;
	ObjEventDefs[OBJECT_ACTION_ONE][6]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][6]->iNextEventIndex=7;
	
	ObjEventDefs[OBJECT_ACTION_ONE][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][7]->iTargetX=480;
	ObjEventDefs[OBJECT_ACTION_ONE][7]->iTargetY=50;
	ObjEventDefs[OBJECT_ACTION_ONE][7]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][7]->iNextEventIndex=8;
	
	ObjEventDefs[OBJECT_ACTION_ONE][8]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][8]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][8]->iTargetX=800;
	ObjEventDefs[OBJECT_ACTION_ONE][8]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_ONE][8]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][8]->iNextEventIndex=9;
	
	ObjEventDefs[OBJECT_ACTION_ONE][9]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ONE][9]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ONE][9]->iTargetX=1000;
	ObjEventDefs[OBJECT_ACTION_ONE][9]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_ONE][9]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ONE][9]->iNextEventIndex=0;
	
	for(ii=10; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_ONE][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_ONE][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_ONE][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_ONE][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_ONE][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_ONE][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_TWO
	ObjEventDefs[OBJECT_ACTION_TWO][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][0]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_TWO][0]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_TWO][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWO][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_TWO][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][1]->iTargetX=725;
	ObjEventDefs[OBJECT_ACTION_TWO][1]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_TWO][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWO][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_TWO][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][2]->iTargetX=650;
	ObjEventDefs[OBJECT_ACTION_TWO][2]->iTargetY=162;
	ObjEventDefs[OBJECT_ACTION_TWO][2]->iVectorSpeed=6.5;
	ObjEventDefs[OBJECT_ACTION_TWO][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_TWO][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][3]->iTargetX=480;
	ObjEventDefs[OBJECT_ACTION_TWO][3]->iTargetY=162;
	ObjEventDefs[OBJECT_ACTION_TWO][3]->iVectorSpeed=5.5;
	ObjEventDefs[OBJECT_ACTION_TWO][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_TWO][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][4]->iTargetX=350;
	ObjEventDefs[OBJECT_ACTION_TWO][4]->iTargetY=200;
	ObjEventDefs[OBJECT_ACTION_TWO][4]->iVectorSpeed=6.5;
	ObjEventDefs[OBJECT_ACTION_TWO][4]->iNextEventIndex=5;

	ObjEventDefs[OBJECT_ACTION_TWO][5]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][5]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][5]->iTargetX=230;
	ObjEventDefs[OBJECT_ACTION_TWO][5]->iTargetY=200;
	ObjEventDefs[OBJECT_ACTION_TWO][5]->iVectorSpeed=5.5;
	ObjEventDefs[OBJECT_ACTION_TWO][5]->iNextEventIndex=6;

	ObjEventDefs[OBJECT_ACTION_TWO][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][6]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_TWO][6]->iTargetY=200;
	ObjEventDefs[OBJECT_ACTION_TWO][6]->iVectorSpeed=8;
	ObjEventDefs[OBJECT_ACTION_TWO][6]->iNextEventIndex=7;

	ObjEventDefs[OBJECT_ACTION_TWO][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWO][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWO][7]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_TWO][7]->iTargetY=100;
	ObjEventDefs[OBJECT_ACTION_TWO][7]->iVectorSpeed=1;
	ObjEventDefs[OBJECT_ACTION_TWO][7]->iNextEventIndex=0;

	for(ii=8; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_TWO][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_TWO][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_TWO][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_TWO][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_TWO][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_TWO][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_THREE
	ObjEventDefs[OBJECT_ACTION_THREE][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][0]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_THREE][0]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_THREE][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_THREE][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_THREE][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][1]->iTargetX=725;
	ObjEventDefs[OBJECT_ACTION_THREE][1]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_THREE][1]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_THREE][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][2]->iTargetX=650;
	ObjEventDefs[OBJECT_ACTION_THREE][2]->iTargetY=45;
	ObjEventDefs[OBJECT_ACTION_THREE][2]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_THREE][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][3]->iTargetX=480;
	ObjEventDefs[OBJECT_ACTION_THREE][3]->iTargetY=45;
	ObjEventDefs[OBJECT_ACTION_THREE][3]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_THREE][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][4]->iTargetX=350;
	ObjEventDefs[OBJECT_ACTION_THREE][4]->iTargetY=10;
	ObjEventDefs[OBJECT_ACTION_THREE][4]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][4]->iNextEventIndex=5;

	ObjEventDefs[OBJECT_ACTION_THREE][5]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][5]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][5]->iTargetX=230;
	ObjEventDefs[OBJECT_ACTION_THREE][5]->iTargetY=10;
	ObjEventDefs[OBJECT_ACTION_THREE][5]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][5]->iNextEventIndex=6;

	ObjEventDefs[OBJECT_ACTION_THREE][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][6]->iTargetX=450;
	ObjEventDefs[OBJECT_ACTION_THREE][6]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_THREE][6]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][6]->iNextEventIndex=7;

	ObjEventDefs[OBJECT_ACTION_THREE][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][7]->iTargetX=350;
	ObjEventDefs[OBJECT_ACTION_THREE][7]->iTargetY=200;
	ObjEventDefs[OBJECT_ACTION_THREE][7]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][7]->iNextEventIndex=8;

	ObjEventDefs[OBJECT_ACTION_THREE][8]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][8]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][8]->iTargetX=450;
	ObjEventDefs[OBJECT_ACTION_THREE][8]->iTargetY=325;
	ObjEventDefs[OBJECT_ACTION_THREE][8]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][8]->iNextEventIndex=9;

	ObjEventDefs[OBJECT_ACTION_THREE][9]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][9]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][9]->iTargetX=230;
	ObjEventDefs[OBJECT_ACTION_THREE][9]->iTargetY=390;
	ObjEventDefs[OBJECT_ACTION_THREE][9]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][9]->iNextEventIndex=10;

	ObjEventDefs[OBJECT_ACTION_THREE][10]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][10]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][10]->iTargetX=350;
	ObjEventDefs[OBJECT_ACTION_THREE][10]->iTargetY=390;
	ObjEventDefs[OBJECT_ACTION_THREE][10]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][10]->iNextEventIndex=11;

	ObjEventDefs[OBJECT_ACTION_THREE][11]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][11]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][11]->iTargetX=480;
	ObjEventDefs[OBJECT_ACTION_THREE][11]->iTargetY=355;
	ObjEventDefs[OBJECT_ACTION_THREE][11]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][11]->iNextEventIndex=12;

	ObjEventDefs[OBJECT_ACTION_THREE][12]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][12]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][12]->iTargetX=650;
	ObjEventDefs[OBJECT_ACTION_THREE][12]->iTargetY=355;
	ObjEventDefs[OBJECT_ACTION_THREE][12]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][12]->iNextEventIndex=13;

	ObjEventDefs[OBJECT_ACTION_THREE][13]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][13]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][13]->iTargetX=725;
	ObjEventDefs[OBJECT_ACTION_THREE][13]->iTargetY=325;
	ObjEventDefs[OBJECT_ACTION_THREE][13]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][13]->iNextEventIndex=14;

	ObjEventDefs[OBJECT_ACTION_THREE][14]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_THREE][14]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_THREE][14]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_THREE][14]->iTargetY=325;
	ObjEventDefs[OBJECT_ACTION_THREE][14]->iVectorSpeed=7;
	ObjEventDefs[OBJECT_ACTION_THREE][14]->iNextEventIndex=0;

	for(ii=15; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_THREE][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_THREE][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_THREE][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_THREE][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_THREE][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_THREE][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_FOUR
	ObjEventDefs[OBJECT_ACTION_FOUR][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][0]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_FOUR][0]->iTargetY=345;
	ObjEventDefs[OBJECT_ACTION_FOUR][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_FOUR][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_FOUR][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][1]->iTargetX=725;
	ObjEventDefs[OBJECT_ACTION_FOUR][1]->iTargetY=345;
	ObjEventDefs[OBJECT_ACTION_FOUR][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_FOUR][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_FOUR][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][2]->iTargetX=650;
	ObjEventDefs[OBJECT_ACTION_FOUR][2]->iTargetY=258;
	ObjEventDefs[OBJECT_ACTION_FOUR][2]->iVectorSpeed=6.5;
	ObjEventDefs[OBJECT_ACTION_FOUR][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_FOUR][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][3]->iTargetX=480;
	ObjEventDefs[OBJECT_ACTION_FOUR][3]->iTargetY=258;
	ObjEventDefs[OBJECT_ACTION_FOUR][3]->iVectorSpeed=5.5;
	ObjEventDefs[OBJECT_ACTION_FOUR][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_FOUR][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][4]->iTargetX=350;
	ObjEventDefs[OBJECT_ACTION_FOUR][4]->iTargetY=220;
	ObjEventDefs[OBJECT_ACTION_FOUR][4]->iVectorSpeed=6.5;
	ObjEventDefs[OBJECT_ACTION_FOUR][4]->iNextEventIndex=5;

	ObjEventDefs[OBJECT_ACTION_FOUR][5]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][5]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][5]->iTargetX=230;
	ObjEventDefs[OBJECT_ACTION_FOUR][5]->iTargetY=220;
	ObjEventDefs[OBJECT_ACTION_FOUR][5]->iVectorSpeed=5.5;
	ObjEventDefs[OBJECT_ACTION_FOUR][5]->iNextEventIndex=6;

	ObjEventDefs[OBJECT_ACTION_FOUR][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][6]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_FOUR][6]->iTargetY=220;
	ObjEventDefs[OBJECT_ACTION_FOUR][6]->iVectorSpeed=8;
	ObjEventDefs[OBJECT_ACTION_FOUR][6]->iNextEventIndex=7;

	ObjEventDefs[OBJECT_ACTION_FOUR][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FOUR][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FOUR][7]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_FOUR][7]->iTargetY=320;
	ObjEventDefs[OBJECT_ACTION_FOUR][7]->iVectorSpeed=1;
	ObjEventDefs[OBJECT_ACTION_FOUR][7]->iNextEventIndex=0;

	for(ii=8; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_FOUR][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_FOUR][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_FOUR][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_FOUR][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_FOUR][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_FOUR][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_FIVE
	ObjEventDefs[OBJECT_ACTION_FIVE][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FIVE][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FIVE][0]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_FIVE][0]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_FIVE][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_FIVE][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_FIVE][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FIVE][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FIVE][1]->iTargetX=540;
	ObjEventDefs[OBJECT_ACTION_FIVE][1]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_FIVE][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_FIVE][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_FIVE][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FIVE][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FIVE][2]->iTargetX=540;
	ObjEventDefs[OBJECT_ACTION_FIVE][2]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_FIVE][2]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_FIVE][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_FIVE][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FIVE][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FIVE][3]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_FIVE][3]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_FIVE][3]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_FIVE][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_FIVE][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_FIVE][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_FIVE][4]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_FIVE][4]->iTargetY=80;
	ObjEventDefs[OBJECT_ACTION_FIVE][4]->iVectorSpeed=1;
	ObjEventDefs[OBJECT_ACTION_FIVE][4]->iNextEventIndex=0;

	for(ii=5; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_FIVE][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_FIVE][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_FIVE][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_FIVE][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_FIVE][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_FIVE][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_SIX
	ObjEventDefs[OBJECT_ACTION_SIX][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][0]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_SIX][0]->iTargetY=125;
	ObjEventDefs[OBJECT_ACTION_SIX][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_SIX][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][1]->iTargetX=675;
	ObjEventDefs[OBJECT_ACTION_SIX][1]->iTargetY=125;
	ObjEventDefs[OBJECT_ACTION_SIX][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_SIX][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][2]->iTargetX=600;
	ObjEventDefs[OBJECT_ACTION_SIX][2]->iTargetY=175;
	ObjEventDefs[OBJECT_ACTION_SIX][2]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_SIX][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][3]->iTargetX=725;
	ObjEventDefs[OBJECT_ACTION_SIX][3]->iTargetY=175;
	ObjEventDefs[OBJECT_ACTION_SIX][3]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_SIX][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][4]->iTargetX=725;
	ObjEventDefs[OBJECT_ACTION_SIX][4]->iTargetY=275;
	ObjEventDefs[OBJECT_ACTION_SIX][4]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][4]->iNextEventIndex=5;

	ObjEventDefs[OBJECT_ACTION_SIX][5]->bMoveToPosition=FALSE;	//!!
	ObjEventDefs[OBJECT_ACTION_SIX][5]->bFireWeapon=TRUE;		//!! fire
	ObjEventDefs[OBJECT_ACTION_SIX][5]->iTargetX=-1;
	ObjEventDefs[OBJECT_ACTION_SIX][5]->iTargetY=-1;
	ObjEventDefs[OBJECT_ACTION_SIX][5]->iVectorSpeed=8;
	ObjEventDefs[OBJECT_ACTION_SIX][5]->iNextEventIndex=6;
	
	ObjEventDefs[OBJECT_ACTION_SIX][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][6]->iTargetX=600;
	ObjEventDefs[OBJECT_ACTION_SIX][6]->iTargetY=275;
	ObjEventDefs[OBJECT_ACTION_SIX][6]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][6]->iNextEventIndex=7;

	ObjEventDefs[OBJECT_ACTION_SIX][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][7]->iTargetX=675;
	ObjEventDefs[OBJECT_ACTION_SIX][7]->iTargetY=325;
	ObjEventDefs[OBJECT_ACTION_SIX][7]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][7]->iNextEventIndex=8;

	ObjEventDefs[OBJECT_ACTION_SIX][8]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][8]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][8]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_SIX][8]->iTargetY=325;
	ObjEventDefs[OBJECT_ACTION_SIX][8]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][8]->iNextEventIndex=9;

	ObjEventDefs[OBJECT_ACTION_SIX][9]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SIX][9]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SIX][9]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_SIX][9]->iTargetY=80;
	ObjEventDefs[OBJECT_ACTION_SIX][9]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_SIX][9]->iNextEventIndex=0;

	for(ii=10; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_SIX][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_SIX][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_SIX][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_SIX][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_SIX][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_SIX][ii]->iNextEventIndex=0; 
	}

	//definition of OBJECT_ACTION_SEVEN (moves toward ship)
	ObjEventDefs[OBJECT_ACTION_SEVEN][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_SEVEN][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_SEVEN][0]->iTargetX=-1; //toward ship
	ObjEventDefs[OBJECT_ACTION_SEVEN][0]->iTargetY=-1; //toward ship
	ObjEventDefs[OBJECT_ACTION_SEVEN][0]->iVectorSpeed=3;
	ObjEventDefs[OBJECT_ACTION_SEVEN][0]->iNextEventIndex=0;

	for(ii=1; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_SEVEN][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_SEVEN][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_SEVEN][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_SEVEN][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_SEVEN][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_SEVEN][ii]->iNextEventIndex=0; 
	}

	//definition of OBJECT_ACTION_EIGHT (moves toward ship, fast)
	ObjEventDefs[OBJECT_ACTION_EIGHT][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_EIGHT][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_EIGHT][0]->iTargetX=-1; //toward ship
	ObjEventDefs[OBJECT_ACTION_EIGHT][0]->iTargetY=-1; //toward ship
	ObjEventDefs[OBJECT_ACTION_EIGHT][0]->iVectorSpeed=6;
	ObjEventDefs[OBJECT_ACTION_EIGHT][0]->iNextEventIndex=0;

	for(ii=1; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_EIGHT][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_EIGHT][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_EIGHT][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_EIGHT][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_EIGHT][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_EIGHT][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_NINE
	ObjEventDefs[OBJECT_ACTION_NINE][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][0]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_NINE][0]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_NINE][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_NINE][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][1]->iTargetX=625;
	ObjEventDefs[OBJECT_ACTION_NINE][1]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_NINE][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_NINE][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][2]->iTargetX=600;
	ObjEventDefs[OBJECT_ACTION_NINE][2]->iTargetY=80;
	ObjEventDefs[OBJECT_ACTION_NINE][2]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_NINE][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][3]->iTargetX=575;
	ObjEventDefs[OBJECT_ACTION_NINE][3]->iTargetY=92;
	ObjEventDefs[OBJECT_ACTION_NINE][3]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_NINE][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][4]->iTargetX=550;
	ObjEventDefs[OBJECT_ACTION_NINE][4]->iTargetY=107;
	ObjEventDefs[OBJECT_ACTION_NINE][4]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][4]->iNextEventIndex=5;

	ObjEventDefs[OBJECT_ACTION_NINE][5]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][5]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][5]->iTargetX=525;
	ObjEventDefs[OBJECT_ACTION_NINE][5]->iTargetY=127;
	ObjEventDefs[OBJECT_ACTION_NINE][5]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][5]->iNextEventIndex=6;

	ObjEventDefs[OBJECT_ACTION_NINE][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][6]->iTargetX=500;
	ObjEventDefs[OBJECT_ACTION_NINE][6]->iTargetY=175;
	ObjEventDefs[OBJECT_ACTION_NINE][6]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][6]->iNextEventIndex=7;

	ObjEventDefs[OBJECT_ACTION_NINE][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][7]->iTargetX=490;
	ObjEventDefs[OBJECT_ACTION_NINE][7]->iTargetY=220;
	ObjEventDefs[OBJECT_ACTION_NINE][7]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][7]->iNextEventIndex=8;

	ObjEventDefs[OBJECT_ACTION_NINE][8]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][8]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][8]->iTargetX=500;
	ObjEventDefs[OBJECT_ACTION_NINE][8]->iTargetY=265;
	ObjEventDefs[OBJECT_ACTION_NINE][8]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][8]->iNextEventIndex=9;

	ObjEventDefs[OBJECT_ACTION_NINE][9]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][9]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][9]->iTargetX=525;
	ObjEventDefs[OBJECT_ACTION_NINE][9]->iTargetY=313;
	ObjEventDefs[OBJECT_ACTION_NINE][9]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][9]->iNextEventIndex=10;

	ObjEventDefs[OBJECT_ACTION_NINE][10]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][10]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][10]->iTargetX=550;
	ObjEventDefs[OBJECT_ACTION_NINE][10]->iTargetY=333;
	ObjEventDefs[OBJECT_ACTION_NINE][10]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][10]->iNextEventIndex=11;

	ObjEventDefs[OBJECT_ACTION_NINE][11]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][11]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][11]->iTargetX=575;
	ObjEventDefs[OBJECT_ACTION_NINE][11]->iTargetY=348;
	ObjEventDefs[OBJECT_ACTION_NINE][11]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][11]->iNextEventIndex=12;

	ObjEventDefs[OBJECT_ACTION_NINE][12]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][12]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][12]->iTargetX=600;
	ObjEventDefs[OBJECT_ACTION_NINE][12]->iTargetY=360;
	ObjEventDefs[OBJECT_ACTION_NINE][12]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][12]->iNextEventIndex=13;

	ObjEventDefs[OBJECT_ACTION_NINE][13]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][13]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][13]->iTargetX=625;
	ObjEventDefs[OBJECT_ACTION_NINE][13]->iTargetY=365;
	ObjEventDefs[OBJECT_ACTION_NINE][13]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][13]->iNextEventIndex=14;

	ObjEventDefs[OBJECT_ACTION_NINE][14]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][14]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][14]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_NINE][14]->iTargetY=365;
	ObjEventDefs[OBJECT_ACTION_NINE][14]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][14]->iNextEventIndex=15;

	ObjEventDefs[OBJECT_ACTION_NINE][15]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_NINE][15]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_NINE][15]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_NINE][15]->iTargetY=125;
	ObjEventDefs[OBJECT_ACTION_NINE][15]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_NINE][15]->iNextEventIndex=0;

	for(ii=16; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_NINE][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_NINE][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_NINE][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_NINE][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_NINE][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_NINE][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_TEN
	ObjEventDefs[OBJECT_ACTION_TEN][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][0]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_TEN][0]->iTargetY=62;
	ObjEventDefs[OBJECT_ACTION_TEN][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_TEN][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][1]->iTargetX=750;
	ObjEventDefs[OBJECT_ACTION_TEN][1]->iTargetY=144;
	ObjEventDefs[OBJECT_ACTION_TEN][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_TEN][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][2]->iTargetX=825;
	ObjEventDefs[OBJECT_ACTION_TEN][2]->iTargetY=144;
	ObjEventDefs[OBJECT_ACTION_TEN][2]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_TEN][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][3]->iTargetX=625;
	ObjEventDefs[OBJECT_ACTION_TEN][3]->iTargetY=213;
	ObjEventDefs[OBJECT_ACTION_TEN][3]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_TEN][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][4]->iTargetX=825;
	ObjEventDefs[OBJECT_ACTION_TEN][4]->iTargetY=213;
	ObjEventDefs[OBJECT_ACTION_TEN][4]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][4]->iNextEventIndex=5;

	ObjEventDefs[OBJECT_ACTION_TEN][5]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][5]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][5]->iTargetX=500;
	ObjEventDefs[OBJECT_ACTION_TEN][5]->iTargetY=282;
	ObjEventDefs[OBJECT_ACTION_TEN][5]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][5]->iNextEventIndex=6;

	ObjEventDefs[OBJECT_ACTION_TEN][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][6]->iTargetX=825;
	ObjEventDefs[OBJECT_ACTION_TEN][6]->iTargetY=282;
	ObjEventDefs[OBJECT_ACTION_TEN][6]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][6]->iNextEventIndex=7;

	ObjEventDefs[OBJECT_ACTION_TEN][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][7]->iTargetX=375;
	ObjEventDefs[OBJECT_ACTION_TEN][7]->iTargetY=350;
	ObjEventDefs[OBJECT_ACTION_TEN][7]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][7]->iNextEventIndex=8;

	ObjEventDefs[OBJECT_ACTION_TEN][8]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][8]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][8]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_TEN][8]->iTargetY=350;
	ObjEventDefs[OBJECT_ACTION_TEN][8]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][8]->iNextEventIndex=9;

	ObjEventDefs[OBJECT_ACTION_TEN][9]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TEN][9]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TEN][9]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_TEN][9]->iTargetY=125;
	ObjEventDefs[OBJECT_ACTION_TEN][9]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TEN][9]->iNextEventIndex=0;

	for(ii=10; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_TEN][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_TEN][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_TEN][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_TEN][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_TEN][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_TEN][ii]->iNextEventIndex=0; 
	}

	//definition of OBJECT_ACTION_ELEVEN
	ObjEventDefs[OBJECT_ACTION_ELEVEN][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][0]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][0]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_ELEVEN][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][1]->iTargetX=540;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][1]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_ELEVEN][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][2]->iTargetX=540;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][2]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][2]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_ELEVEN][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][3]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][3]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][3]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_ELEVEN][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][4]->iTargetX=880;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][4]->iTargetY=300;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][4]->iVectorSpeed=1;
	ObjEventDefs[OBJECT_ACTION_ELEVEN][4]->iNextEventIndex=0;

	for(ii=5; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_ELEVEN][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_ELEVEN][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_ELEVEN][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_ELEVEN][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_ELEVEN][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_ELEVEN][ii]->iNextEventIndex=0; 
	}


	//definition of OBJECT_ACTION_TWELVE
	ObjEventDefs[OBJECT_ACTION_TWELVE][0]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][0]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][0]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_TWELVE][0]->iTargetY=365;
	ObjEventDefs[OBJECT_ACTION_TWELVE][0]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][0]->iNextEventIndex=1;

	ObjEventDefs[OBJECT_ACTION_TWELVE][1]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][1]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][1]->iTargetX=625;
	ObjEventDefs[OBJECT_ACTION_TWELVE][1]->iTargetY=375;
	ObjEventDefs[OBJECT_ACTION_TWELVE][1]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][1]->iNextEventIndex=2;

	ObjEventDefs[OBJECT_ACTION_TWELVE][2]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][2]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][2]->iTargetX=600;
	ObjEventDefs[OBJECT_ACTION_TWELVE][2]->iTargetY=360;
	ObjEventDefs[OBJECT_ACTION_TWELVE][2]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][2]->iNextEventIndex=3;

	ObjEventDefs[OBJECT_ACTION_TWELVE][3]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][3]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][3]->iTargetX=575;
	ObjEventDefs[OBJECT_ACTION_TWELVE][3]->iTargetY=348;
	ObjEventDefs[OBJECT_ACTION_TWELVE][3]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][3]->iNextEventIndex=4;

	ObjEventDefs[OBJECT_ACTION_TWELVE][4]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][4]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][4]->iTargetX=550;
	ObjEventDefs[OBJECT_ACTION_TWELVE][4]->iTargetY=333;
	ObjEventDefs[OBJECT_ACTION_TWELVE][4]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][4]->iNextEventIndex=5;

	ObjEventDefs[OBJECT_ACTION_TWELVE][5]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][5]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][5]->iTargetX=525;
	ObjEventDefs[OBJECT_ACTION_TWELVE][5]->iTargetY=313;
	ObjEventDefs[OBJECT_ACTION_TWELVE][5]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][5]->iNextEventIndex=6;

	ObjEventDefs[OBJECT_ACTION_TWELVE][6]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][6]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][6]->iTargetX=500;
	ObjEventDefs[OBJECT_ACTION_TWELVE][6]->iTargetY=265;
	ObjEventDefs[OBJECT_ACTION_TWELVE][6]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][6]->iNextEventIndex=7;

	ObjEventDefs[OBJECT_ACTION_TWELVE][7]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][7]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][7]->iTargetX=490;
	ObjEventDefs[OBJECT_ACTION_TWELVE][7]->iTargetY=220;
	ObjEventDefs[OBJECT_ACTION_TWELVE][7]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][7]->iNextEventIndex=8;

	ObjEventDefs[OBJECT_ACTION_TWELVE][8]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][8]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][8]->iTargetX=500;
	ObjEventDefs[OBJECT_ACTION_TWELVE][8]->iTargetY=175;
	ObjEventDefs[OBJECT_ACTION_TWELVE][8]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][8]->iNextEventIndex=9;

	ObjEventDefs[OBJECT_ACTION_TWELVE][9]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][9]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][9]->iTargetX=525;
	ObjEventDefs[OBJECT_ACTION_TWELVE][9]->iTargetY=127;
	ObjEventDefs[OBJECT_ACTION_TWELVE][9]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][9]->iNextEventIndex=10;

	ObjEventDefs[OBJECT_ACTION_TWELVE][10]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][10]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][10]->iTargetX=550;
	ObjEventDefs[OBJECT_ACTION_TWELVE][10]->iTargetY=107;
	ObjEventDefs[OBJECT_ACTION_TWELVE][10]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][10]->iNextEventIndex=11;

	ObjEventDefs[OBJECT_ACTION_TWELVE][11]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][11]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][11]->iTargetX=575;
	ObjEventDefs[OBJECT_ACTION_TWELVE][11]->iTargetY=92;
	ObjEventDefs[OBJECT_ACTION_TWELVE][11]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][11]->iNextEventIndex=12;

	ObjEventDefs[OBJECT_ACTION_TWELVE][12]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][12]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][12]->iTargetX=600;
	ObjEventDefs[OBJECT_ACTION_TWELVE][12]->iTargetY=80;
	ObjEventDefs[OBJECT_ACTION_TWELVE][12]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][12]->iNextEventIndex=13;

	ObjEventDefs[OBJECT_ACTION_TWELVE][13]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][13]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][13]->iTargetX=625;
	ObjEventDefs[OBJECT_ACTION_TWELVE][13]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_TWELVE][13]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][13]->iNextEventIndex=14;

	ObjEventDefs[OBJECT_ACTION_TWELVE][14]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][14]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][14]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_TWELVE][14]->iTargetY=75;
	ObjEventDefs[OBJECT_ACTION_TWELVE][14]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][14]->iNextEventIndex=15;

	ObjEventDefs[OBJECT_ACTION_TWELVE][15]->bMoveToPosition=TRUE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][15]->bFireWeapon=FALSE;
	ObjEventDefs[OBJECT_ACTION_TWELVE][15]->iTargetX=900;
	ObjEventDefs[OBJECT_ACTION_TWELVE][15]->iTargetY=265;
	ObjEventDefs[OBJECT_ACTION_TWELVE][15]->iVectorSpeed=5;
	ObjEventDefs[OBJECT_ACTION_TWELVE][15]->iNextEventIndex=0;

	for(ii=16; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[OBJECT_ACTION_TWELVE][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[OBJECT_ACTION_TWELVE][ii]->bFireWeapon=FALSE;
		ObjEventDefs[OBJECT_ACTION_TWELVE][ii]->iTargetX=0;
		ObjEventDefs[OBJECT_ACTION_TWELVE][ii]->iTargetY=0;
		ObjEventDefs[OBJECT_ACTION_TWELVE][ii]->iVectorSpeed=0;
		ObjEventDefs[OBJECT_ACTION_TWELVE][ii]->iNextEventIndex=0; 
	}


}