
#include "GUI.H"

GUIWindow::GUIWindow(IDirectDraw4* lpDirectDraw4, IDirectDrawSurface4* lpBackBuffer, LPDIMOUSESTATE lpMouseState,
					 int left, int top, int right, int bottom, char* backgroundFile, char* cursorFile)
{
	//
	defaultCmd=NULL;
	//
	BITMAP_FILE bgBMPFile;
	memset(&bgBMPFile,0,sizeof(BITMAP_FILE));
	Load_Bitmap_File(&bgBMPFile,backgroundFile);
	//
	lpdd = lpDirectDraw4;
	lpddsBackBuffer = lpBackBuffer;
	lpdiMouseState=lpMouseState;
	this->left=left;
	this->top=top;
	this->right=right;
	this->bottom=bottom;
	int width=bgBMPFile.bitmapinfoheader.biWidth;
	int height=bgBMPFile.bitmapinfoheader.biHeight;
	lastx=(left+right)/2;
	lasty=(top+bottom)/2;
	// create lpddsBackground & cursor surfaces
	DDSURFACEDESC2 ddsd;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth  = width;
	ddsd.dwHeight = height;
	// set surface to offscreen plain (in sys mem)
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	// create the bg surface
	lpdd->CreateSurface(&ddsd,&lpddsBackground,NULL);
	//  explicit cursor width/height req
	ddsd.dwWidth  = CursorWidth;
	ddsd.dwHeight = CursorHeight;
	lpdd->CreateSurface(&ddsd,&cursor,NULL);
	// set color key to color 0
	DDCOLORKEY color_key; // used to set color key
	color_key.dwColorSpaceLowValue  = 0;
	color_key.dwColorSpaceHighValue = 0;
	// now set the color key for source blitting
	lpddsBackground->SetColorKey(DDCKEY_SRCBLT, &color_key);
	cursor->SetColorKey(DDCKEY_SRCBLT, &color_key);
	// set cursor bitmap
	BITMAP_FILE cursorBMPFile;
	memset(&cursorBMPFile,0,sizeof(BITMAP_FILE));
	Load_Bitmap_File(&cursorBMPFile,cursorFile);
	LoadSurfaceImage(cursor,CursorWidth,CursorHeight,&cursorBMPFile,0,0,BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&cursorBMPFile);
	// set back ground bitmap
	LoadSurfaceImage(lpddsBackground,width,height,
					&bgBMPFile,0,0,BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bgBMPFile);
	//create buttons list
	buttons=new LinkedList<GUIButton*>();
}

GUIWindow::~GUIWindow()
{
	//delete buttons
	while (buttons->length()>0) {
		delete buttons->pop();
	}
	delete buttons;
	//release background surface & cursor
	cursor->Release();
	lpddsBackground->Release();
}

void GUIWindow::update()
{
	int i;
	GUIButton* cmd;

	//update mouse pos
	lastx+=lpdiMouseState->lX;
	lasty+=lpdiMouseState->lY;
	if (lastx<left) lastx=left;
	else if (lastx>=right) lastx=right-1;
	if (lasty<top) lasty=top;
	else if (lasty>=bottom) lasty=bottom-1;
	//check for click on button
	int len=buttons->length();
	for (i=len; i>0; i--) {
		cmd=buttons->get(i);
		if (lpdiMouseState->rgbButtons[MouseLeftButton] & 0x80) {
			if ( lastx>cmd->left && lastx<cmd->right &&
				 lasty>cmd->top && lasty<cmd->bottom ) {
				 if (cmd->state==ButtonStateUnpressed) {
					cmd->state=ButtonStatePressed;
					break;
				 }
			}
		}
		else {
			if ( lastx>cmd->left && lastx<cmd->right &&
				 lasty>cmd->top && lasty<cmd->bottom ) {
				 if (cmd->state==ButtonStatePressed) {
					cmd->onClick();
					cmd->state=ButtonStateUnpressed;
					break;
				 }
			}
			else {
				cmd->state=ButtonStateUnpressed;
			}
		}
	}
}

