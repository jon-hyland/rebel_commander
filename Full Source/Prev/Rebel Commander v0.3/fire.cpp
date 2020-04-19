#include "fire.h"

extern CClippedSprite *g_pSprite[];
extern CTimer Timer;
extern CRandom Random;
extern CViewPoint Viewpoint;

CFire::CFire(FireType firetype, int orig_x, int orig_y, int target_x, int target_y)
{
	m_enFireType=firetype;
	m_nOriginX=m_nX=orig_x;
	m_nOriginY=m_nY=orig_y;
	m_nTargetX=target_x;
	m_nTargetY=target_y;
	
	m_nLastXMoveTime=m_nLastYMoveTime=Timer.time();
	m_bDead=FALSE;
	m_bShipFire=FALSE;
	m_nVectorSpeed=0;
	m_nVectorX=m_nVectorY=0;
	m_pSprite=NULL;

	switch(m_enFireType)
	{
		case SHIP_LASER_1:
			m_bShipFire=TRUE;
			m_nVectorSpeed=20;
			AlterVector(m_nVectorX, m_nVectorY, m_nX, m_nY, m_nTargetX, m_nTargetY, m_nVectorSpeed);
			break;
	}
}

void CFire::AlterVector(float &vectorx, float &vectory, int current_x, int current_y, int target_x, int target_y, float vector_speed)
{
	float vector_length;
	static float vector_remainder;

	vectorx=float(target_x-current_x);
	vectory=float(target_y-current_y);
	vector_length=float(sqrt(vectorx*vectorx+vectory*vectory));
	if(vector_length<vector_speed)
	{
		vectorx=(vectorx/vector_speed)*vector_speed+vector_remainder;
		vectory=(vectory/vector_speed)*vector_speed+vector_remainder;
		vector_remainder=0;//vector_speed-vector_length;
	}
	else
	{
		vectorx=(vectorx/vector_length)*vector_speed+vector_remainder;
		vectory=(vectory/vector_length)*vector_speed+vector_remainder;
		vector_remainder=0;
	}
}

BOOL CFire::ReachedDestination(int dest_x, int dest_y, int current_x, int current_y, int orig_x, int orig_y)
{
	if(current_x==dest_x&&current_y==dest_y)
		return TRUE;
	return FALSE;
}

void CFire::Move()
{
	const int XSCALE=16;
	const int YSCALE=16;
	
	int xdelta, ydelta;
	int time=Timer.time();
	int tfactor;
	
	switch(m_enFireType)
	{
		//every fire type moves differently, could have different death triggers,
		//and may alter vector as needed
		case SHIP_LASER_1:	//a simple one
			//if(ReachedDestination(m_nTargetX, m_nTargetY, m_nX, m_nY, m_nOriginX, m_nOriginY))
			//	m_bDead=TRUE;
			if(m_nX>m_nTargetX)
				m_bDead=TRUE;

			if(!m_bDead)
			{
				tfactor=time-m_nLastXMoveTime;
				xdelta=int((m_nVectorX*tfactor)/XSCALE);
				m_nX+=xdelta;
				if(xdelta||m_nVectorX==0)
					m_nLastXMoveTime=time;
			}
			break;
	}
}

void CFire::Draw(DDSURFACEDESC *ddsd)
{
	int t;		//time factor for blit animation frames
	int x, y;	//increment vars for plotting
	int x_offset, y_offset;
	LPDIRECTDRAWSURFACE surface;	//surface referenced in ddsd

	//follow pattern of draw then animate

	//every fire type draws differently, could be blitted bitmap or direct plot
	//draw function will assume a locked surface but will not enforce

	switch(m_enFireType)
	{
		case SHIP_LASER_1:	//draw laser 1 (length=16, width=4)
			x_offset=m_nX+36;
			y_offset=m_nY+4;
			for(x=0; x<16; x++)
			{
				switch(x)
				{
					case 0: case 1:
						PutPx(x+x_offset, 0+y_offset, 177, ddsd);
						PutPx(x+x_offset, 1+y_offset, 177, ddsd);
						PutPx(x+x_offset, 2+y_offset, 98, ddsd);
						PutPx(x+x_offset, 3+y_offset, 98, ddsd);
						break;
					case 4: case 5:
						PutPx(x+x_offset, 0+y_offset, 156, ddsd);
						PutPx(x+x_offset, 1+y_offset, 156, ddsd);
						PutPx(x+x_offset, 2+y_offset, 177, ddsd);
						PutPx(x+x_offset, 3+y_offset, 177, ddsd);
						break;
					case 6: case 7:
						PutPx(x+x_offset, 0+y_offset, 98, ddsd);
						PutPx(x+x_offset, 1+y_offset, 98, ddsd);
						break;
					case 8: case 9:
						PutPx(x+x_offset, 0+y_offset, 62, ddsd);
						PutPx(x+x_offset, 1+y_offset, 62, ddsd);
						PutPx(x+x_offset, 2+y_offset, 156, ddsd);
						PutPx(x+x_offset, 3+y_offset, 156, ddsd);
						break;
					case 10: case 11:
						PutPx(x+x_offset, 0+y_offset, 85, ddsd);
						PutPx(x+x_offset, 1+y_offset, 85, ddsd);
						PutPx(x+x_offset, 2+y_offset, 156, ddsd);
						PutPx(x+x_offset, 3+y_offset, 156, ddsd);
						break;
					case 12: case 13:
						PutPx(x+x_offset, 0+y_offset, 85, ddsd);
						PutPx(x+x_offset, 1+y_offset, 85, ddsd);
						PutPx(x+x_offset, 2+y_offset, 62, ddsd);
						PutPx(x+x_offset, 3+y_offset, 62, ddsd);
						break;
					case 14: case 15:
						PutPx(x+x_offset, 0+y_offset, 156, ddsd);
						PutPx(x+x_offset, 1+y_offset, 156, ddsd);
						PutPx(x+x_offset, 2+y_offset, 156, ddsd);
						PutPx(x+x_offset, 3+y_offset, 156, ddsd);
						break;
				}
			}
			
			break;
	}
}