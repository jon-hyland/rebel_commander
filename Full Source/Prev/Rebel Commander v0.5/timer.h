#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>

#include "defines.h"


class CTimer
{
	protected:
		int m_nGameStartTime;
	public:
		CTimer();
		virtual void Start();
		virtual DWORD GameTime();
		void SetGameTime(DWORD NewGameTime);
		BOOL Elapsed(int &start, int interval);
};

#endif

