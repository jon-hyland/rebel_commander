#include "Stars.h"

CStars::CStars(int starcount)
{
	m_iStarCount=starcount;
	for(int i=0; i<starcount; i++)
	{
		m_iStarArray[i].X=float(Random.number(0,SCREEN_WIDTH-2));
		m_iStarArray[i].Y=float(Random.number(0,SCREEN_HEIGHT-2));
		m_iStarArray[i].VectorX=float(Random.number(-112,-7))/10;
		m_iStarArray[i].VectorY=0;
		switch(Random.number(0,13))
		{
			case 0:		m_iStarArray[i].Pal_Index=62;	break;
			case 1:		m_iStarArray[i].Pal_Index=85;	break;
			case 2:		m_iStarArray[i].Pal_Index=98;	break;
			case 3:		m_iStarArray[i].Pal_Index=150;	break;
			case 4:		m_iStarArray[i].Pal_Index=151;	break;
			case 5:		m_iStarArray[i].Pal_Index=152;	break;
			case 6:		m_iStarArray[i].Pal_Index=153;	break;
			case 7:		m_iStarArray[i].Pal_Index=156;	break;
			case 8:		m_iStarArray[i].Pal_Index=177;	break;
			case 9:		m_iStarArray[i].Pal_Index=242;	break;
			case 10:	m_iStarArray[i].Pal_Index=243;	break;
			case 11:	m_iStarArray[i].Pal_Index=244;	break;
			case 12:	m_iStarArray[i].Pal_Index=245;	break;
			case 13:	m_iStarArray[i].Pal_Index=246;	break;
		}
		
		if(m_iStarArray[i].VectorX>-5)
			m_iStarArray[i].Star_Type=SMALL_STAR;
		else if(m_iStarArray[i].VectorX>-7)
			switch(Random.number(0,2))
			{
				case 0: case 1:	m_iStarArray[i].Star_Type=SMALL_STAR; break;
				case 2: m_iStarArray[i].Star_Type=LARGE_STAR; break;
			}
		else
			m_iStarArray[i].Star_Type=LARGE_STAR;
	}
}

CStars::~CStars()
{
	for(int i=0; i<m_iStarCount; i++)
	{
		//delete m_nStarArray[i]; ??
	}

}

void CStars::Move()
{
	for(int i=0; i<m_iStarCount; i++)
	{
		m_iStarArray[i].X+=m_iStarArray[i].VectorX;
		if(m_iStarArray[i].X >= SCREEN_WIDTH) m_iStarArray[i].X=0;
		if(m_iStarArray[i].X < 0) m_iStarArray[i].X=SCREEN_WIDTH-1;
		m_iStarArray[i].Y+=m_iStarArray[i].VectorY;
		if(m_iStarArray[i].Y >= SCREEN_HEIGHT) m_iStarArray[i].Y=0;
		if(m_iStarArray[i].Y < 0) m_iStarArray[i].Y=SCREEN_HEIGHT-1;
	}
}

void CStars::Draw(DDSURFACEDESC *ddsd)
{
	for(int i=0; i<m_iStarCount; i++)
	{
		switch(m_iStarArray[i].Star_Type)
		{
			case SMALL_STAR:
				PutPx(int(m_iStarArray[i].X+SCREEN_OFFSET), int(m_iStarArray[i].Y), m_iStarArray[i].Pal_Index, ddsd);
				break;
			case LARGE_STAR:
				PutPx(int(m_iStarArray[i].X+SCREEN_OFFSET), int(m_iStarArray[i].Y), m_iStarArray[i].Pal_Index, ddsd);
				PutPx(int(m_iStarArray[i].X+SCREEN_OFFSET+1), int(m_iStarArray[i].Y), m_iStarArray[i].Pal_Index, ddsd);
				PutPx(int(m_iStarArray[i].X+SCREEN_OFFSET), int(m_iStarArray[i].Y+1), m_iStarArray[i].Pal_Index, ddsd);
				PutPx(int(m_iStarArray[i].X+SCREEN_OFFSET+1), int(m_iStarArray[i].Y+1), m_iStarArray[i].Pal_Index, ddsd);
				break;
		}
	}
}
