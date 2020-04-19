//topscore.cpp: high score list

//Copyright Ian Parberry, 2000
//Last updated February 10, 2000

#include <windows.h>
#include <stdio.h>

#include "topscore.h"
#include "text.h" //for text sprite manager

const int PRIME0=16787; //a 16-bit prime number
const int PRIME1=15241; //a 16-bit prime number
const int PRIME2=16223; //a 16-bit prime number
const int PRIME3=15907; //a 16-bit prime number

extern CTextManager *Text; //text manager

CHighScoreList::CHighScoreList(){ //constructor
  m_nNamesize=0;
  //null out data
  for(int i=0; i<MAX_TOPSCORES; i++){
    m_pNames[i][0]=0; m_pTopScore[i]=m_pTopLevel[i]=0;
  }
  m_nChecksum0=m_nChecksum1=0; //checksums
  //read data from registry
  load();
}

void CHighScoreList::clear(){
  //null out data
  for(int i=0; i<MAX_TOPSCORES; i++){
    m_pNames[i][0]=0; m_pTopScore[i]=m_pTopLevel[i]=0;
  }
  m_nChecksum0=m_nChecksum1=0; //checksums
  //write data from registry
  save();
}

BOOL CHighScoreList::insert(int level,int score){ 
//insert new entry, return TRUE if entry actually fits
  int i=0,j;
  BOOL result=FALSE;
  //bail out of bad data
  if(score<=0)return FALSE; //bail if no score
  if(strlen(m_szNamebuffer)<=0)return FALSE; //bail if no name
  //search for place in table sorted on score
  while(i<MAX_TOPSCORES&&m_pTopScore[i]>=score)++i;
  if(i<MAX_TOPSCORES){ //new entry
    result=TRUE;
    //move data down in table to make room
    for(j=MAX_TOPSCORES-1; j>i; j--){
      m_pTopScore[j]=m_pTopScore[j-1]; //score
      m_pTopLevel[j]=m_pTopLevel[j-1]; //level
      strcpy(m_pNames[j],m_pNames[j-1]); //name
    }
    //enter new data in place
    m_pTopLevel[i]=level; m_pTopScore[i]=score; //level, score
    strncpy(m_pNames[i],m_szNamebuffer,
      MAX_TOPSCORE_NAMELENGTH-1); //name
    //compute checksums and save to registry
    m_nChecksum0=checksum0(); //compute checksum
    m_nChecksum1=checksum1(); //compute checksum
    save(); //save new high score list to registry
  }
  return result;
}

void CHighScoreList::draw(LPDIRECTDRAWSURFACE surface){ 
//draw to surface
  const int topmargin=150; //margin at top
  const int leftmargin=15; //margin at left
  const int levelmargin=380; //left margin for level number
  const int scoremargin=500; //left margin for score
  const int vertskip=40; //line height
  char buffer[16]; //temporary text buffer
  int i=0;
  while(i<MAX_TOPSCORES&&m_pTopScore[i]>0){ //for each score
    int y=topmargin+i*vertskip;
    //name
    Text->jump(leftmargin,y); //start of line
    Text->draw(m_pNames[i],surface); //print name
    //level
    Text->jump(levelmargin,y); //start of line
    sprintf(buffer,"%d",m_pTopLevel[i]); //convert to string
    Text->draw(buffer,surface); //print score
    //score
    Text->jump(scoremargin,y); //start of line
    sprintf(buffer,"%d",m_pTopScore[i]); //convert to string
    Text->draw(buffer,surface); //print score
    i++;
  }
}

void CHighScoreList::load(HKEY RegistryKey,int index){ 
//load one entry from registry
  unsigned long length; //length of entry
  const int BUFSIZE=32; //buffer size
  char buffer[BUFSIZE]; //text buffer
  DWORD type; //type of registry entry
  char id[16]; //player name (identifier)
  sprintf(id,"Player%dName",index); //create id
  //get name
  length=MAX_TOPSCORE_NAMELENGTH;
  RegQueryValueEx(RegistryKey,id,NULL,&type,
    (unsigned char *)(m_pNames[index]),&length);
  m_pNames[index][length]=0;
  //get level
  sprintf(id,"Player%dLevel",index);
  length=BUFSIZE-1; buffer[0]=0;
  RegQueryValueEx(RegistryKey,id,NULL,&type,
    (unsigned char *)buffer,&length);
  if(length){
    buffer[length]=0;
    sscanf(buffer,"%d",&(m_pTopLevel[index]));
  }
  else m_pTopLevel[index]=0;
  //get level
  sprintf(id,"Player%dScore",index);
  length=BUFSIZE-1; buffer[0]=0;
  RegQueryValueEx(RegistryKey,id,NULL,&type,
    (unsigned char *)buffer,&length);
  if(length){
    buffer[length]=0;
    sscanf(buffer,"%d",&(m_pTopScore[index]));
  }
  else m_pTopScore[index]=0;
}

void CHighScoreList::load(){ //load from registry
  HKEY RegistryKey;
  //open registry
  int result=RegCreateKeyEx(HKEY_LOCAL_MACHINE,
    "SOFTWARE\\LARC\\NedFarm",0,NULL,0,KEY_READ,
    NULL,&RegistryKey,NULL);
  if(result!=ERROR_SUCCESS)return; //bail if failed
  //load high scores and checksums
  for(int i=0; i<MAX_TOPSCORES; i++)load(RegistryKey,i);
  load_checksums(RegistryKey);
  if(!checksums_ok())clear();
  //close and exit
  RegCloseKey(RegistryKey);
}

