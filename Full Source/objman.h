#ifndef __OBJMAN_H__
#define __OBJMAN_H__

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "object.h"
#include "timer.h"

class CObjectManager
{
	private:
		int m_nCount;
	public:
		CObject **ObjList;
		ObjEventItem **ObjEventDefs[MAX_OBJECT_EVENTS];
		CObjectManager();
		~CObjectManager();
		void DefineObjectSeqs();
		int Create(ObjectType object, SpriteType object_sprite, SpriteType dying_sprite, UpType up_type, int x, int y, ObjAction obj_event_id);
		void MoveAll();
		void DrawAll(LPDIRECTDRAWSURFACE surface);
		void Reset();
		int GetCount();
};

#endif