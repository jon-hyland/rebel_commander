#include "stars.h"

CStars::CStars(int starcount)
{
	m_iStarCount=starcount;
	for(int i=0; i<starcount; i++)
	{
		m_iStarArray[i].x=Random.number(0,SCREEN_WIDTH-2);
		m_iStarArray[i].y=Random.number(0,SCREEN_HEIGHT-2);
		m_iStarArray[i].x_speed=Random.number(-11,-1);
		m_iStarArray[i].y_speed=0;
		m_iStarArray[i].pal_index=Random.number(0,255);
		m_iStarArray[i].startype=Random.number(0,1);
	}
}

CStars::~CStars()
{
	for(int i=0; i<m_iStarCount; i++)
	{
		//delete m_nStarArray[i]; ??
	}

}

void CStars::move()
{
	for(int i=0; i<m_iStarCount; i++)
	{
		m_iStarArray[i].x+=m_iStarArray[i].x_speed;
		if(m_iStarArray[i].x >= SCREEN_WIDTH) m_iStarArray[i].x=0;
		if(m_iStarArray[i].x < 0) m_iStarArray[i].x=SCREEN_WIDTH-1;
		m_iStarArray[i].y+=m_iStarArray[i].y_speed;
		if(m_iStarArray[i].y >= SCREEN_HEIGHT) m_iStarArray[i].y=0;
		if(m_iStarArray[i].y < 0) m_iStarArray[i].y=SCREEN_HEIGHT-1;
	}
}

void CStars::draw(DDSURFACEDESC *ddsd)
{
	for(int i=0; i<m_iStarCount; i++)
	{
		switch(m_iStarArray[i].startype)
		{
			case SMALL_STAR:
				PutPx(m_iStarArray[i].x, m_iStarArray[i].y, m_iStarArray[i].pal_index, ddsd);
				break;
			case LARGE_STAR:
				PutPx(m_iStarArray[i].x, m_iStarArray[i].y, m_iStarArray[i].pal_index, ddsd);
				PutPx(m_iStarArray[i].x+1, m_iStarArray[i].y, m_iStarArray[i].pal_index, ddsd);
				PutPx(m_iStarArray[i].x, m_iStarArray[i].y+1, m_iStarArray[i].pal_index, ddsd);
				PutPx(m_iStarArray[i].x+1, m_iStarArray[i].y+1, m_iStarArray[i].pal_index, ddsd);
				break;
		}
	}
}
