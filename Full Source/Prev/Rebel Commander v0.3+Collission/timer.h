#include <windows.h>

#include "defines.h"

#ifndef __timer_h__
#define __timer_h__

#define TIME_SEQ_COUNT 5

struct TimeSeqDef
{
	DWORD nTriggerTime;		//when in game time the event(s) will occur

	BOOL bInstObject;
	ObjectType enObjectType;
	SpriteType enObjectSprite;
	SpriteType enObjectDyingSprite;
	int iObjectX;
	int iObjectY;
	ObjEventId enObjectEventId;

	//...more
};

class CTimer
{
	protected:
		int m_nStartTime;
		TimeSeqDef *lpMasterTimeSeq[TIME_SEQ_COUNT];
		void DefineTimeSeq();
	public:
		CTimer();
		virtual void start();
		virtual DWORD time();
		BOOL elapsed(int &start, int interval);
		void PollSeqList();
};

#endif

