#include "convert.h"

//passes char pointer text through function
char *int2char(int number, char *text)
{
	//for(int i=0; i<128; i++)
	//	text[i] = NULL;
	sprintf (text, "%i", number);
	return text;
}