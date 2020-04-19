#include <windows.h>

#ifndef __timer_h__
#define __timer_h__

class CTimer
{
	protected:
		int m_nStartTime;
	public:
		CTimer();
		virtual void start();
		virtual int time();
		BOOL elapsed(int &start, int interval);
};

#endif

