#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>

#include "csprite.h"
#include "sound.h"

extern int GameState;
extern LPDIRECTDRAWSURFACE lpPrimary;
extern LPDIRECTDRAWPALETTE lpPalette;
extern LPDIRECTDRAWSURFACE lpSecondary;
extern LPDIRECTDRAWSURFACE lpBackground;
extern CBmpFileReader background;
extern CSoundManager *SoundManager;
extern CClippedSprite *g_pSprite[SPRITE_COUNT];

CBmpSpriteFileReader g_cRedAlphaSprites;
CBmpSpriteFileReader g_cBlueAlphaSprites;
CBmpSpriteFileReader g_cShipSprites;
CBmpSpriteFileReader g_cBlipSprites;
CBmpSpriteFileReader g_cGroundCannonSprites;
CBmpSpriteFileReader g_cUpsSprites;
CBmpSpriteFileReader g_cDefaultFire;
CBmpSpriteFileReader g_cShipBombSprites;
CBmpSpriteFileReader g_cSlithSprites;
CBmpSpriteFileReader g_cSparkSprites;

void LoadSounds()
{
	SoundManager->Load("alien_intro.wav");
	SoundManager->Load("blip_die.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("continue_wait.wav");
	SoundManager->Load("engine_loop.wav");
	SoundManager->Load("gameover_wait.wav");
	SoundManager->Load("generic_die_1.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("invulnerable.wav");
	SoundManager->Load("menu_up.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("ok.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("ping.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("rebirth.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("score.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("select.wav");
	SoundManager->Load("ship_die.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("upgrade.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("zap_1.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("zap_2.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("zap_3.wav",MAX_SOUND_INSTANCES);
	SoundManager->Load("zap_full.wav",MAX_SOUND_INSTANCES);
}


BOOL LoadGraphics()
{
	//load background image(s)
	//set palette based on background image
	if(!background.load("bottom_red.bmp")) return FALSE;
	background.draw(lpBackground);
	if(!background.setpalette(lpPalette)) return FALSE;

	//load blue alpha sprites
	if(!g_cBlueAlphaSprites.load("blue_alpha.bmp")) return FALSE;
	g_pSprite[BLUE_ALPHA_SPRITE]=new CClippedSprite(40,16,16);
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,0,1,1)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,1,18,1)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,2,35,1)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,3,52,1)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,4,69,1)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,5,86,1)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,6,103,1)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,7,1,18)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,8,18,18)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,9,35,18)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,10,52,18)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,11,69,18)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,12,86,18)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,13,103,18)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,14,1,35)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,15,18,35)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,16,35,35)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,17,52,35)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,18,69,35)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,19,86,35)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,20,103,35)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,21,1,52)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,22,18,52)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,23,35,52)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,24,52,52)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,25,69,52)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,26,86,52)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,27,103,52)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,28,1,69)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,29,18,69)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,30,35,69)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,31,52,69)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,32,69,69)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,33,86,69)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,34,103,69)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,35,1,86)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,36,18,86)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,37,35,86)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,38,52,86)) return FALSE;
	if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,39,69,86)) return FALSE;
	
	//load red alpha sprites
	if(!g_cRedAlphaSprites.load("red_alpha.bmp")) return FALSE;
	g_pSprite[RED_ALPHA_SPRITE]=new CClippedSprite(40,16,16);
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,0,1,1)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,1,18,1)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,2,35,1)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,3,52,1)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,4,69,1)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,5,86,1)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,6,103,1)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,7,1,18)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,8,18,18)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,9,35,18)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,10,52,18)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,11,69,18)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,12,86,18)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,13,103,18)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,14,1,35)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,15,18,35)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,16,35,35)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,17,52,35)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,18,69,35)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,19,86,35)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,20,103,35)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,21,1,52)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,22,18,52)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,23,35,52)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,24,52,52)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,25,69,52)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,26,86,52)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,27,103,52)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,28,1,69)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,29,18,69)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,30,35,69)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,31,52,69)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,32,69,69)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,33,86,69)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,34,103,69)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,35,1,86)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,36,18,86)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,37,35,86)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,38,52,86)) return FALSE;
	if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,39,69,86)) return FALSE;

	//load ship graphics
	if(!g_cShipSprites.load("ship_sprites.bmp")) return FALSE;
	g_pSprite[SHIP_SPRITE]=new CClippedSprite(2,64,32);
	g_pSprite[SHIP_UP_SPRITE]=new CClippedSprite(2,64,32);
	g_pSprite[SHIP_DOWN_SPRITE]=new CClippedSprite(2,64,32);
	g_pSprite[SHIP_DYING_SPRITE]=new CClippedSprite(4,64,32);
	if(!g_pSprite[SHIP_SPRITE]->load(&g_cShipSprites,0,1,1)) return FALSE;
	if(!g_pSprite[SHIP_SPRITE]->load(&g_cShipSprites,1,66,1)) return FALSE;
	if(!g_pSprite[SHIP_UP_SPRITE]->load(&g_cShipSprites,0,131,1)) return FALSE;
	if(!g_pSprite[SHIP_UP_SPRITE]->load(&g_cShipSprites,1,196,1)) return FALSE;
	if(!g_pSprite[SHIP_DOWN_SPRITE]->load(&g_cShipSprites,0,261,1)) return FALSE;
	if(!g_pSprite[SHIP_DOWN_SPRITE]->load(&g_cShipSprites,1,1,34)) return FALSE;
	if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,0,66,34)) return FALSE;
	if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,1,131,34)) return FALSE;
	if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,2,196,34)) return FALSE;
	if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,3,261,34)) return FALSE;

	//load blip graphics	
	if(!g_cBlipSprites.load("blip_sprites.bmp")) return FALSE;
	g_pSprite[ORANGE_BLIP_SPRITE]=new CClippedSprite(4,32,32);
	g_pSprite[ORANGE_BLIP_DYING_SPRITE]=new CClippedSprite(3,32,32);
	g_pSprite[GREEN_BLIP_SPRITE]=new CClippedSprite(4,32,32);
	g_pSprite[GREEN_BLIP_DYING_SPRITE]=new CClippedSprite(3,32,32);
	if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,0,1,1)) return FALSE;
	if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,1,34,1)) return FALSE;
	if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,2,67,1)) return FALSE;
	if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,3,100,1)) return FALSE;
	if(!g_pSprite[ORANGE_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,0,1,34)) return FALSE;
	if(!g_pSprite[ORANGE_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,1,34,34)) return FALSE;
	if(!g_pSprite[ORANGE_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,2,67,34)) return FALSE;
	if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,0,1,67)) return FALSE;
	if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,1,34,67)) return FALSE;
	if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,2,67,67)) return FALSE;
	if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,3,100,67)) return FALSE;
	if(!g_pSprite[GREEN_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,0,1,100)) return FALSE;
	if(!g_pSprite[GREEN_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,1,34,100)) return FALSE;
	if(!g_pSprite[GREEN_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,2,67,100)) return FALSE;
	
	//load ground cannon graphics
	if(!g_cGroundCannonSprites.load("ground_cannon_sprites.bmp")) return FALSE;
	g_pSprite[GROUND_CANNON_SPRITE]=new CClippedSprite(1,36,32);
	if(!g_pSprite[GROUND_CANNON_SPRITE]->load(&g_cGroundCannonSprites,0,3,3)) return FALSE;

	//load ups graphics	
	if(!g_cUpsSprites.load("ups_sprites.bmp")) return FALSE;
	g_pSprite[WEAPON_UP_SPRITE]=new CClippedSprite(2,32,28);
	g_pSprite[SPEED_UP_SPRITE]=new CClippedSprite(2,32,28);
	g_pSprite[LIFE_UP_SPRITE]=new CClippedSprite(2,32,28);
	if(!g_pSprite[WEAPON_UP_SPRITE]->load(&g_cUpsSprites,0,1,1)) return FALSE;
	if(!g_pSprite[WEAPON_UP_SPRITE]->load(&g_cUpsSprites,1,34,1)) return FALSE;
	if(!g_pSprite[SPEED_UP_SPRITE]->load(&g_cUpsSprites,0,1,30)) return FALSE;
	if(!g_pSprite[SPEED_UP_SPRITE]->load(&g_cUpsSprites,1,34,30)) return FALSE;
	if(!g_pSprite[LIFE_UP_SPRITE]->load(&g_cUpsSprites,0,1,59)) return FALSE;
	if(!g_pSprite[LIFE_UP_SPRITE]->load(&g_cUpsSprites,1,34,59)) return FALSE;

	//load default fire graphics	
	if(!g_cDefaultFire.load("default_fire.bmp")) return FALSE;
	g_pSprite[DEFAULT_FIRE_SPRITE]=new CClippedSprite(1,12,12);
	if(!g_pSprite[DEFAULT_FIRE_SPRITE]->load(&g_cDefaultFire,0,0,0)) return FALSE;

	//load ship bomb graphics	
	if(!g_cShipBombSprites.load("ship_bomb_sprites.bmp")) return FALSE;
	g_pSprite[SHIP_BOMB_SPRITE]=new CClippedSprite(2,18,22);
	if(!g_pSprite[SHIP_BOMB_SPRITE]->load(&g_cShipBombSprites,0,1,1)) return FALSE;
	if(!g_pSprite[SHIP_BOMB_SPRITE]->load(&g_cShipBombSprites,1,20,1)) return FALSE;

	//load slith graphics	
	if(!g_cSlithSprites.load("slith_sprites.bmp")) return FALSE;
	g_pSprite[BLUE_SLITH_SPRITE]=new CClippedSprite(4,32,32);
	g_pSprite[GOLD_SLITH_SPRITE]=new CClippedSprite(4,32,32);
	g_pSprite[SLITH_DYING_SPRITE]=new CClippedSprite(4,32,32);
	if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,0,1,1)) return FALSE;
	if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,1,34,1)) return FALSE;
	if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,2,67,1)) return FALSE;
	if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,3,100,1)) return FALSE;
	if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,0,1,34)) return FALSE;
	if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,1,34,34)) return FALSE;
	if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,2,67,34)) return FALSE;
	if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,3,100,34)) return FALSE;
	if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,0,1,67)) return FALSE;
	if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,1,34,67)) return FALSE;
	if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,2,67,67)) return FALSE;
	if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,3,100,67)) return FALSE;

	//load spark graphics
	if(!g_cSparkSprites.load("spark_sprites.bmp")) return FALSE;
	g_pSprite[SPARK_SPRITE]=new CClippedSprite(3,24,24);
	g_pSprite[SPARK_DYING_SPRITE]=new CClippedSprite(3,24,24);
	if(!g_pSprite[SPARK_SPRITE]->load(&g_cSparkSprites,0,1,1)) return FALSE;
	if(!g_pSprite[SPARK_SPRITE]->load(&g_cSparkSprites,1,26,1)) return FALSE;
	if(!g_pSprite[SPARK_SPRITE]->load(&g_cSparkSprites,2,51,1)) return FALSE; 
	if(!g_pSprite[SPARK_DYING_SPRITE]->load(&g_cSparkSprites,0,1,26)) return FALSE;
	if(!g_pSprite[SPARK_DYING_SPRITE]->load(&g_cSparkSprites,1,26,26)) return FALSE;
	if(!g_pSprite[SPARK_DYING_SPRITE]->load(&g_cSparkSprites,2,51,26)) return FALSE; 

	return TRUE;
}


