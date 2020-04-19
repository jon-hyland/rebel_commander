#include "timer.h"

#include "objman.h"

extern CObjectManager ObjectManager;
extern int Junk_ObjectsCreated;

CTimer::CTimer()
{
	m_nStartTime=0;
	DefineTimeSeq();
}

void CTimer::start()
{
	m_nStartTime=timeGetTime();
}

DWORD CTimer::time()
{
	return timeGetTime()-m_nStartTime;
}

int CTimer::elapsed(int &start, int interval)
{
	int current_time=time();
	if(current_time>=start+interval)
	{
		start=current_time;
		return TRUE;
	}
	else return FALSE;
}

void CTimer::PollSeqList()
{
	static int current_seq=0;

	if(current_seq<TIME_SEQ_COUNT)
		if(time()>=lpMasterTimeSeq[current_seq]->nTriggerTime)
		{
			if(lpMasterTimeSeq[current_seq]->bInstObject)
			{
				ObjectManager.Create(lpMasterTimeSeq[current_seq]->enObjectType, 
					lpMasterTimeSeq[current_seq]->enObjectSprite,
					lpMasterTimeSeq[current_seq]->enObjectDyingSprite,
					lpMasterTimeSeq[current_seq]->iObjectX, 
					lpMasterTimeSeq[current_seq]->iObjectY, 
					lpMasterTimeSeq[current_seq]->enObjectEventId);
			
				current_seq++;
			}
		}
}

void CTimer::DefineTimeSeq()
{
	for (int i=0; i<TIME_SEQ_COUNT; i++)
		lpMasterTimeSeq[i]=new TimeSeqDef;
	
	lpMasterTimeSeq[0]->nTriggerTime=0;
	lpMasterTimeSeq[0]->bInstObject=TRUE;
	lpMasterTimeSeq[0]->enObjectType=BLIP_OBJECT;
	lpMasterTimeSeq[0]->enObjectSprite=BLIP_SPRITE;
	lpMasterTimeSeq[0]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	lpMasterTimeSeq[0]->iObjectX=1255;
	lpMasterTimeSeq[0]->iObjectY=220;
	lpMasterTimeSeq[0]->enObjectEventId=BLIP_DEFAULT_ACTION;


	lpMasterTimeSeq[1]->nTriggerTime=150;
	lpMasterTimeSeq[1]->bInstObject=TRUE;
	lpMasterTimeSeq[1]->enObjectType=BLIP_OBJECT;
	lpMasterTimeSeq[1]->enObjectSprite=BLIP_SPRITE;
	lpMasterTimeSeq[1]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	lpMasterTimeSeq[1]->iObjectX=1255;
	lpMasterTimeSeq[1]->iObjectY=220;
	lpMasterTimeSeq[1]->enObjectEventId=BLIP_DEFAULT_ACTION;

	lpMasterTimeSeq[2]->nTriggerTime=300;
	lpMasterTimeSeq[2]->bInstObject=TRUE;
	lpMasterTimeSeq[2]->enObjectType=BLIP_OBJECT;
	lpMasterTimeSeq[2]->enObjectSprite=BLIP_SPRITE;
	lpMasterTimeSeq[2]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	lpMasterTimeSeq[2]->iObjectX=1255;
	lpMasterTimeSeq[2]->iObjectY=220;
	lpMasterTimeSeq[2]->enObjectEventId=BLIP_DEFAULT_ACTION;

	lpMasterTimeSeq[3]->nTriggerTime=450;
	lpMasterTimeSeq[3]->bInstObject=TRUE;
	lpMasterTimeSeq[3]->enObjectType=BLIP_OBJECT;
	lpMasterTimeSeq[3]->enObjectSprite=BLIP_SPRITE;
	lpMasterTimeSeq[3]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	lpMasterTimeSeq[3]->iObjectX=1255;
	lpMasterTimeSeq[3]->iObjectY=220;
	lpMasterTimeSeq[3]->enObjectEventId=BLIP_DEFAULT_ACTION;

	lpMasterTimeSeq[4]->nTriggerTime=600;
	lpMasterTimeSeq[4]->bInstObject=TRUE;
	lpMasterTimeSeq[4]->enObjectType=BLIP_OBJECT;
	lpMasterTimeSeq[4]->enObjectSprite=BLIP_SPRITE;
	lpMasterTimeSeq[4]->enObjectDyingSprite=BLIP_DYING_SPRITE;
	lpMasterTimeSeq[4]->iObjectX=1255;
	lpMasterTimeSeq[4]->iObjectY=220;
	lpMasterTimeSeq[4]->enObjectEventId=BLIP_DEFAULT_ACTION;
}
