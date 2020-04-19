#ifndef __FIREMAN_H__
#define __FIREMAN_H__

#include "fire.h"

class CFireManager
{
	private:
		int m_nCount;
	public:
		CFire **FireList;
		CFireManager();
		~CFireManager();
		int Create(FireType firetype, int orig_x, int orig_y, int target_x, int target_y);
		void MoveAll();
		void DrawAll(DDSURFACEDESC *ddsd);
		int GetCount();
};

#endif