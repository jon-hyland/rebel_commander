/*
	private:
		int m_nCount;
	public:
		CPowerUp **UpList;
		CPowerUpManager();
		~CPowerUpManager();
		int Create(ObjectType object, SpriteType object_sprite, SpriteType dying_sprite, int x, int y, ObjEventId obj_event_id);
		void DrawAll(LPDIRECTDRAWSURFACE surface);
		int GetCount();
*/

#include "pwrupman.h"

CPowerUpManager::CPowerUpManager()
{
	m_nCount=0;
	UpList=new CPowerUp*[MAX_UP_INSTANCES];
	for(int i=0; i<MAX_UP_INSTANCES; i++)
		UpList[i]=NULL;
}

CPowerUpManager::~CPowerUpManager()
{
	for(int i=0; i<MAX_UP_INSTANCES; i++)
		delete UpList[i];
	delete[]UpList;
}

int CPowerUpManager::Create(UpType up_type, int x, int y)
{
	if(m_nCount<MAX_UP_INSTANCES)
	{
		int i=0;
		while(UpList[i]!=NULL)i++;	//find first free slot
		UpList[i]=new CPowerUp();
		UpList[i]->Create(up_type, x, y);
		m_nCount++;
		return i;
	}
	else return -1;
}


void CPowerUpManager::DrawAll(LPDIRECTDRAWSURFACE surface)
{
	for(int i=0; i<MAX_UP_INSTANCES; i++) 
		if(UpList[i]!=NULL)
			UpList[i]->Draw(surface);
}

//resets/destroys all powerup objects in fire manager
void CPowerUpManager::Reset()
{
	//delete powerup object array
	for(int i=0; i<MAX_UP_INSTANCES; i++)
		delete UpList[i];
	delete[]UpList;

	//recreate powerup object array
	m_nCount=0;
	UpList=new CPowerUp*[MAX_UP_INSTANCES];
	for(i=0; i<MAX_UP_INSTANCES; i++)
		UpList[i]=NULL;
}


int CPowerUpManager::GetCount() { return m_nCount; }