#ifndef BLOCKDUDE_H
#define BLOCKDUDE_H

#include <SDL_keysym.h>
#include <SDL.h>

enum GameStates {GSTitleScreen,GSCredits,GSQuit,GSGame,GSStageClear,GSStageSelect,GSLevelEditor,GSLevelEditorMenu};

extern SDL_Surface *IMGBackground,*IMGFloor,*IMGPlayer,*IMGBox,*IMGEmpty,*IMGTitleScreen,*IMGExit,*IMGFloorLeft,*IMGFloorRight,
            *IMGEarthGrassRight,*IMGEarthGrassLeft,*IMGEarthLeft,*IMGEarthRight,*IMGEarthMiddle,*IMGFloatingFloor,*IMGFloatingFloorLeft,
            *IMGFloatingFloorRight,*IMGFloatingFloorMiddle,*IMGTower,*IMGStartTower,*IMGTowerShaft,*IMGRoof1,*IMGRoof2,*IMGRoofCornerLeft,
            *IMGRoofCornerRight,*IMGRoofDownRight,*IMGRoofDownLeft,*IMGRoofCornerBoth,*IMGGrid,*IMGLayer2;

typedef struct SPoint SPoint;
struct SPoint
{
	int X,Y;
};

constexpr int SMOOTHING = 0,
          WINDOW_WIDTH = 240,
          WINDOW_HEIGHT = 240,
   		  TileWidth = 16,
          TileHeight = 16,
	      AlphaInc =20,
          NrOfRows = 50,
		  NrOfCols = 50,
		  NrOfRowsVisible = (WINDOW_HEIGHT / TileHeight),
    	  NrOfColsVisible = (WINDOW_WIDTH / TileWidth) ,
		  MaxHistory = 1000,
		  IDEmpty = 1,
		  IDPlayer = 2,
		  IDBox = 3,
          IDFloor = 4,
          IDExit = 5,
          IDEarthGrassLeft = 6,
          IDEarthGrassRight = 7,
          IDEarthLeft =8,
          IDEarthMiddle =9,
          IDEarthRight = 10,
          IDFloatingFloor = 11,
          IDFloatingFloorLeft = 12,
          IDFloatingFloorMiddle = 13,
          IDFloatingFloorRight = 14,
          IDFloorLeft = 15,
          IDFloorRight=16,
          IDTower = 17,
          IDStartTower = 18,
          IDTowerShaft = 19,
          IDRoof1 = 20,
          IDRoof2 = 21,
          IDRoofCornerLeft = 22,
          IDRoofCornerRight = 23,
          IDRoofCornerBoth = 24,
          IDRoofDownRight = 25,
          IDRoofDownLeft = 26,
          GroupFloor = 1,
		  GroupPlayer = 2,
		  GroupBox = 3,
		  GroupExit = 4,
          ZPlayer= 10, //has to be drawn last + It has to be moved as last also for the logic to work. (otherwise a box can fall down when a player jumps)
          ZWall = 5,
          ZBox = 5,
          ZFloor = 5,
          ZExit=1,
          FPS = 40,
          InputDelay = 13,
		  MaxSkins = 100,
		  MaxLevelPacks=200,
		  MaxMusicFiles=26,
		  NrOfSounds=4,
		  SND_MENU=0,
		  SND_SELECT=1,
		  SND_ERROR=2,
		  SND_STAGEEND=3,
		  MaxAlpha=150,
		  AnimBaseLeft = 0,
		  AnimBaseRight = 4,
		  AnimBaseLeftJump = 8,
		  AnimBaseRightJump= 12;


#ifdef FUNKEY

#define BUT_UP SDLK_u
#define BUT_RIGHT SDLK_r
#define BUT_DOWN SDLK_d
#define BUT_LEFT SDLK_l
#define BUT_A SDLK_a
#define BUT_X SDLK_x
#define BUT_Y SDLK_y
#define BUT_B SDLK_b
#define BUT_START SDLK_s
#define BUT_SELECT SDLK_k
#define BUT_L SDLK_m
#define BUT_R SDLK_n

#else

#define BUT_UP SDLK_UP
#define BUT_RIGHT SDLK_RIGHT
#define BUT_DOWN SDLK_DOWN
#define BUT_LEFT SDLK_LEFT
#define BUT_A SDLK_SPACE
#define BUT_X SDLK_x
#define BUT_Y SDLK_y
#define BUT_B SDLK_LCTRL
#define BUT_START SDLK_RETURN
#define BUT_SELECT SDLK_ESCAPE
#define BUT_L SDLK_l
#define BUT_R SDLK_r


#endif

#endif