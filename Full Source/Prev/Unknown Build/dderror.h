#ifndef __DDERROR_H__
#define __DDERROR_H__

#include <windows.h>
#include <ddraw.h>

class CDDError
{
	private:
	public:
		CDDError();
		~CDDError();
		char* GetDDErrorString(HRESULT hr);
};

#endif