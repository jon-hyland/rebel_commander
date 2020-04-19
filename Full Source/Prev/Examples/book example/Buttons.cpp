//buttons.cpp: button class
//assumes that the menu background has been drawn to
//both primary and secondary surfaces

//Copyright Ian Parberry, 1999
//Last updated March 15, 2000

#include "buttons.h"
#include "sound.h" //for sound manager

extern LPDIRECTDRAWSURFACE lpPrimary; //primary surface
extern LPDIRECTDRAWSURFACE lpSecondary; //back buffer
extern CSoundManager* SoundManager; //sound manager

BOOL PageFlip(); //from demo??.cpp

CButtonManager::CButtonManager(int count,SIZE size,
                               char *filename){
  m_rectHotspot=new RECT[m_nMaxCount=count]; //hotspot space
  m_bDown=new BOOL[m_nMaxCount]; //whether button is down
  for(int i=0; i<m_nMaxCount; i++)
    m_bDown[i]=FALSE; //all buttons up
  m_sizeButton=size; //size of buttons loaded
  m_nCount=0; //number of buttons loaded
  loadsprite(filename); //load the sprite
  m_nButtonDownSound=m_nButtonUpSound=-1; //no sounds yet
  m_bRadio=FALSE; //not radio buttons
}

CButtonManager::CButtonManager(int count,SIZE size,
                POINT point,int ydelta,char *filename){
  m_rectHotspot=new RECT[m_nMaxCount=count]; //hotspot space
  m_bDown=new BOOL[m_nMaxCount]; //whether button is down
  for(int i=0; i<m_nMaxCount; i++)
    m_bDown[i]=FALSE; //all buttons up
  m_sizeButton=size; //size of buttons
  loadsprite(filename); //load the sprite
  m_nButtonDownSound=m_nButtonUpSound=-1; //no sounds yet
  m_bRadio=FALSE; //not radio buttons
  //add the buttons
  m_nCount=0; addbutton(point);
  for(i=0; i<count-1; i++){
    point.y+=ydelta; addbutton(point);
  }
}

CButtonManager::~CButtonManager(){ //destructor
  delete[]m_rectHotspot;
  delete m_spriteDefault;
  delete[]m_bDown;
}

void CButtonManager::set_radio(BOOL on){ //set to radio buttons
  m_bRadio=on;
}

BOOL CButtonManager::addbutton(RECT rect){ 
//add a hotspot in this rectangle
  if(m_nCount>=m_nMaxCount)return FALSE; //bail if no space
  m_rectHotspot[m_nCount++]=rect; //insert hotspot
  return TRUE;
}

BOOL CButtonManager::addbutton(POINT point){
//add a hotspot with top left at point
  RECT rect; //bounding rectangle
  rect.left=point.x; rect.right=point.x+m_sizeButton.cx; //horizontal
  rect.top=point.y; rect.bottom=point.y+m_sizeButton.cy; //vertical
  return addbutton(rect); //add hotspot in this rectangle
}

int CButtonManager::hit(POINT point){
//return index of button hit (-1 if no hit)
  int result=-1; //start by assuming no hit
  int i=0; //index
  while(i<m_nCount&&result<0) //for each button
    if(PointInRect(m_rectHotspot[i],point)) //check for hit
      result=i; //record hit
    else i++; //next button
  return result;
}

int CButtonManager::release(POINT point){ 
//return index of button for mouse release
  int result=-1; //start by assuming no hit
  int i=0; //index
  while(i<m_nCount&&result<0) //for each button
    if(m_bDown[i]&&PointInRect(m_rectHotspot[i],point))
      result=i; //a hit
    else i++; //next button
  return result;
}

BOOL CButtonManager::PointInRect(RECT rect,POINT point){
//is point in rectangle?
  return point.x>=rect.left&&point.x<=rect.right&&
    point.y>=rect.top&&point.y<=rect.bottom;
}

void CButtonManager::loadimages(){ //load button images
  m_spriteDefault->load(&m_cImage,0,0,0); //up
  m_spriteDefault->load(&m_cImage,1,0,m_sizeButton.cy); //down
}

BOOL CButtonManager::loadsprite(char *filename){
  //bail if size not set
  if(m_sizeButton.cx==0&&m_sizeButton.cy==0)return FALSE;
 //bail if can't load file
  if(!m_cImage.load(filename))return FALSE;
  //make sprite
  m_spriteDefault=
    new CClippedSprite(2,m_sizeButton.cx,m_sizeButton.cy);
  //load button images
  loadimages();
  return TRUE;
}

BOOL CButtonManager::Restore(){ //restore surfaces
  if(m_spriteDefault->Restore()){
    loadimages();
    return TRUE;
  }
  else return FALSE;
}

void CButtonManager::Release(){ //release surfaces
  m_spriteDefault->Release();
}

BOOL CButtonManager::buttondown(POINT point){
  return buttondown(hit(point));
}

BOOL CButtonManager::buttondown(int index,BOOL sound){
  if(index>=0&&index<m_nCount&&!m_bDown[index]){
    if(sound)SoundManager->play(m_nButtonDownSound);
    if(m_bRadio&&index<m_nCount-1) //if radio button (not last)
      for(int i=0; i<m_nCount-1; i++)
        m_bDown[i]=FALSE; //pop up other radio buttons
    m_bDown[index]=TRUE; //button is now down
    display_menu(); //display menu page
    return TRUE;
  }
  else return FALSE;
}

BOOL CButtonManager::buttonup(POINT point){
  return buttonup(hit(point));
}

BOOL CButtonManager::buttonup(int index){
  if((!m_bRadio||index==m_nCount-1)&&
  index>=0&&index<m_nCount&&m_bDown[index]){ //if valid
    SoundManager->play(m_nButtonUpSound);
    m_bDown[index]=FALSE; //button is up
    display_menu(); //display menu page
    return TRUE;
  }
  else return FALSE;
}

void CButtonManager::allbuttonsup(){
  int i; //index
  if(m_bRadio){ //don't pop up radio buttons
    if(m_bDown[m_nCount-1]){ //pop up quit button if down
      m_bDown[m_nCount-1]=FALSE;
      SoundManager->play(m_nButtonUpSound);
    }
  }
  else{ //not radio buttons, so pop up all of them
    BOOL found=FALSE; //to make sure sound played only once
    for(i=0; i<m_nCount; i++) //for each button
      if(m_bDown[i]){ //if valid down button
        found=TRUE; //will play sound later
        m_bDown[i]=FALSE; //button is up
      }
    if(found)SoundManager->play(m_nButtonUpSound);
  }
  display_menu(); //display menu page
}

void CButtonManager::display_menu(){ //display menu page 
  for(int i=0; i<m_nCount; i++) //for each button
    if(m_bDown[i]) //draw button down
      m_spriteDefault->
        draw(1,m_rectHotspot[i].left+m_sizeButton.cx/2,
          m_rectHotspot[i].top+m_sizeButton.cy,lpSecondary);
    else //draw button up
      m_spriteDefault->
        draw(0,m_rectHotspot[i].left+m_sizeButton.cx/2,
          m_rectHotspot[i].top+m_sizeButton.cy,lpSecondary);
  PageFlip(); //flip back buffer to front
}

void CButtonManager::setsounds(int down,int up){ //set sounds
  m_nButtonDownSound=down; m_nButtonUpSound=up;
}
