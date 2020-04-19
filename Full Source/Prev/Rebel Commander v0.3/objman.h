#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "object.h"

#ifndef __OBJMAN_H__
#define __OBJMAN_H__


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
		int Create(ObjectType object, SpriteType object_sprite, SpriteType dying_sprite, int x, int y, ObjEventId obj_event_id);
		void MoveAll();
		void DrawAll(LPDIRECTDRAWSURFACE surface);
		int GetCount();
};

#endif