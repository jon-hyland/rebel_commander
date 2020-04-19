
LPPOPMSG CreateMessage(char** msgs, int numMsgs, int x, int y, int width, int height)
{
	LPPOPMSG msg;
	msg=(LPPOPMSG)malloc(sizeof(POPMSG));
	memset(msg,0,sizeof(POPMSG));
	msg->attr=POPMSG_STYLE_WINDOWSHADEOPEN | POPMSG_STYLE_WINDOWSHADECLOSE;
	msg->currX=msg->currY=msg->currWidth=msg->currHeight=msg->fade=0;
	msg->bmpImage=NULL;
	msg->numMsg=numMsgs;
	msg->currMsg=0;
	msg->x=x;
	msg->y=y;
	msg->width=width;
	msg->height=height;
	msg->visible=1;
	msg->state=POPMSG_STATE_CLOSED;
	msg->msg=msgs;
	return msg;
}

void ShowMessages()
{
	if (gameState==GS_MSGPOPUP) return;
	gameState=GS_TRANS_MSGPOPUP;
	ProcessInputKeyboard=ProcessInputKeyboard_Message;
}

void HideMessages()
{
	gameState=GS_RUNNING;
	ProcessInputKeyboard=ProcessInputKeyboard_Normal;
}

void DrawMessage(LPPOPMSG msg)
{
	HDC hdc; // the working dc
	RECT rect;
	int calcHeight;
	BITMAP_FILE bmpFile; bmpFile.buffer=NULL;
	const int BORDER = 5;

	if (msg->visible) {
		// get the dc from surface
		if (FAILED(lpddsback->GetDC(&hdc)))
		   return;
		HBRUSH brush;
		HPEN pen=CreatePen(PS_SOLID,3,RGB(255,255,255));
		SelectObject(hdc,pen);
		switch (msg->state) {
		case POPMSG_STATE_OPENING:
			if (msg->attr & POPMSG_STYLE_WINDOWSHADEOPEN) {
				if (msg->currHeight>=msg->height) { msg->currHeight=msg->height; msg->state=POPMSG_STATE_OPEN; }
				else msg->currHeight+=3;
				brush=CreateSolidBrush(RGB(0,0,0));
				SelectObject(hdc,brush);
				Rectangle(hdc,msg->x,msg->y,msg->x+msg->width,msg->y+msg->currHeight);
			}
			break;
		case POPMSG_STATE_OPEN:
			brush=CreateSolidBrush(RGB(0,0,0));
			SelectObject(hdc,brush);
			SetBkMode(hdc,TRANSPARENT);
			SetTextColor(hdc,RGB(255,255,255));
			if (msg->currMsg<msg->numMsg) {
				char* txt;
				if (msg->bmpImage!=NULL) {
					//draw msg rect
					txt=&msg->msg[msg->currMsg][38];
					rect.left=msg->x+BORDER;
					rect.top=msg->y+BORDER;
					rect.right=msg->x+msg->width-BORDER;
					rect.bottom=msg->y+msg->height-BORDER;
					calcHeight=DrawText(hdc,txt,strlen(txt),&rect,DT_WORDBREAK|DT_CALCRECT);
					rect.left=msg->x;
					rect.top=msg->y;
					rect.right=msg->x+msg->width;
					rect.bottom=msg->y+calcHeight+2*BORDER;
					if (rect.bottom<msg->bmpImage->y+msg->bmpImage->height)
						rect.bottom=msg->bmpImage->y+msg->bmpImage->height+BORDER;
					Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
					//draw img
					DeleteObject(brush);
					lpddsback->ReleaseDC(hdc);
					back_buffer=DDraw_Lock_Back_Surface();
					Draw_Bitmap(msg->bmpImage,back_buffer, back_lpitch,0);
					DDraw_Unlock_Back_Surface();
					lpddsback->GetDC(&hdc);
					//
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,RGB(255,255,255));
					rect.left+=BORDER;
					rect.top+=BORDER;
					rect.right-=BORDER;
					rect.bottom=rect.top+calcHeight;
					DrawText(hdc,txt,strlen(txt),&rect,DT_WORDBREAK);
				}
				else if (msg->msg[msg->currMsg][0]=='\\') {
					//get image params
					char fn[13],bw[5],bh[5],bx[5],by[5];
					int bmpWidth,bmpHeight,bmpX,bmpY;
					strcpy(fn,&msg->msg[msg->currMsg][1]);
					strcpy(bw,&msg->msg[msg->currMsg][15]);
					strcpy(bh,&msg->msg[msg->currMsg][21]);
					strcpy(bx,&msg->msg[msg->currMsg][27]);
					strcpy(by,&msg->msg[msg->currMsg][33]);
					bmpX=atoi(bx);
					bmpY=atoi(by);
					bmpWidth=atoi(bw);
					bmpHeight=atoi(bh);
					//draw msg rect
					//rect.left=msg->x;
					//rect.top=msg->y;
					//rect.right=msg->x+msg->width;
					//rect.bottom=msg->y+msg->height;
					//Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
					//draw img
					Load_Bitmap_File(&bmpFile, fn);
					msg->bmpImage=(BITMAP_IMAGE*)malloc(sizeof(BITMAP_IMAGE));
					memset(msg->bmpImage,0,sizeof(BITMAP_IMAGE));
	 				Create_Bitmap(msg->bmpImage,bmpX,bmpY,bmpWidth,bmpHeight);
					Load_Image_Bitmap(msg->bmpImage,&bmpFile,0,0,BITMAP_EXTRACT_MODE_ABS);
					Unload_Bitmap_File(&bmpFile);
					//lpddsback->ReleaseDC(hdc);
					//back_buffer=DDraw_Lock_Back_Surface();
					//Draw_Bitmap(msg->bmpImage,back_buffer, back_lpitch,0);
					//DDraw_Unlock_Back_Surface();
					//lpddsback->GetDC(&hdc);
				}
				else {
					txt=msg->msg[msg->currMsg];
					rect.left=msg->x+BORDER;
					rect.top=msg->y+BORDER;
					rect.right=msg->x+msg->width-BORDER;
					rect.bottom=msg->y+msg->height-BORDER;
					calcHeight=DrawText(hdc,txt,strlen(txt),&rect,DT_WORDBREAK|DT_CALCRECT);
					rect.left=msg->x;
					rect.top=msg->y;
					rect.right=msg->x+msg->width;
					rect.bottom=msg->y+calcHeight+2*BORDER;
					Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
					//
					rect.left+=BORDER;
					rect.top+=BORDER;
					rect.right-=BORDER;
					rect.bottom=rect.top+calcHeight;
					DrawText(hdc,txt,strlen(txt),&rect,DT_WORDBREAK);
				}
			}
			else
				msg->state=POPMSG_STATE_CLOSING;
			//clean up any stuff for DC
			DeleteObject(brush);
			break;
		case POPMSG_STATE_CLOSING:
			if (msg->attr & POPMSG_STYLE_WINDOWSHADECLOSE) {
				if (msg->currHeight<=0) { FREE(msg->bmpImage); msg->currHeight=0; msg->state=POPMSG_STATE_CLOSED; msg->visible=0; }
				else msg->currHeight-=3;
				brush=CreateSolidBrush(RGB(0,0,0));
				SelectObject(hdc,brush);
				Rectangle(hdc,msg->x,msg->y,msg->x+msg->width,msg->y+msg->currHeight);
			}
			break;
		case POPMSG_STATE_CLOSED:
			msg->state=POPMSG_STATE_OPENING;
			break;
		}
		// release HDC
		lpddsback->ReleaseDC(hdc);
	}
}