BOOL RestoreSurfaces()
{
	BOOL result=TRUE;
	
	//restore primary and secondary surfaces
	if(SUCCEEDED(lpPrimary->Restore()))
		result=result&&background.draw(lpPrimary)&&background.setpalette(lpPalette);
	else return FALSE;
	if(SUCCEEDED(lpSecondary->Restore()))
		result=result&&background.draw(lpSecondary);
	else return FALSE;
	
	//restore background
	if(SUCCEEDED(lpBackground->Restore()))
		result=result&&background.draw(lpBackground);
	else return FALSE;

	//restore blue alpha
	if(g_pSprite[BLUE_ALPHA_SPRITE]->Restore())
	{
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,0,1,1)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,1,18,1)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,2,35,1)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,3,52,1)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,4,69,1)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,5,86,1)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,6,103,1)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,7,1,18)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,8,18,18)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,9,35,18)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,10,52,18)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,11,69,18)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,12,86,18)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,13,103,18)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,14,1,35)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,15,18,35)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,16,35,35)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,17,52,35)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,18,69,35)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,19,86,35)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,20,103,35)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,21,1,52)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,22,18,52)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,23,35,52)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,24,52,52)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,25,69,52)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,26,86,52)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,27,103,52)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,28,1,69)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,29,18,69)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,30,35,69)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,31,52,69)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,32,69,69)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,33,86,69)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,34,103,69)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,35,1,86)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,36,18,86)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,37,35,86)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,38,52,86)) return FALSE;
		if(!g_pSprite[BLUE_ALPHA_SPRITE]->load(&g_cBlueAlphaSprites,39,69,86)) return FALSE;
	}

	//restore red alpha
	if(g_pSprite[RED_ALPHA_SPRITE]->Restore())
	{
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,0,1,1)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,1,18,1)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,2,35,1)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,3,52,1)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,4,69,1)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,5,86,1)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,6,103,1)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,7,1,18)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,8,18,18)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,9,35,18)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,10,52,18)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,11,69,18)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,12,86,18)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,13,103,18)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,14,1,35)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,15,18,35)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,16,35,35)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,17,52,35)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,18,69,35)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,19,86,35)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,20,103,35)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,21,1,52)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,22,18,52)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,23,35,52)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,24,52,52)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,25,69,52)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,26,86,52)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,27,103,52)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,28,1,69)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,29,18,69)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,30,35,69)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,31,52,69)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,32,69,69)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,33,86,69)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,34,103,69)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,35,1,86)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,36,18,86)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,37,35,86)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,38,52,86)) return FALSE;
		if(!g_pSprite[RED_ALPHA_SPRITE]->load(&g_cRedAlphaSprites,39,69,86)) return FALSE;
	}

	//restore ship
	if(g_pSprite[SHIP_SPRITE]->Restore())
	{
		if(!g_pSprite[SHIP_SPRITE]->load(&g_cShipSprites,0,1,1)) return FALSE;
		if(!g_pSprite[SHIP_SPRITE]->load(&g_cShipSprites,1,66,1)) return FALSE;
	}
	else return FALSE;
	if(g_pSprite[SHIP_UP_SPRITE]->Restore())
	{
		if(!g_pSprite[SHIP_UP_SPRITE]->load(&g_cShipSprites,0,131,1)) return FALSE;
		if(!g_pSprite[SHIP_UP_SPRITE]->load(&g_cShipSprites,1,196,1)) return FALSE;
	}
	else return FALSE;
	if(g_pSprite[SHIP_DOWN_SPRITE]->Restore())
	{
		if(!g_pSprite[SHIP_DOWN_SPRITE]->load(&g_cShipSprites,0,261,1)) return FALSE;
		if(!g_pSprite[SHIP_DOWN_SPRITE]->load(&g_cShipSprites,1,1,34)) return FALSE;
	}
	else return FALSE;
	if(g_pSprite[SHIP_DYING_SPRITE]->Restore())
	{
		if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,0,66,34)) return FALSE;
		if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,1,131,34)) return FALSE;
		if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,2,196,34)) return FALSE;
		if(!g_pSprite[SHIP_DYING_SPRITE]->load(&g_cShipSprites,3,261,34)) return FALSE;
	}
	else return FALSE;
	
	//restore blip
	if(g_pSprite[ORANGE_BLIP_SPRITE]->Restore())
	{
		if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,0,1,1)) return FALSE;
		if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,1,34,1)) return FALSE;
		if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,2,67,1)) return FALSE;
		if(!g_pSprite[ORANGE_BLIP_SPRITE]->load(&g_cBlipSprites,3,100,1)) return FALSE;
	}
	else return FALSE;

	if(g_pSprite[ORANGE_BLIP_DYING_SPRITE]->Restore())
	{
		if(!g_pSprite[ORANGE_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,0,1,67)) return FALSE;
		if(!g_pSprite[ORANGE_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,1,34,67)) return FALSE;
		if(!g_pSprite[ORANGE_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,2,67,67)) return FALSE;
	}
	else return FALSE;

	if(g_pSprite[GREEN_BLIP_SPRITE]->Restore())
	{
		if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,0,1,67)) return FALSE;
		if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,1,34,67)) return FALSE;
		if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,2,67,67)) return FALSE;
		if(!g_pSprite[GREEN_BLIP_SPRITE]->load(&g_cBlipSprites,3,100,67)) return FALSE;
	}
	else return FALSE;

	if(g_pSprite[GREEN_BLIP_DYING_SPRITE]->Restore())
	{
		if(!g_pSprite[GREEN_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,0,1,100)) return FALSE;
		if(!g_pSprite[GREEN_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,1,34,100)) return FALSE;
		if(!g_pSprite[GREEN_BLIP_DYING_SPRITE]->load(&g_cBlipSprites,2,67,100)) return FALSE;
	}
	else return FALSE;

	//restore ground cannon
	if(g_pSprite[GROUND_CANNON_SPRITE]->Restore())
	{
		if(!g_pSprite[GROUND_CANNON_SPRITE]->load(&g_cGroundCannonSprites,0,3,3)) return FALSE;
	}
	else return FALSE;

	//restore ups
	if(g_pSprite[WEAPON_UP_SPRITE]->Restore())
	{
		if(!g_pSprite[WEAPON_UP_SPRITE]->load(&g_cUpsSprites,0,1,1)) return FALSE;
		if(!g_pSprite[WEAPON_UP_SPRITE]->load(&g_cUpsSprites,1,34,1)) return FALSE;
	}
	else return FALSE;
	if(g_pSprite[SPEED_UP_SPRITE]->Restore())
	{
		if(!g_pSprite[SPEED_UP_SPRITE]->load(&g_cUpsSprites,0,1,30)) return FALSE;
		if(!g_pSprite[SPEED_UP_SPRITE]->load(&g_cUpsSprites,1,34,30)) return FALSE;
	}
	else return FALSE;
	if(g_pSprite[LIFE_UP_SPRITE]->Restore())
	{
		if(!g_pSprite[LIFE_UP_SPRITE]->load(&g_cUpsSprites,0,1,59)) return FALSE;
		if(!g_pSprite[LIFE_UP_SPRITE]->load(&g_cUpsSprites,1,34,59)) return FALSE;
	}

	//restore default fire
	if(g_pSprite[DEFAULT_FIRE_SPRITE]->Restore())
	{
		if(!g_pSprite[DEFAULT_FIRE_SPRITE]->load(&g_cDefaultFire,0,0,0)) return FALSE;
	}
	else return FALSE;
	
	//restore ship bomb
	if(g_pSprite[SHIP_BOMB_SPRITE]->Restore())
	{
		if(!g_pSprite[SHIP_BOMB_SPRITE]->load(&g_cShipBombSprites,0,1,1)) return FALSE;
		if(!g_pSprite[SHIP_BOMB_SPRITE]->load(&g_cShipBombSprites,1,20,1)) return FALSE;
	}
	else return FALSE;
	
	//restore slith
	if(g_pSprite[BLUE_SLITH_SPRITE]->Restore())
	{
		if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,0,1,1)) return FALSE;
		if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,1,34,1)) return FALSE;
		if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,2,67,1)) return FALSE;
		if(!g_pSprite[BLUE_SLITH_SPRITE]->load(&g_cSlithSprites,3,100,1)) return FALSE;
	}
	else return FALSE;
	if(g_pSprite[GOLD_SLITH_SPRITE]->Restore())
	{
		if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,0,1,34)) return FALSE;
		if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,1,34,34)) return FALSE;
		if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,2,67,34)) return FALSE;
		if(!g_pSprite[GOLD_SLITH_SPRITE]->load(&g_cSlithSprites,3,100,34)) return FALSE;
	}
	else return FALSE;
	if(g_pSprite[SLITH_DYING_SPRITE]->Restore())
	{
		if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,0,1,67)) return FALSE;
		if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,1,34,67)) return FALSE;
		if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,2,67,67)) return FALSE;
		if(!g_pSprite[SLITH_DYING_SPRITE]->load(&g_cSlithSprites,3,100,67)) return FALSE;
	}
	else return FALSE;

	//restore spark
	if(g_pSprite[SPARK_SPRITE]->Restore())
	{
		if(!g_pSprite[SPARK_SPRITE]->load(&g_cSparkSprites,0,1,1)) return FALSE;
		if(!g_pSprite[SPARK_SPRITE]->load(&g_cSparkSprites,1,26,1)) return FALSE;
		if(!g_pSprite[SPARK_SPRITE]->load(&g_cSparkSprites,2,51,1)) return FALSE;
	}
	else return FALSE; 
	if(g_pSprite[SPARK_DYING_SPRITE]->Restore())
	{
		if(!g_pSprite[SPARK_DYING_SPRITE]->load(&g_cSparkSprites,0,1,26)) return FALSE;
		if(!g_pSprite[SPARK_DYING_SPRITE]->load(&g_cSparkSprites,1,26,26)) return FALSE;
		if(!g_pSprite[SPARK_DYING_SPRITE]->load(&g_cSparkSprites,2,51,26)) return FALSE;
	}
	else return FALSE; 
	
	return result;
}
