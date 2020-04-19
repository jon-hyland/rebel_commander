#ifndef __FILEOUT_H__
#define __FILEOUT_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class CFileOut
{
	private:
		FILE *m_pfOUT;
	public:
		CFileOut();
		void WriteLine(char *pszLine);
};

#endif