#include "sound.h"

CSoundManager::CSoundManager(HWND hwnd)
{
	m_nCount=0;
	for(int i=0; i<MAX_SOUNDS; i++)
	{
		m_lpBuffer[i]=NULL; m_nCopyCount[i]=0;
	}

	m_bOperational=SUCCEEDED(DirectSoundCreate(NULL,&m_lpDirectSound,NULL));
	if(!m_bOperational)
		return;
	m_bOperational=SUCCEEDED(m_lpDirectSound->SetCooperativeLevel(hwnd,DSSCL_NORMAL));

	m_bSoundOn=m_bOperational;
}

CSoundManager::~CSoundManager()
{
	if(!m_bOperational)
		return;
	Clear();
	(void)m_lpDirectSound->Release();
}

void CSoundManager::Clear()
{
	if(!m_bOperational)
		return;
	Stop();
	for(int i=0; i<m_nCount; i++)
	{
		for(int j=0; j<m_nCopyCount[i]; j++)
		{
			m_lpBuffer[i][j]->Release();
			m_lpBuffer[i][j]=NULL;
		}
	    delete[]m_lpBuffer[i];
	}
	m_nCount=0;
}

void CSoundManager::Load(char *filename,int copies)
{
	int length;
	BYTE *sound=NULL;

	if(!m_bOperational)
		return;
	if(m_nCount>=MAX_SOUNDS)
		return;
	length=LoadSound(filename,&sound);
	CreateBuffer(m_nCount,length,copies);
	LoadBuffer(m_nCount,sound,length);
	CopyBuffer(m_nCount,copies);
	delete[]sound;
	m_nCount++;
}

void CSoundManager::Play(int index,BOOL looping)
{
	if((!m_bOperational)||(!m_bSoundOn))
		return;
	if(index<0||index>=m_nCount)
		return;
	int copy=0;
	DWORD status;

	if(FAILED(m_lpBuffer[index][copy]->GetStatus(&status)))
    status=DSBSTATUS_PLAYING;
	while(copy<m_nCopyCount[index]&&(status&DSBSTATUS_PLAYING))
	{
		if(++copy<m_nCopyCount[index])
			if(FAILED(m_lpBuffer[index][copy]->GetStatus(&status)))
				status=DSBSTATUS_PLAYING;
	}
	if(copy<m_nCopyCount[index])
		m_lpBuffer[index][copy]->Play(0,0,looping?DSBPLAY_LOOPING:0);
}

void CSoundManager::Stop(int index)
{
	if(!m_bOperational)
		return;
	if(index<0||index>=m_nCount)
		return;
	for(int i=0; i<m_nCopyCount[index]; i++)
	{
		m_lpBuffer[index][i]->Stop();
		m_lpBuffer[index][i]->SetCurrentPosition(0);
	}
}

void CSoundManager::Stop(void)
{ 
	if(!m_bOperational)
		return;
	for(int index=0; index<m_nCount; index++)
		Stop(index);
}

void CSoundManager::TurnOn()
{
	m_bSoundOn=TRUE;
}

void CSoundManager::TurnOff()
{
	Stop(); 
	m_bSoundOn=FALSE;
}

BOOL CSoundManager::CopyBuffer(int index,int copies)
{ 
	if(!m_bOperational)
		return FALSE;
	BOOL result=TRUE;
	m_nCopyCount[index]=copies;
	for(int i=1; i<copies; i++)
		result=result&&SUCCEEDED(m_lpDirectSound->DuplicateSoundBuffer(*m_lpBuffer[index],&(m_lpBuffer[index][i])));
	return result;
}

