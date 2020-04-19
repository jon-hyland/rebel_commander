#include "alpha.h"

extern CClippedSprite *g_pSprite[];
extern CViewPoint Viewpoint;

CAlpha::CAlpha()
{
}

CAlpha::~CAlpha()
{
}

void CAlpha::Write(LPDIRECTDRAWSURFACE surface, int x, int y,  char *text, SpriteType sprite_type)
{
	int char_count;
	int char_width, char_height;
	int blit_x;
	
	m_pAlphaSprite=g_pSprite[sprite_type];
	char_count=int(strlen(text));
	char_width=m_pAlphaSprite->GetWidth();
	char_height=m_pAlphaSprite->GetHeight();
	for(int i=0; i<char_count; i++)
	{
		blit_x=int(x-((char_width*(char_count-1))/2)+(i*char_width));
		switch(text[i])
		{
			case 'A': case 'a': m_pAlphaSprite->draw(0, Viewpoint.screen(blit_x), y, surface); break;
			case 'B': case 'b': m_pAlphaSprite->draw(1, Viewpoint.screen(blit_x), y, surface); break;
			case 'C': case 'c': m_pAlphaSprite->draw(2, Viewpoint.screen(blit_x), y, surface); break;
			case 'D': case 'd': m_pAlphaSprite->draw(3, Viewpoint.screen(blit_x), y, surface); break;
			case 'E': case 'e': m_pAlphaSprite->draw(4, Viewpoint.screen(blit_x), y, surface); break;
			case 'F': case 'f': m_pAlphaSprite->draw(5, Viewpoint.screen(blit_x), y, surface); break;
			case 'G': case 'g': m_pAlphaSprite->draw(6, Viewpoint.screen(blit_x), y, surface); break;
			case 'H': case 'h': m_pAlphaSprite->draw(7, Viewpoint.screen(blit_x), y, surface); break;
			case 'I': case 'i': m_pAlphaSprite->draw(8, Viewpoint.screen(blit_x), y, surface); break;
			case 'J': case 'j': m_pAlphaSprite->draw(9, Viewpoint.screen(blit_x), y, surface); break;
			case 'K': case 'k': m_pAlphaSprite->draw(10, Viewpoint.screen(blit_x), y, surface); break;
			case 'L': case 'l': m_pAlphaSprite->draw(11, Viewpoint.screen(blit_x), y, surface); break;
			case 'M': case 'm': m_pAlphaSprite->draw(12, Viewpoint.screen(blit_x), y, surface); break;
			case 'N': case 'n': m_pAlphaSprite->draw(13, Viewpoint.screen(blit_x), y, surface); break;
			case 'O': case 'o': m_pAlphaSprite->draw(14, Viewpoint.screen(blit_x), y, surface); break;
			case 'P': case 'p': m_pAlphaSprite->draw(15, Viewpoint.screen(blit_x), y, surface); break;
			case 'Q': case 'q': m_pAlphaSprite->draw(16, Viewpoint.screen(blit_x), y, surface); break;
			case 'R': case 'r': m_pAlphaSprite->draw(17, Viewpoint.screen(blit_x), y, surface); break;
			case 'S': case 's': m_pAlphaSprite->draw(18, Viewpoint.screen(blit_x), y, surface); break;
			case 'T': case 't': m_pAlphaSprite->draw(19, Viewpoint.screen(blit_x), y, surface); break;
			case 'U': case 'u': m_pAlphaSprite->draw(20, Viewpoint.screen(blit_x), y, surface); break;
			case 'V': case 'v': m_pAlphaSprite->draw(21, Viewpoint.screen(blit_x), y, surface); break;
			case 'W': case 'w': m_pAlphaSprite->draw(22, Viewpoint.screen(blit_x), y, surface); break;
			case 'X': case 'x': m_pAlphaSprite->draw(23, Viewpoint.screen(blit_x), y, surface); break;
			case 'Y': case 'y': m_pAlphaSprite->draw(24, Viewpoint.screen(blit_x), y, surface); break;
			case 'Z': case 'z': m_pAlphaSprite->draw(25, Viewpoint.screen(blit_x), y, surface); break;
			case '0': m_pAlphaSprite->draw(26, Viewpoint.screen(blit_x), y, surface); break;
			case '1': m_pAlphaSprite->draw(27, Viewpoint.screen(blit_x), y, surface); break;
			case '2': m_pAlphaSprite->draw(28, Viewpoint.screen(blit_x), y, surface); break;
			case '3': m_pAlphaSprite->draw(29, Viewpoint.screen(blit_x), y, surface); break;
			case '4': m_pAlphaSprite->draw(30, Viewpoint.screen(blit_x), y, surface); break;
			case '5': m_pAlphaSprite->draw(31, Viewpoint.screen(blit_x), y, surface); break;
			case '6': m_pAlphaSprite->draw(32, Viewpoint.screen(blit_x), y, surface); break;
			case '7': m_pAlphaSprite->draw(33, Viewpoint.screen(blit_x), y, surface); break;
			case '8': m_pAlphaSprite->draw(34, Viewpoint.screen(blit_x), y, surface); break;
			case '9': m_pAlphaSprite->draw(35, Viewpoint.screen(blit_x), y, surface); break;
			case '@': m_pAlphaSprite->draw(36, Viewpoint.screen(blit_x), y, surface); break;
			case '.': m_pAlphaSprite->draw(37, Viewpoint.screen(blit_x), y, surface); break;
			case ',': m_pAlphaSprite->draw(38, Viewpoint.screen(blit_x), y, surface); break;
			case '-': m_pAlphaSprite->draw(39, Viewpoint.screen(blit_x), y, surface); break;
		}
	}
}

