#include <windows.h>

#ifndef __RANDOM_H__
#define __RANDOM_H__

#define STALE_RAND 1000

class CRandom
{
	private:
		int m_nCount;
	public:
		CRandom();
		int number(int i, int j);
		void sowseed();
};

#endif