#include "fireman.h"

CFireManager::CFireManager()
{
	m_nCount=0;
	FireList=new CFire*[MAX_FIRE_INSTANCES];
	for(int i=0; i<MAX_FIRE_INSTANCES; i++)
		FireList[i]=NULL;
}

CFireManager::~CFireManager()
{
	for(int i=0; i<MAX_FIRE_INSTANCES; i++)
		delete FireList[i];
	delete[]FireList;
}

int CFireManager::Create(FireType firetype, int orig_x, int orig_y, int target_x, int target_y, float vector_speed)
{
	if(m_nCount<MAX_FIRE_INSTANCES)
	{
		int i=0;
		while(FireList[i]!=NULL)i++;	//find 1st free slot
		FireList[i]=new CFire(firetype, orig_x, orig_y, target_x, target_y, vector_speed);
		m_nCount++;
		return i;
	}
	else
		return -1;
}

void CFireManager::MoveAll()
{
	//delete any dead fire objects
	for(int i=0; i<MAX_FIRE_INSTANCES; i++)
		if(FireList[i]!=NULL)
			if(FireList[i]->m_bDead)
			{
				m_nCount--;
				delete FireList[i];
				FireList[i]=NULL;
			}
	
	//move each fire object
	for(i=0; i<MAX_FIRE_INSTANCES; i++)
		if(FireList[i]!=NULL)
			FireList[i]->Move();
}

void CFireManager::DrawAllBlit(LPDIRECTDRAWSURFACE surface)
{
	for(int i=0; i<MAX_FIRE_INSTANCES; i++) 
		if(FireList[i]!=NULL)
			switch(FireList[i]->GetFireType())
			{
				case ENEMY_FIRE_ONE:
					FireList[i]->DrawBlit(surface);
					break;
			}
}

void CFireManager::DrawAllPlot(DDSURFACEDESC *ddsd)
{
	for(int i=0; i<MAX_FIRE_INSTANCES; i++) 
		if(FireList[i]!=NULL)
			switch(FireList[i]->GetFireType())
			{
				case SHIP_LASER_ONE:
					FireList[i]->DrawPlot(ddsd);
					break;
			}
}

//resets/destroys all fire objects in fire manager
void CFireManager::Reset()
{
	//delete fire object array
	for(int i=0; i<MAX_FIRE_INSTANCES; i++)
		delete FireList[i];
	delete[]FireList;

	//recreate fire object array
	m_nCount=0;
	FireList=new CFire*[MAX_FIRE_INSTANCES];
	for(i=0; i<MAX_FIRE_INSTANCES; i++)
		FireList[i]=NULL;
}

int CFireManager::GetCount() { return m_nCount; }