void CHighScoreList::load_checksums(HKEY RegistryKey){ 
//load checksums from registry
  unsigned long length; //length of entry
  const int BUFSIZE=32; //buffer size
  char buffer[BUFSIZE]; //text buffer
  DWORD type; //type of registry entry
  //checksum0
  length=BUFSIZE-1; buffer[0]=0;
  RegQueryValueEx(RegistryKey,"Checksum0",NULL,&type,
    (unsigned char *)buffer,&length);
  if(length){
    buffer[length]=0;
    sscanf(buffer,"%d",&m_nChecksum0);
  }
  else m_nChecksum0=0;
  //checksum1
  length=BUFSIZE-1; buffer[0]=0;
  RegQueryValueEx(RegistryKey,"Checksum1",NULL,&type,
    (unsigned char *)buffer,&length);
  if(length){
    buffer[length]=0;
    sscanf(buffer,"%d",&m_nChecksum1);
  }
  else m_nChecksum1=0;
}

void CHighScoreList::save(HKEY RegistryKey,int index){
//save one entry in registry
  const int BUFSIZE=32; //buffer size
  char buffer[BUFSIZE]; //text buffer
  char id[16]; //player name (identifier)
  //put name
  sprintf(id,"Player%dName",index);
  RegSetValueEx(RegistryKey,id,NULL,REG_SZ,
    (unsigned char *)m_pNames[index],strlen(m_pNames[index])+1);
  //put level
  sprintf(id,"Player%dLevel",index);
  sprintf(buffer,"%d",m_pTopLevel[index]);
  RegSetValueEx(RegistryKey,id,NULL,REG_SZ,
    (unsigned char *)buffer,strlen(buffer)+1);
  //put score
  sprintf(id,"Player%dScore",index);
  sprintf(buffer,"%d",m_pTopScore[index]);
  RegSetValueEx(RegistryKey,id,NULL,REG_SZ,
    (unsigned char *)buffer,strlen(buffer)+1);
}

void CHighScoreList::save(){ //save all data in registry
  HKEY RegistryKey;
  //open registry
  int result=RegCreateKeyEx(HKEY_LOCAL_MACHINE,
    "SOFTWARE\\LARC\\NedFarm",0,NULL,0,KEY_WRITE,
    NULL,&RegistryKey,NULL);
  if(result!=ERROR_SUCCESS)return; //bail if failed
  //save high scores and checksums
  for(int i=0; i<MAX_TOPSCORES; i++)save(RegistryKey,i);
  save_checksums(RegistryKey);
  //close and exit
  RegCloseKey(RegistryKey);
}

void CHighScoreList::save_checksums(HKEY RegistryKey){ 
//save checksums to registry
  const int BUFSIZE=32; //buffer size
  char buffer[BUFSIZE]; //text buffer
  //checksum0
  sprintf(buffer,"%d",m_nChecksum0);
  RegSetValueEx(RegistryKey,"Checksum0",NULL,REG_SZ,
    (unsigned char *)buffer,strlen(buffer)+1);
  //checksum1
  sprintf(buffer,"%d",m_nChecksum1);
  RegSetValueEx(RegistryKey,"Checksum1",NULL,REG_SZ,
    (unsigned char *)buffer,strlen(buffer)+1);
}

BOOL CHighScoreList::made_the_grade(int score){
//TRUE if entry fits in table
  return score>m_pTopScore[MAX_TOPSCORES-1];
}

int CHighScoreList::enumerate(){
//turn high score table into a number
  int result=0,i,j;
  const int mask=0xFFFF; //mask for lower 16 bits
  for(i=0; i<MAX_TOPSCORES; i++){
    j=0;
    while(m_pNames[i][j]!=0){result+=m_pNames[i][j]; j++;} //names
    result+=m_pTopScore[i]+m_pTopLevel[i]; //score and level
    result&=mask; //mask off lower 16 bits
  }
  return result;
}

int CHighScoreList::checksum0(){ //compute a checksum
  return enumerate()*PRIME0%PRIME1;
}

int CHighScoreList::checksum1(){ //compute a checksum
  return enumerate()*PRIME2%PRIME3;
}

BOOL CHighScoreList::checksums_ok(){ //TRUE if checksums are OK
  return m_nChecksum0==checksum0()&&m_nChecksum1==checksum1();
}

BOOL CHighScoreList::append_to_current(char c){ 
//insert char at end of current name
  if(m_nNamesize<MAX_TOPSCORE_NAMELENGTH-1){ //if there is room
    m_szNamebuffer[m_nNamesize++]=c; //append character
    m_szNamebuffer[m_nNamesize]=0x00; //null at end
    return TRUE; //success
  }
  else return FALSE; //no room
}

BOOL CHighScoreList::delete_from_current(){ 
//delete char at end of current name
  if(m_nNamesize>0)
    m_szNamebuffer[--m_nNamesize]=0x00; //back off & put a null
  else{
    m_nNamesize=0; //fell off beginning of string
    return FALSE; //deletion didn't happen
  }
  return TRUE; //deletion actually happened
}

void CHighScoreList::cancel_current(){ //clear the current name
  m_szNamebuffer[m_nNamesize=0]=0;
}

char* CHighScoreList::current_name(){ //return current name
  return m_szNamebuffer;
}
