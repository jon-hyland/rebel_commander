#include "objman.h"
#include "view.h"

extern CViewPoint Viewpoint;

CObjectManager::CObjectManager(int max)
{
	m_nMaxCount=max;
	m_nCount=0;
	m_nCurrentObject=0;
	m_pObjectList=new CObject*[max];
	for(int i=0; i<m_nMaxCount; i++)
		m_pObjectList[i]=new CObject;
}

CObjectManager::~CObjectManager()
{
	for(int i=0; i<m_nMaxCount; i++)
		delete m_pObjectList[i];
	delete[]m_pObjectList;
}

int CObjectManager::create(ObjectType object, int x, int y, int xspeed, int yspeed)
{
	if(m_nCount<m_nMaxCount)
	{
		m_pObjectList[m_nCount]->create(object,x,y,xspeed,yspeed);
		return m_nCount++;
	}
	else return -1;
}

void CObjectManager::animate(LPDIRECTDRAWSURFACE surface)
{
	for(int i=0; i<m_nCount; i++)m_pObjectList[i]->move();

	//Viewpoint.set_position(m_pObjectList[m_nCurrentObject]->m_nX);
	
	for(i=0; i<m_nCount; i++)m_pObjectList[i]->draw(surface);
}

void CObjectManager::accelerate(int xdelta, int ydelta)
{
	m_pObjectList[m_nCurrentObject]->accelerate(xdelta, ydelta);
}

void CObjectManager::set_current(int index)
{
	if(index>=0&&index<m_nCount)m_nCurrentObject=index;
}

int CObjectManager::speed()
{
	return abs(m_pObjectList[m_nCurrentObject]->m_nXspeed);
}
