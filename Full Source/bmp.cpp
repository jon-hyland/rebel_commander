#include "bmp.h"

CBmpFileReader::CBmpFileReader()
{
	m_cImage=0;
}

CBmpFileReader::~CBmpFileReader()
{
	delete[]m_cImage;
}


BOOL CBmpFileReader::load(char *filename)
{
	HANDLE hfile;
	DWORD actualRead;
	int image_size;
	BOOL OK=TRUE;
	
	hfile=CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);
	if(hfile==INVALID_HANDLE_VALUE)
		return FALSE;
	OK=ReadFile(hfile,&m_BMPFileHead,sizeof(m_BMPFileHead),&actualRead,NULL);
	if(OK)OK=ReadFile(hfile,&m_BMPFileInfo,sizeof(m_BMPFileInfo),&actualRead,NULL);\
	if(OK)OK=m_BMPFileInfo.biBitCount==COLOR_DEPTH;
	if(OK)OK=ReadFile(hfile,m_rgbPalette,sizeof(m_rgbPalette),&actualRead,NULL);
	if(!OK)
	{
		CloseHandle(hfile);
		return FALSE;
	}
	image_size=m_BMPFileInfo.biWidth*m_BMPFileInfo.biHeight;
	if(m_cImage)delete[]m_cImage;
	m_cImage=new BYTE[image_size];
	if(!m_cImage)
	{
		CloseHandle(hfile);
		return FALSE;
	}
	int width=(m_BMPFileInfo.biWidth+3)&~3;
	int i=0;
	BYTE trash[4];
	int remainder=width-m_BMPFileInfo.biWidth;
	while(OK&&i<m_BMPFileInfo.biHeight)
	{
		OK=OK&&ReadFile(hfile,(BYTE*)(m_cImage+i*m_BMPFileInfo.biWidth),m_BMPFileInfo.biWidth,&actualRead,NULL);
		OK=OK&&ReadFile(hfile,trash,remainder,&actualRead,NULL);
		i++;
	}
	if(!OK)delete[]m_cImage;
	CloseHandle(hfile);
	return OK;
}

BOOL CBmpFileReader::draw(LPDIRECTDRAWSURFACE surface)
{
	DDSURFACEDESC ddsd;
	BYTE *dest,*src;
	int src_width;

	memset(&ddsd,0,sizeof(DDSURFACEDESC));
	ddsd.dwSize=sizeof(ddsd);
	if(FAILED(surface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL)))
		return FALSE;
	dest=(BYTE*)ddsd.lpSurface;
	src=m_cImage+((m_BMPFileInfo.biHeight-1)*m_BMPFileInfo.biWidth);
	if(m_BMPFileInfo.biWidth>ddsd.lPitch)src_width=ddsd.lPitch;
	else src_width=m_BMPFileInfo.biWidth;
	for(int i=0; i<m_BMPFileInfo.biHeight; i++)
	{
		memcpy(dest,src,src_width);
		dest+=ddsd.lPitch;
		src-=src_width;
	}
	surface->Unlock(NULL);
	return TRUE;
}

BOOL CBmpFileReader::setpalette(LPDIRECTDRAWPALETTE palette)
{
	PALETTEENTRY pe[COLORS];

	for(int i=0; i<COLORS; i++)
	{
		pe[i].peRed=m_rgbPalette[i].rgbRed;
		pe[i].peGreen=m_rgbPalette[i].rgbGreen;
		pe[i].peBlue=m_rgbPalette[i].rgbBlue;
	}
	palette->SetEntries(NULL,0,COLORS,pe);
	return TRUE;
}