BOOL CSoundManager::CreateBuffer(int index,int length,int copies)
{
	if(!m_bOperational)
		return FALSE;
	DSBUFFERDESC dsbdesc;
	PCMWAVEFORMAT pcmwf;
	if(length<=0)
		return FALSE;
	
	memset(&pcmwf,0,sizeof(PCMWAVEFORMAT));
	pcmwf.wf.wFormatTag=WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels=DS_CHANSPERSAMPLE;
	pcmwf.wf.nSamplesPerSec=DS_SAMPLERATE;
	pcmwf.wf.nBlockAlign=DS_CHANSPERSAMPLE*DS_BITSPERSAMPLE/8;
	pcmwf.wf.nAvgBytesPerSec=pcmwf.wf.nSamplesPerSec*pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample=DS_BITSPERSAMPLE;

	memset(&dsbdesc,0,sizeof(DSBUFFERDESC));
	dsbdesc.dwSize=sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags=DSBCAPS_STATIC|DSBCAPS_CTRLPOSITIONNOTIFY|DSBCAPS_GETCURRENTPOSITION2;
	dsbdesc.dwBufferBytes=length;
	dsbdesc.lpwfxFormat=(LPWAVEFORMATEX)&pcmwf;

	m_lpBuffer[index]=new LPDIRECTSOUNDBUFFER[copies];
	for(int i=0; i<copies; i++)
		m_lpBuffer[index][i]=NULL;
	return SUCCEEDED(m_lpDirectSound->CreateSoundBuffer(&dsbdesc,m_lpBuffer[index],NULL));
}

BOOL CSoundManager::LoadBuffer(int index,BYTE *sound,int length)
{
	if(!m_bOperational)
		return FALSE;
	LPVOID w1,w2; 
	DWORD l1,l2; 
	LPDIRECTSOUNDBUFFER buffer=*m_lpBuffer[index];
	if(length<=0)
		return FALSE;
	if(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0)==DSERR_BUFFERLOST)
	{
		buffer->Restore(); 
		if(FAILED(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0)))
			return FALSE; 
	}
	CopyMemory(w1,sound,l1); 
	if(w2!=NULL)CopyMemory(w2,sound+l1,l2);
	if(FAILED(buffer->Unlock(w1,l1,w2,l2)))
		return FALSE;
	return TRUE;
}

int CSoundManager::LoadSound(char *filename,BYTE **buffer)
{
	if(!m_bOperational)
		return 0;
	HMMIO hmfr;
	MMCKINFO parent,child;
	WAVEFORMATEX wfmtx;
	int size=0;
	  delete[]*buffer;
	parent.ckid=(FOURCC)0;
	parent.cksize=0;
	parent.fccType=(FOURCC)0;
	parent.dwDataOffset=0;
	parent.dwFlags=0;
	child=parent;
	hmfr=mmioOpen(filename,NULL,MMIO_READ|MMIO_ALLOCBUF);
	if(hmfr==NULL)
		return NULL;
	parent.fccType=mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmfr,&parent,NULL,MMIO_FINDRIFF))
	{
		mmioClose(hmfr,0); 
		return NULL;
	}

	child.ckid=mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmfr,&child,&parent,0))
	{
		mmioClose(hmfr,0); return NULL;
	}

	if(mmioRead(hmfr,(char*)&wfmtx,sizeof(wfmtx))!=sizeof(wfmtx))
	{
		mmioClose(hmfr,0); 
		return NULL;
	}

	if(wfmtx.wFormatTag!=WAVE_FORMAT_PCM)
	{
		mmioClose(hmfr,0); 
		return NULL;
	}

	if(mmioAscend(hmfr,&child,0))
	{
		mmioClose(hmfr,0); 
		return NULL;
	}

	child.ckid=mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmfr,&child,&parent,MMIO_FINDCHUNK))
	{
		mmioClose(hmfr,0); 
		return NULL;
	}

	size=child.cksize;
	*buffer=new BYTE[size];
	if(*buffer==NULL)
	{
		mmioClose(hmfr,0); 
		return NULL;
	}

	if((int)mmioRead(hmfr,(char *)*buffer,size)!=size)
	{
		mmioClose(hmfr,0); 
		delete[]*buffer; 
		return NULL; 
	}

	mmioClose(hmfr,0);
	return size;
}


BOOL CSoundManager::Operational() { return m_bOperational; }
BOOL CSoundManager::SoundOn() { return m_bSoundOn; }