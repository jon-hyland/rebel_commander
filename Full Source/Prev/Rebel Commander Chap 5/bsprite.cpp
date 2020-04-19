#include <string.h>

#include "defines.h"
#include "bsprite.h"

extern LPDIRECTDRAW lpDirectDrawObject;

CBaseSprite::CBaseSprite(int frames, int width, int height)
{
	m_nWidth=width;
	m_nHeight=height;
	m_nFrameCount=frames;
	m_lpDDImage=new LPDIRECTDRAWSURFACE[frames];
	for(int i=0; i<frames; i++) 
		m_lpDDImage[i]=NULL;
	CreateSurfaces();
}

CBaseSprite::~CBaseSprite()
{
	delete[]m_lpDDImage;
}

void CBaseSprite::CreateSurfaces()
{
	DDSURFACEDESC ddsd;
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight=m_nHeight;
	ddsd.dwWidth=m_nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue=ddck.dwColorSpaceHighValue=TRANSPARENT_COLOR;

	for(int i=0; i<m_nFrameCount; i++)
	{
		if(FAILED(lpDirectDrawObject->CreateSurface(&ddsd,&(m_lpDDImage[i]),NULL)))
			m_lpDDImage[i]=NULL;
		m_lpDDImage[i]->SetColorKey(DDCKEY_SRCBLT,&ddck);
	}
}

void CBaseSprite::Release()
{
	for(int i=0; i<m_nFrameCount; i++)
		if(m_lpDDImage[i]!=NULL)
			m_lpDDImage[i]->Release();
}

BOOL CBaseSprite::load(CBmpSpriteFileReader *buffer, int frame, int x, int y)
{
	return buffer->draw(m_lpDDImage[frame],m_nWidth,m_nHeight,x,y);
}

void CBaseSprite::draw(int frame, int x, int y, LPDIRECTDRAWSURFACE dest)
{
	dest->BltFast(x-m_nWidth/2, y-m_nHeight, m_lpDDImage[frame], NULL, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
}

BOOL CBaseSprite::Restore()
{
	BOOL result=TRUE;
	for(int i=0; i<m_nFrameCount; i++)
		if(m_lpDDImage[i])
			result=result&&SUCCEEDED(m_lpDDImage[i]->Restore());
	return result;
}

int CBaseSprite::frame_count()
{
	return m_nFrameCount;
}

int CBaseSprite::height()
{
	return m_nHeight;
}

int CBaseSprite::width()
{
	return m_nWidth;
}

