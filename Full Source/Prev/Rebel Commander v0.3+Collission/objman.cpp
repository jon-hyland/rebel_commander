#include "objman.h"

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

int CObjectManager::Create(ObjectType object, SpriteType object_sprite, SpriteType dying_sprite, int x, int y, ObjEventId obj_event_id)
{
	if(m_nCount<MAX_OBJECT_INSTANCES)
	{
		int i=0;
		while(ObjList[i]!=NULL)i++;	//find first free slot
		ObjList[i]=new CObject(object, object_sprite, dying_sprite, x, y, ObjEventDefs[obj_event_id]);
		m_nCount++;
		return i;
	}
	else return -1;
}


void CObjectManager::MoveAll()
{
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
			ObjList[i]->Move();
}

void CObjectManager::DrawAll(LPDIRECTDRAWSURFACE surface)
{
	for(int i=0; i<MAX_OBJECT_INSTANCES; i++) 
		if(ObjList[i]!=NULL)
			ObjList[i]->Draw(surface);
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

	//definition of BLIP_DEFAULT_ACTION
	ObjEventDefs[BLIP_DEFAULT_ACTION][0]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][0]->iTargetX=1140;
	ObjEventDefs[BLIP_DEFAULT_ACTION][0]->iTargetY=200;
	ObjEventDefs[BLIP_DEFAULT_ACTION][0]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][0]->iNextEventIndex=1;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][1]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][1]->iTargetX=1000;
	ObjEventDefs[BLIP_DEFAULT_ACTION][1]->iTargetY=50;
	ObjEventDefs[BLIP_DEFAULT_ACTION][1]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][1]->iNextEventIndex=2;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][2]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][2]->iTargetX=700;
	ObjEventDefs[BLIP_DEFAULT_ACTION][2]->iTargetY=50;
	ObjEventDefs[BLIP_DEFAULT_ACTION][2]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][2]->iNextEventIndex=3;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][3]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][3]->iTargetX=540;
	ObjEventDefs[BLIP_DEFAULT_ACTION][3]->iTargetY=375;
	ObjEventDefs[BLIP_DEFAULT_ACTION][3]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][3]->iNextEventIndex=5;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][4]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][4]->iTargetX=380;
	ObjEventDefs[BLIP_DEFAULT_ACTION][4]->iTargetY=375;
	ObjEventDefs[BLIP_DEFAULT_ACTION][4]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][4]->iNextEventIndex=5;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][5]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][5]->iTargetX=1;
	ObjEventDefs[BLIP_DEFAULT_ACTION][5]->iTargetY=200;
	ObjEventDefs[BLIP_DEFAULT_ACTION][5]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][5]->iNextEventIndex=6;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][6]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][6]->iTargetX=380;
	ObjEventDefs[BLIP_DEFAULT_ACTION][6]->iTargetY=50;
	ObjEventDefs[BLIP_DEFAULT_ACTION][6]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][6]->iNextEventIndex=7;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][7]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][7]->iTargetX=540;
	ObjEventDefs[BLIP_DEFAULT_ACTION][7]->iTargetY=50;
	ObjEventDefs[BLIP_DEFAULT_ACTION][7]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][7]->iNextEventIndex=8;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][8]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][8]->iTargetX=700;
	ObjEventDefs[BLIP_DEFAULT_ACTION][8]->iTargetY=375;
	ObjEventDefs[BLIP_DEFAULT_ACTION][8]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][8]->iNextEventIndex=9;
	
	ObjEventDefs[BLIP_DEFAULT_ACTION][9]->bMoveToPosition=TRUE;
	ObjEventDefs[BLIP_DEFAULT_ACTION][9]->iTargetX=1000;
	ObjEventDefs[BLIP_DEFAULT_ACTION][9]->iTargetY=375;
	ObjEventDefs[BLIP_DEFAULT_ACTION][9]->iVectorSpeed=5;
	ObjEventDefs[BLIP_DEFAULT_ACTION][9]->iNextEventIndex=0;
	
	for(ii=10; ii<MAX_OBJECT_EVENTS; ii++) { //reset the rest
		ObjEventDefs[BLIP_DEFAULT_ACTION][ii]->bMoveToPosition=FALSE;
		ObjEventDefs[BLIP_DEFAULT_ACTION][ii]->iTargetX=0;
		ObjEventDefs[BLIP_DEFAULT_ACTION][ii]->iTargetY=0;
		ObjEventDefs[BLIP_DEFAULT_ACTION][ii]->iVectorSpeed=0;
		ObjEventDefs[BLIP_DEFAULT_ACTION][ii]->iNextEventIndex=0; 
	}



}