void GUIWindow::draw()
{
	int i;
	char txt[50];//,txtfrag[50];
	RECT dest_rect;   // the destination rectangle

	//render background
	dest_rect.left   = left;
	dest_rect.top    = top;
	dest_rect.right  = right;
	dest_rect.bottom = bottom;
	lpddsBackBuffer->Blt(&dest_rect,lpddsBackground,NULL,(DDBLT_WAIT),NULL);
	//render any buttons (right now no check for buttons rendered outside of the window
	GUIButton* cmd;
	int len=buttons->length();
	for (i=1; i<=len; i++) {
		cmd=buttons->get(i);
		cmd->draw();
	}
	//draw default button hightlight if any button is set as default
	if (defaultCmd!=NULL) {
		int pitch;
		UCHAR* buffer=DDraw_Lock_Surface(lpddsBackBuffer,&pitch);
		Draw_Line(defaultCmd->left,defaultCmd->top,defaultCmd->left,defaultCmd->bottom-1,255,buffer,pitch);
		Draw_Line(defaultCmd->left,defaultCmd->bottom-1,defaultCmd->right-1,defaultCmd->bottom-1,255,buffer,pitch);
		Draw_Line(defaultCmd->right-1,defaultCmd->bottom-1,defaultCmd->right-1,defaultCmd->top,255,buffer,pitch);
		Draw_Line(defaultCmd->right-1,defaultCmd->top,defaultCmd->left,defaultCmd->top,255,buffer,pitch);
		DDraw_Unlock_Surface(lpddsBackBuffer);
	}
	//render mouse cursor
	dest_rect.left   = lastx;
	dest_rect.top    = lasty;
	dest_rect.right  = lastx+CursorWidth;
	dest_rect.bottom = lasty+CursorHeight;
	lpddsBackBuffer->Blt(&dest_rect,cursor,NULL,(DDBLT_WAIT | DDBLT_KEYSRC),NULL);
	/*//mouse pos
	strcpy(txt,"Position: ");
	strcat(txt,itoa(lastx,txtfrag,10));
	strcat(txt,", ");
	strcat(txt,itoa(lasty,txtfrag,10));*/
	//version
	strcpy(txt,"Version: ");
	strcat(txt,"1.0");
	Draw_Text_GDI(txt,0,0,255,lpddsBackBuffer);
}

void GUIWindow::addButton(int left, int top, int width, int height, int isDefault, void (*onClick)(void),
						  char* buttonUpImg, char* buttonDnImg)
{
	GUIButton * cmd;
	buttons->insert(cmd=(new GUIButton(left+this->left,top+this->top,left+width+this->left,top+height+this->top,onClick,
								  lpdd,lpddsBackBuffer,buttonUpImg,buttonDnImg))
					);
	if (isDefault)
		defaultCmd=cmd;
}

//////////////////////// GUIButton /////////////////////////////////////////////////////////////////

