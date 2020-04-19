#include "timer.h"

#include "objman.h"

extern CObjectManager ObjectManager;
extern int Junk_ObjectsCreated;

CTimer::CTimer()
{
	m_nGameStartTime=0;
}

void CTimer::Start()
{
	m_nGameStartTime=timeGetTime();
}

DWORD CTimer::GameTime()
{
	return timeGetTime()-m_nGameStartTime;
}

int CTimer::Elapsed(int &start, int interval)
{
	int current_time=GameTime();
	if(current_time>=start+interval)
	{
		start=current_time;
		return TRUE;
	}
	else return FALSE;
}

void CTimer::SetGameTime(DWORD NewGameTime)
{
	m_nGameStartTime+=GameTime()-NewGameTime;
}



