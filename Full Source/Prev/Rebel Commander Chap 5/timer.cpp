#include "timer.h"

CTimer::CTimer()
{
	m_nStartTime=0;
}

void CTimer::start()
{
	m_nStartTime=timeGetTime();
}

int CTimer::time()
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

