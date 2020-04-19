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
		int Create(FireType firetype, int orig_x, int orig_y, int target_x, int target_y, float vector_speed);
		void MoveAll();
		void DrawAllBlit(LPDIRECTDRAWSURFACE surface);
		void DrawAllPlot(DDSURFACEDESC *ddsd);
		void Reset();
		int GetCount();
};

#endif