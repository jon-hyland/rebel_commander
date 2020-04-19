//topscore.h: header file for topscore.cpp

//Copyright Ian Parberry, 2000
//Last updated February 10, 2000

#ifndef __TOPSCORE__
#define __TOPSCORE__

#include <ddraw.h>

#define MAX_TOPSCORES 8
#define MAX_TOPSCORE_NAMELENGTH 17 

class CHighScoreList{
  private:
    char m_pNames[MAX_TOPSCORES][MAX_TOPSCORE_NAMELENGTH]; //names
    int m_pTopScore[MAX_TOPSCORES]; //top scores
    int m_pTopLevel[MAX_TOPSCORES]; //top levels
    int m_nChecksum0,m_nChecksum1; //checksums
    char m_szNamebuffer[MAX_TOPSCORE_NAMELENGTH]; //name buffer
    int m_nNamesize; //size of name in namebuffer
    void save(); //save all high scores
    void save(HKEY RegistryKey,int index); //save one high score
    void load(); //load all high scores
    void load(HKEY RegistryKey,int index); //load one high score
    int enumerate(); //turn data into a number
    int checksum0(); //calculate a checksum
    int checksum1(); //calculate a checksum
    void load_checksums(HKEY RegistryKey); //load checksums
    void save_checksums(HKEY RegistryKey); //save checksums
    BOOL checksums_ok(); //TRUE if checksums are OK
  public:
    CHighScoreList(); //constructor
    BOOL insert(int level,int score); //insert new high score
    void draw(LPDIRECTDRAWSURFACE surface); //draw high score
    void clear(); //clear all high scores
    BOOL made_the_grade(int score); //TRUE if score gets on table
    //the following functions affect the current name
    BOOL append_to_current(char c); //insert char at end
    BOOL delete_from_current(); //delete char at end
    void cancel_current(); //clear out the current name
    char *current_name(); //return current name
};

#endif