void CAlpha::Write(LPDIRECTDRAWSURFACE surface, int x, int y,  int number, SpriteType sprite_type)
{
	char text[16];
	int char_count;
	int char_width, char_height;
	int blit_x;

	sprintf (text, "%i\n", number);
	m_pAlphaSprite=g_pSprite[sprite_type];
	char_count=int(strlen(text));
	char_width=m_pAlphaSprite->GetWidth();
	char_height=m_pAlphaSprite->GetHeight();
	for(int i=0; i<char_count; i++)
	{
		blit_x=int(x-((char_width*(char_count-1))/2)+(i*char_width));
		switch(text[i])
		{
			case 'A': case 'a': m_pAlphaSprite->draw(0, Viewpoint.screen(blit_x), y, surface); break;
			case 'B': case 'b': m_pAlphaSprite->draw(1, Viewpoint.screen(blit_x), y, surface); break;
			case 'C': case 'c': m_pAlphaSprite->draw(2, Viewpoint.screen(blit_x), y, surface); break;
			case 'D': case 'd': m_pAlphaSprite->draw(3, Viewpoint.screen(blit_x), y, surface); break;
			case 'E': case 'e': m_pAlphaSprite->draw(4, Viewpoint.screen(blit_x), y, surface); break;
			case 'F': case 'f': m_pAlphaSprite->draw(5, Viewpoint.screen(blit_x), y, surface); break;
			case 'G': case 'g': m_pAlphaSprite->draw(6, Viewpoint.screen(blit_x), y, surface); break;
			case 'H': case 'h': m_pAlphaSprite->draw(7, Viewpoint.screen(blit_x), y, surface); break;
			case 'I': case 'i': m_pAlphaSprite->draw(8, Viewpoint.screen(blit_x), y, surface); break;
			case 'J': case 'j': m_pAlphaSprite->draw(9, Viewpoint.screen(blit_x), y, surface); break;
			case 'K': case 'k': m_pAlphaSprite->draw(10, Viewpoint.screen(blit_x), y, surface); break;
			case 'L': case 'l': m_pAlphaSprite->draw(11, Viewpoint.screen(blit_x), y, surface); break;
			case 'M': case 'm': m_pAlphaSprite->draw(12, Viewpoint.screen(blit_x), y, surface); break;
			case 'N': case 'n': m_pAlphaSprite->draw(13, Viewpoint.screen(blit_x), y, surface); break;
			case 'O': case 'o': m_pAlphaSprite->draw(14, Viewpoint.screen(blit_x), y, surface); break;
			case 'P': case 'p': m_pAlphaSprite->draw(15, Viewpoint.screen(blit_x), y, surface); break;
			case 'Q': case 'q': m_pAlphaSprite->draw(16, Viewpoint.screen(blit_x), y, surface); break;
			case 'R': case 'r': m_pAlphaSprite->draw(17, Viewpoint.screen(blit_x), y, surface); break;
			case 'S': case 's': m_pAlphaSprite->draw(18, Viewpoint.screen(blit_x), y, surface); break;
			case 'T': case 't': m_pAlphaSprite->draw(19, Viewpoint.screen(blit_x), y, surface); break;
			case 'U': case 'u': m_pAlphaSprite->draw(20, Viewpoint.screen(blit_x), y, surface); break;
			case 'V': case 'v': m_pAlphaSprite->draw(21, Viewpoint.screen(blit_x), y, surface); break;
			case 'W': case 'w': m_pAlphaSprite->draw(22, Viewpoint.screen(blit_x), y, surface); break;
			case 'X': case 'x': m_pAlphaSprite->draw(23, Viewpoint.screen(blit_x), y, surface); break;
			case 'Y': case 'y': m_pAlphaSprite->draw(24, Viewpoint.screen(blit_x), y, surface); break;
			case 'Z': case 'z': m_pAlphaSprite->draw(25, Viewpoint.screen(blit_x), y, surface); break;
			case '0': m_pAlphaSprite->draw(26, Viewpoint.screen(blit_x), y, surface); break;
			case '1': m_pAlphaSprite->draw(27, Viewpoint.screen(blit_x), y, surface); break;
			case '2': m_pAlphaSprite->draw(28, Viewpoint.screen(blit_x), y, surface); break;
			case '3': m_pAlphaSprite->draw(29, Viewpoint.screen(blit_x), y, surface); break;
			case '4': m_pAlphaSprite->draw(30, Viewpoint.screen(blit_x), y, surface); break;
			case '5': m_pAlphaSprite->draw(31, Viewpoint.screen(blit_x), y, surface); break;
			case '6': m_pAlphaSprite->draw(32, Viewpoint.screen(blit_x), y, surface); break;
			case '7': m_pAlphaSprite->draw(33, Viewpoint.screen(blit_x), y, surface); break;
			case '8': m_pAlphaSprite->draw(34, Viewpoint.screen(blit_x), y, surface); break;
			case '9': m_pAlphaSprite->draw(35, Viewpoint.screen(blit_x), y, surface); break;
			case '@': m_pAlphaSprite->draw(36, Viewpoint.screen(blit_x), y, surface); break;
			case '.': m_pAlphaSprite->draw(37, Viewpoint.screen(blit_x), y, surface); break;
			case ',': m_pAlphaSprite->draw(38, Viewpoint.screen(blit_x), y, surface); break;
			case '-': m_pAlphaSprite->draw(39, Viewpoint.screen(blit_x), y, surface); break;
		}
	}
}