GUIButton::GUIButton(int left, int top, int right, int bottom,
					 void (*onClick)(void),
					 IDirectDraw4* lpdd, IDirectDrawSurface4* lpddsBackBuffer,
					 char* buttonUpImg, char* buttonDnImg)
{
	// Both button face BMPs must have the same dimensions
	//  load up bmp
	BITMAP_FILE bmpFileUp;
	memset(&bmpFileUp,0,sizeof(BITMAP_FILE));
	Load_Bitmap_File(&bmpFileUp,buttonUpImg);
	//  load down bmp
	BITMAP_FILE bmpFileDn;
	memset(&bmpFileDn,0,sizeof(BITMAP_FILE));
	Load_Bitmap_File(&bmpFileDn,buttonDnImg);
	//
	int width=bmpFileUp.bitmapinfoheader.biWidth;
	int height=bmpFileUp.bitmapinfoheader.biHeight;
	this->left=left;
	this->top=top;
	this->right=right;
	this->bottom=bottom;
	this->onClick=onClick;
	this->lpdd=lpdd;
	this->lpddsBackBuffer=lpddsBackBuffer;
	this->state=ButtonStateUnpressed;
	// create up and down surfaces for button face
	DDSURFACEDESC2 ddsd;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth  = width;
	ddsd.dwHeight = height;
	// set surface to offscreen plain (in sys mem)
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	// create the bg surface
	lpdd->CreateSurface(&ddsd,&lpddsButtonUp,NULL);
	lpdd->CreateSurface(&ddsd,&lpddsButtonDn,NULL);
	// set color key to color 0
	DDCOLORKEY color_key; // used to set color key
	color_key.dwColorSpaceLowValue  = 0;
	color_key.dwColorSpaceHighValue = 0;
	// now set the color key for source blitting
	lpddsButtonUp->SetColorKey(DDCKEY_SRCBLT, &color_key);
	lpddsButtonDn->SetColorKey(DDCKEY_SRCBLT, &color_key);
	// read button up/down bitmaps into surfaces
	LoadSurfaceImage(lpddsButtonUp,width,height,&bmpFileUp,0,0,BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bmpFileUp);
	LoadSurfaceImage(lpddsButtonDn,width,height,&bmpFileDn,0,0,BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bmpFileDn);
}

GUIButton::~GUIButton()
{
	//release buttons' surfaces
	lpddsButtonUp->Release();
	lpddsButtonDn->Release();
}

void GUIButton::draw()
{
	RECT dest_rect;
	//fill in the destination rect
	dest_rect.left   = left;
	dest_rect.top    = top;
	dest_rect.right  = right;
	dest_rect.bottom = bottom;
	if (state==ButtonStatePressed)
		lpddsBackBuffer->Blt(&dest_rect,lpddsButtonDn,NULL,(DDBLT_WAIT | DDBLT_KEYSRC),NULL);
	else
		lpddsBackBuffer->Blt(&dest_rect,lpddsButtonUp,NULL,(DDBLT_WAIT | DDBLT_KEYSRC),NULL);
}

// Utility funcs ///////////////////////////////////////////////////////////////////////////////////

//extracts a bitmap out of a bitmap file and draws it into a surface
//NOTE: remember:
// - that surfaces should be a multiple of 8
// - 
int LoadSurfaceImage(IDirectDrawSurface4* lpdds,	// surface
					 int width, int height,
					 BITMAP_FILE_PTR bitmap,		// bitmap to scan image data from
					 int cx, int cy,	// cell or absolute pos. to scan image from
					 int mode)			// if 0 then cx,cy is cell position, else 
											// cx,cy are absolute coords
{
	UCHAR *source_ptr,   // working pointers
		  *dest_ptr;
	DDSURFACEDESC2 ddsd;  //  direct draw surface description 

	// test the mode of extraction, cell based or absolute
	if (mode==BITMAP_EXTRACT_MODE_CELL) { // re-compute x,y ?
		cx = cx*(width+1) + 1;
		cy = cy*(height+1) + 1;
	}
	// extract bitmap data
	source_ptr = bitmap->buffer + cy*bitmap->bitmapinfoheader.biWidth+cx;
	// get the addr to destination surface memory
	// set size of the structure
	ddsd.dwSize = sizeof(ddsd);
	// lock the display surface
	lpdds->Lock(NULL,
				&ddsd,
				DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
				NULL);
	// assign a pointer to the memory surface for manipulation
	dest_ptr = (UCHAR*)ddsd.lpSurface;
	//char txt[50],txtfrag[50];
	//strcpy(txt,itoa(ddsd.lPitch,txtfrag,10));
	//Write_Error(txt);
	// iterate thru each scanline and copy bitmap
	for (int index_y=0; index_y<height; index_y++) {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr,width);
		// advance pointers
		dest_ptr   += width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	}
	// unlock the surface 
	lpdds->Unlock(NULL);
	// return success
	return(1);
}


