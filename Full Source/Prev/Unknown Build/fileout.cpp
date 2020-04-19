#include "fileout.h"

//constructor
CFileOut::CFileOut()
{
}

//write a line to the log file
void CFileOut::WriteLine(char* pszLine)
{
	//open file for appending, or create new
	m_pfOUT = fopen("output.txt", "a");

	//return if file can't be opened
	if (m_pfOUT == NULL)
		return;

	//write to file
	fputs(pszLine, m_pfOUT);
	fputs("\n", m_pfOUT);

	//close file
	fclose(m_pfOUT);
}
