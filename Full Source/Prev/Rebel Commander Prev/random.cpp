#include "random.h"

CRandom::CRandom()
{
	sowseed();
}

void CRandom::sowseed()
{
	srand(timeGetTime());
	m_nCount=0;
}

int CRandom::number(int i, int j)
{
	if(++m_nCount>STALE_RAND) sowseed();
	int sample;
	if(j<0x7FFF)sample=rand();	// 15-bit
	else sample=((int)rand()<<15)|(int)rand(); //30-bit
	return sample%(j-i+1)+i;
}