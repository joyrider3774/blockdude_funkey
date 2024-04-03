//BUG IN LEVELEDITOR
//stageselector naar rechts duwen, dan nieuwe level maken -> er is een player toegevoegd geweest en selector staat mis !

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <vector>
#include <SDL_keysym.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_framerate.h>
#include <SDL_keysym.h>
#include "CInput.h"
#include "blockdude.h"
#include "Functions.h"
#include "CViewPort.h"
#include "CWorldParts.h"
#include "CWorldPart.h"
#include "CSelector.h"

#ifdef WIN32
    #include <dir.h>
#endif

using namespace std;

SDL_Surface *IMGBackground=NULL,*IMGFloor=NULL,*IMGPlayer=NULL,*IMGBox=NULL,*IMGEmpty=NULL,*IMGTitleScreen=NULL,*IMGExit=NULL,*IMGFloorLeft=NULL,*IMGFloorRight=NULL,
            *IMGEarthGrassRight=NULL,*IMGEarthGrassLeft=NULL,*IMGEarthLeft=NULL,*IMGEarthRight=NULL,*IMGEarthMiddle=NULL,*IMGFloatingFloor=NULL,*IMGFloatingFloorLeft=NULL,
            *IMGFloatingFloorRight=NULL,*IMGFloatingFloorMiddle=NULL,*IMGTower=NULL,*IMGStartTower=NULL,*IMGTowerShaft=NULL,*IMGRoof1=NULL,*IMGRoof2=NULL,*IMGRoofCornerLeft=NULL,
            *IMGRoofCornerRight=NULL,*IMGRoofDownRight=NULL,*IMGRoofDownLeft=NULL,*IMGRoofCornerBoth=NULL,*IMGGrid=NULL,*IMGLayer2=NULL;

unsigned char HashTable[1004] = {0xE4,0xF6,0xD7,0xA4,0xA5,0xC3,0xA3,0xE5,0xA4,0xF8,0xA3,0xF6,0xB5,0x98,0xA3,0xA7,0xB9,0xA0,0xD9,0xD6,0xE6,0x29,0x49,0xF8,0xB9,0xF0,0x79,0xA6,0x88,0xA8,0xC6,0xC2,0xF2,0xB7,0x79,0xC0,0xF0,0xD0,0xA8,0xE1,0xE1,0xF9,0xA5,0xE5,0xF1,0xE4,0xC7,0xE8,0xD9,0xB7,0xC0,0xE1,0xC0,0xB4,0xA9,0xF8,0xF8,0xF8,0xA1,0xA4,0xD0,0xC6,0x06,0xB9,0xA1,0xB0,0xF0,0xE8,0xB0,0xE5,0xC3,0xC8,0xA1,0xB2,0xF1,0xE2,0xE0,0xF0,0xF8,0xA5,0xE6,0xF2,0xB4,0xF7,0x79,0xB5,0x75,0xB0,0xF1,0xA6,0xC9,0xE2,0xB6,0x68,0xD9,0xA0,0xE7,0xD1,0xB4,0x51,0xD9,0xE9,0x96,0x61,0xD2,0xC2,0xC2,0x06,0x47,0x86,0x88,0xF4,0xC7,0x88,0xA0,0xA9,0xB1,0xD2,0xE9,0xF1,0x93,0xF4,0x35,0xC6,0xB2,0xC8,0xB8,0xB0,0xA8,0xE9,0xC1,0xA4,0xF5,0xD6,0xD1,0xC8,0xD3,0xA9,0xE9,0xA9,0xF4,0xB4,0xA3,0x57,0xE1,0xC5,0xF1,0xB7,0xB6,0xB2,0xE3,0x45,0x64,0xE3,0xD9,0xE5,0xB5,0xE2,0xE8,0xC8,0xB5,0xE7,0xD3,0xB0,0xD2,0xF7,0x81,0xB2,0xE4,0xA8,0x00,0xC9,0xD5,0xD4,0x86,0xC9,0xB1,0xA2,0xA0,0xD3,0xE3,0xD5,0xF7,0xE0,0xF9,0xE5,0xE2,0xD1,0xA5,0xD4,0xF4,0xC6,0xB4,0xD3,0xA2,0xD6,0xD3,0x35,0xE4,0xE3,0xC4,0xB6,0x18,0xD7,0xE7,0xC5,0xE2,0x59,0xA0,0xB5,0xA7,0xC5,0xF6,0xA7,0xC2,0xD7,0xA2,0xB5,0xB9,0xF6,0x81,0xA3,0xA5,0x00,0xA2,0xA8,0xE7,0xD5,0x87,0xC8,0xE9,0xD7,0xE6,0xA0,0x54,0xF9,0xF9,0xF4,0xB4,0xF4,0xE9,0xA7,0xD0,0x74,0xD0,0xE7,0x24,0xD9,0xB0,0xE8,0xE3,0xF1,0xF7,0xF7,0x88,0x97,0xC0,0xE0,0xD2,0xF3,0xC2,0xA1,0xC4,0xE1,0xF8,0xE8,0xB1,0x76,0x97,0xE7,0xB6,0x96,0xA7,0xD8,0xA4,0xF1,0xA0,0xF3,0xE8,0xE7,0x76,0xE3,0xB1,0xC9,0xE4,0xB4,0xF0,0xA4,0xD0,0x28,0xB8,0xF7,0xD9,0xD3,0xE5,0xC9,0x81,0xF7,0xC0,0xD9,0xC9,0xE3,0xA8,0xD6,0xB9,0xA7,0xF9,0xD0,0xD8,0x54,0xF6,0xF9,0xB8,0xC8,0xC9,0xB9,0xF0,0xF4,0xD1,0xA4,0xD0,0xE6,0xA8,0xB6,0xD2,0xB0,0xE9,0xE9,0xE9,0xB7,0xD9,0xD4,0xE6,0xF0,0xA4,0xC7,0xF1,0xC0,0xC2,0xA8,0xA7,0x14,0x25,0xA1,0xB6,0xB9,0xD5,0x78,0xC1,0xD1,0xC6,0x93,0xE8,0xE7,0xE7,0xA9,0xA2,0xA8,0x89,0xA1,0xA6,0xF9,0xA8,0xF7,0xE4,0xD8,0xA2,0xD2,0xB0,0x39,0xD1,0xC5,0x72,0xB0,0xD6,0xC6,0xA0,0xE9,0xE5,0xC1,0xC2,0xF9,0xF3,0xB2,0xC8,0xD0,0xD2,0xD1,0xC1,0xD7,0x53,0x93,0xB4,0xF9,0xB0,0xD0,0xD4,0xF5,0xE4,0xE4,0x13,0xB2,0xE9,0x70,0xB3,0xB2,0xD8,0xF0,0xC0,0xC1,0xB0,0xE8,0xA3,0xE7,0x33,0xB1,0xC0,0xA7,0x17,0xD4,0xA9,0xE5,0xF6,0xF3,0xF3,0xA3,0xE0,0xD5,0xE2,0x71,0xB7,0xB4,0xA9,0xD6,0xC1,0xC5,0xA3,0xB2,0xD7,0xB6,0xE8,0xC5,0xF0,0x10,0xA6,0xB0,0xC3,0xA3,0xA0,0xB2,0xF6,0xB9,0xF4,0xD8,0xD5,0xF8,0xB9,0xE3,0x70,0xD1,0xF1,0xA6,0xE6,0xE5,0xA8,0xD5,0xC9,0xC1,0xA5,0xE5,0x61,0xF4,0xB2,0xA9,0xA4,0xF2,0xE8,0xB7,0xD7,0xA1,0xD0,0xF4,0xF9,0xE4,0xE8,0xC6,0xB3,0xC4,0xB2,0xC9,0xE3,0x49,0xC3,0x62,0xA6,0xF3,0xB0,0x57,0x65,0xC4,0xF6,0xB9,0x23,0xB8,0xB3,0xA9,0xE6,0xB7,0xA5,0xE8,0xF6,0xC9,0xE7,0x63,0xF7,0xD9,0xA5,0xA5,0xD2,0xD4,0xA6,0xF2,0x94,0x04,0xB6,0x85,0xA3,0xD3,0x59,0xA3,0xA8,0x69,0xF1,0xB9,0xD1,0xD1,0xE5,0xE5,0x57,0xB0,0xD5,0x59,0xD4,0xD9,0x52,0xB4,0xA8,0x75,0xF0,0xE0,0x20,0xA1,0xE7,0x69,0xA2,0xC0,0xB1,0xA3,0xE5,0xB9,0xF5,0xB9,0xB5,0xE1,0xF3,0xB7,0x56,0xA1,0xA4,0xA9,0xA4,0xE7,0xC5,0xC7,0xC7,0x32,0xB7,0xA6,0xB4,0xB2,0xF6,0xB1,0xC4,0xB5,0xC4,0xC1,0xE1,0x97,0xB1,0xE3,0x37,0xA6,0xD8,0xC3,0xF1,0xD5,0xE9,0xF2,0xE2,0xF6,0xF3,0xF6,0xF4,0x51,0xE7,0xE9,0xC1,0xE8,0xA8,0x69,0xD3,0xB4,0xD8,0xD6,0xE8,0xD8,0x47,0xE1,0xA8,0xA0,0x94,0xB5,0xC2,0xD3,0xF6,0x28,0xF5,0x21,0xD7,0x48,0xC4,0xD8,0xF2,0xC7,0xC0,0xA7,0xF6,0xC7,0xE7,0x64,0xB5,0xD5,0x97,0xA0,0xD6,0xF7,0xC4,0xF7,0xD0,0xD7,0xA8,0xD7,0x28,0xC2,0xE5,0xD0,0xC2,0xF2,0xC8,0xF9,0xE5,0x08,0xD3,0xA0,0xA0,0xB2,0xD3,0x29,0xF0,0xB1,0xD0,0xB2,0xD7,0xF1,0x47,0xF2,0xD4,0xD3,0x53,0x92,0xC4,0xF9,0xA2,0xE8,0xC3,0xA9,0xF8,0xB1,0xE4,0x90,0xD0,0xE4,0x26,0x18,0xB4,0xF8,0xE7,0xC7,0xE0,0xF9,0xC1,0x36,0xE3,0xC2,0xE6,0xC9,0xC2,0xF8,0xA1,0x26,0xE7,0xC1,0x57,0xE9,0xD1,0xC8,0xC3,0xB2,0xE1,0x86,0xA5,0xE8,0xB4,0xA3,0xB5,0xD7,0x89,0xD5,0xF7,0xD0,0xD6,0xD7,0xB4,0x41,0xE1,0xA2,0xD5,0xF1,0xA8,0xF5,0xF7,0xC8,0xA3,0xB1,0xC0,0xC7,0x83,0x65,0xE6,0x46,0xA8,0xA3,0xE8,0xC5,0xF9,0xC2,0xD6,0xA5,0xD5,0xA5,0xA8,0xB6,0xE0,0xD0,0xC4,0xE1,0xF0,0x08,0x06,0x45,0xA7,0xF1,0xA1,0xE8,0xF3,0xF1,0xE2,0xC4,0xA6,0xF0,0xF3,0xD3,0xB2,0xE5,0xC5,0x85,0xA6,0xC7,0xD5,0xD4,0xD7,0xC2,0xC8,0xC8,0xD2,0xB8,0xA4,0x79,0x06,0xF7,0xE0,0xB6,0xF4,0xF4,0xC2,0xD1,0xB4,0xA4,0xB5,0xA8,0xC0,0xB2,0xB9,0xF7,0xC0,0xD6,0xD6,0xC3,0xF5,0xA3,0xF5,0xA7,0xE5,0xE1,0xB0,0xA5,0xE0,0xC3,0x41,0xD3,0xB2,0xF9,0xF2,0x19,0xB3,0xA7,0x58,0x40,0xB1,0x86,0xD5,0x71,0xF8,0xF6,0xD2,0xF2,0xA5,0xF4,0xB7,0x80,0xE5,0x03,0xF8,0x14,0xF5,0xE4,0xE7,0xE2,0xB9,0xA6,0xB5,0xE2,0xB5,0xD8,0xC7,0xE5,0xC5,0xF9,0xD4,0xD3,0xE5,0xD9,0xE8,0x79,0xD2,0xE3,0xF0,0xA1,0xF8,0xD8,0x31,0x78,0xB9,0xA9,0x41,0xC1,0xB1,0xC6,0x52,0xC5,0xA7,0xD0,0xF9,0xF5,0xA8,0xF6,0xA3,0xA3,0xD0,0xA5,0xF3,0xB1,0xB1,0x31,0xE0,0xC5,0xA7,0x29,0xC0,0xE1,0xD4,0xF7,0x15,0xC2,0xF3,0xA1,0xC9,0xF3,0xE0,0xA5,0xB6,0xE6,0xE5,0xB1,0xA3,0xB3,0xE7,0xF5,0xD8,0x93,0xF8,0xB2,0x94,0xC8,0xD2,0xE2,0xB0,0xF9,0xF1,0xB4,0xE0,0xD5,0x65,0x81,0xC4,0xC7,0xD0,0xE8,0xA7,0xA2,0xF7,0x45,0xA0,0xE9,0xC5,0xB6,0xE2,0xF2,0xE6,0xA7,0xE8,0x28,0xA6,0xE5,0x90,0xF0,0xD6,0xE1,0xC4,0xD2,0xD5,0xF0,0xA5,0xB1,0xC6,0xA0,0xC1,0xF5,0xB1,0xA4,0xD3,0xF3,0xA3,0xF3,0x10,0xE8,0xA9,0xC1,0xB2,0xB8,0xA5,0xD4,0x41,0xB4,0xC6,0xF7,0xC6,0xF9,0xD9,0xD0,0xC2,0xE9,0xD5,0xC7};
SDL_Surface* Screen;
TTF_Font* font,* BigFont,*MonoFont,*BigFontSkin;
SDL_Joystick *Joystick;
GameStates GameState = GSTitleScreen;
int InstalledLevelPacksCount=0,InstalledLevels=0,SelectedLevel=0,SelectedLevelPack=0,UnlockedLevels=1;
bool TvOutMode=false,GlobalSoundEnabled = true,LevelEditorMode=false,LevelHasChanged=false,StageReload=false;
SDL_Color MenuBoxColor = {79,80,67,255}, MenuBoxBorderColor = {204,224,207,255}, MenuTextColor = {79,106,7,0};
CWorldParts WorldParts;
Uint32 NextTime=0;
int Volume = 128,InstalledSkinsCount=0,SelectedSkin=-1;
char LevelPackName[21]= "";
char LevelPackFileName[21] = "";
char InstalledLevelPacks[MaxLevelPacks][21];
Mix_Music *Music[MaxMusicFiles];
Mix_Chunk *Sounds[NrOfSounds];
char InstalledSkins[MaxSkins][FILENAME_MAX];
char SkinName[FILENAME_MAX];
FPSmanager Fpsman;

void LoadSettings()
{
    int Teller;
	FILE *Fp;
	char Filename[512];
	sprintf(Filename, "%s/.blockdude_settings.dat", getenv("HOME") == NULL ? ".": getenv("HOME"));
	Fp = fopen(Filename,"rt");
	if (Fp)
	{
        fscanf(Fp,"Skin=%s\n",&SkinName[0]);
        for(Teller=0;Teller<InstalledSkinsCount;Teller++)
        {
            if (strcmp(InstalledSkins[Teller],SkinName) == 0)
            {
                SelectedSkin = Teller;
                break;
            }
            else
                SelectedSkin = -1;
        }
		fclose(Fp);
	}
}

void SaveSettings()
{
	FILE *Fp;
	char Filename[512];
	sprintf(Filename, "%s/.blockdude_settings.dat", getenv("HOME") == NULL ? ".": getenv("HOME"));
	Fp = fopen(Filename,"wt");
	if (Fp)
	{
        fprintf(Fp,"Skin=%s\n",SkinName);
		fclose(Fp);
	}
}

void SearchForSkins()
{
	struct dirent *Entry;
	DIR *Directory;
	struct stat Stats;
	int Teller=0;
	char FileName[FILENAME_MAX];
	Directory = opendir("./blockdudefs/skins");
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"./blockdudefs/skins/%s",Entry->d_name);
			stat(FileName,&Stats);
			if(S_ISDIR(Stats.st_mode))
			{
				if(strncmp(".", Entry->d_name, 1)  && (Teller< MaxSkins) && (strlen(Entry->d_name) < FILENAME_MAX-1))
				{
					sprintf(InstalledSkins[Teller],"%s",Entry->d_name);
					Teller++;
				}
			}
			Entry=readdir(Directory);
		}
		closedir(Directory);
	}
	InstalledSkinsCount = Teller;
	SelectedSkin=-1;
}

void UnLoadGraphics()
{
    if(IMGLayer2)
	{
        SDL_FreeSurface(IMGLayer2);
    }
	if(IMGGrid)
	{
        SDL_FreeSurface(IMGGrid);
	}
	if(IMGExit)
	{
        SDL_FreeSurface(IMGExit);
	}
	if(IMGBackground)
	{
		SDL_FreeSurface(IMGBackground);
	}
	if(IMGFloor)
	{
		SDL_FreeSurface(IMGFloor);
	}
	if(IMGPlayer)
	{
		SDL_FreeSurface(IMGPlayer);
	}
	if(IMGBox)
	{
		SDL_FreeSurface(IMGBox);
	}
	if(IMGEmpty)
	{
		SDL_FreeSurface(IMGEmpty);
	}
	if(IMGTitleScreen)
	{
		SDL_FreeSurface(IMGTitleScreen);
    }
	if(IMGFloorLeft)
	{
        SDL_FreeSurface(IMGFloorLeft);
    }
	if(IMGFloorRight)
	{
        SDL_FreeSurface(IMGFloorRight);
    }
	if(IMGEarthGrassRight)
	{
        SDL_FreeSurface(IMGEarthGrassRight);
    }
	if(IMGEarthGrassLeft)
	{
        SDL_FreeSurface(IMGEarthGrassLeft);
    }
	if(IMGEarthLeft)
	{
        SDL_FreeSurface(IMGEarthLeft);
    }
	if(IMGEarthRight)
	{
        SDL_FreeSurface(IMGEarthRight);
    }
	if(IMGEarthMiddle)
	{
        SDL_FreeSurface(IMGEarthMiddle);
    }
	if(IMGFloatingFloor)
	{
        SDL_FreeSurface(IMGFloatingFloor);
    }
	if(IMGFloatingFloorLeft)
	{
        SDL_FreeSurface(IMGFloatingFloorLeft);
    }
	if(IMGFloatingFloorRight)
	{
        SDL_FreeSurface(IMGFloatingFloorRight);
    }
	if(IMGFloatingFloorMiddle)
	{
        SDL_FreeSurface(IMGFloatingFloorMiddle);
    }
	if(IMGTower)
	{
        SDL_FreeSurface(IMGTower);
    }
	if(IMGStartTower)
	{
        SDL_FreeSurface(IMGStartTower);
    }
	if(IMGTowerShaft)
	{
        SDL_FreeSurface(IMGTowerShaft);
    }
	if(IMGRoof1)
	{
        SDL_FreeSurface(IMGRoof1);
    }
	if(IMGRoof2)
	{
        SDL_FreeSurface(IMGRoof2);
    }
	if(IMGRoofCornerLeft)
	{
        SDL_FreeSurface(IMGRoofCornerLeft);
    }
	if(IMGRoofCornerRight)
	{
        SDL_FreeSurface(IMGRoofCornerRight);
    }
	if(IMGRoofDownRight)
	{
        SDL_FreeSurface(IMGRoofDownRight);
    }
	if(IMGRoofDownLeft)
	{
        SDL_FreeSurface(IMGRoofDownLeft);
    }
	if(IMGRoofCornerBoth)
	{
        SDL_FreeSurface(IMGRoofCornerBoth);
    }
}


void LoadGraphics()
{
	FILE *ColorsFile;
	SDL_Surface *Tmp;
	int R,G,B,A;
	char FileName[FILENAME_MAX];

    UnLoadGraphics();
    
    sprintf(FileName,"./blockdudefs/skins/%s/grid.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/grid.png");
    IMGGrid = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGGrid, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGGrid->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/layer2.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/layer2.png");
    IMGLayer2 = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGLayer2, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGGrid->format,255,0,255));
 
    sprintf(FileName,"./blockdudefs/skins/%s/roof1.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/roof1.png");
    IMGRoof1 = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGRoof1, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGRoof1->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/roof2.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/roof2.png");
    IMGRoof2 = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGRoof2, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGRoof2->format,255,0,255));
 
    sprintf(FileName,"./blockdudefs/skins/%s/roofcornerleft.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/roofcornerleft.png");
    IMGRoofCornerLeft = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGRoofCornerLeft, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGRoofCornerLeft->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/roofcornerright.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/roofcornerright.png");
    IMGRoofCornerRight = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGRoofCornerRight, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGRoofCornerRight->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/roofdownright.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/roofdownright.png");
    IMGRoofDownRight = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGRoofDownRight, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGRoofDownRight->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/roofdownleft.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/roofdownleft.png");
    IMGRoofDownLeft = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGRoofDownLeft, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGRoofDownLeft->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/roofcornerboth.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/roofcornerboth.png");
    IMGRoofCornerBoth = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGRoofCornerBoth, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGRoofCornerBoth->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/door.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/door.png");
    IMGExit = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGExit, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGExit->format,255,0,255));


    sprintf(FileName,"./blockdudefs/skins/%s/floor.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/floor.png");
    IMGFloor = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloor, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloor->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/box.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/box.png");
    IMGBox = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGBox, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGBox->format,255,0,255));

	sprintf(FileName,"./blockdudefs/skins/%s/player.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/player.png");
    IMGPlayer = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGPlayer, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGPlayer->format,255,0,255));

	sprintf(FileName,"./blockdudefs/skins/%s/empty.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/empty.png");
    IMGEmpty = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGEmpty, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGEmpty->format,255,0,255));


    sprintf(FileName,"./blockdudefs/skins/%s/floorleft.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/floorleft.png");
    IMGFloorLeft = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloorLeft, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloorLeft->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/floorright.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/floorright.png");
    IMGFloorRight = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloorRight, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloorRight->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/earthgrassright.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/earthgrassright.png");
    IMGEarthGrassRight = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGEarthGrassRight, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGEarthGrassRight->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/earthgrassleft.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/earthgrassleft.png");
    IMGEarthGrassLeft = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGEarthGrassLeft, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGEarthGrassLeft->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/earthleft.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/earthleft.png");
    IMGEarthLeft = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGEarthLeft, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGEarthLeft->format,255,0,255));


    sprintf(FileName,"./blockdudefs/skins/%s/earthright.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/earthright.png");
    IMGEarthRight = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGEarthRight, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGEarthRight->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/earthmiddle.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/earthmiddle.png");
    IMGEarthMiddle = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGEarthMiddle, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGEarthMiddle->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/floatingfloor.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/floatingfloor.png");
    IMGFloatingFloor = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloatingFloor, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloatingFloor->format,255,0,255));


    sprintf(FileName,"./blockdudefs/skins/%s/floatingfloorleft.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/floatingfloorleft.png");
    IMGFloatingFloorLeft = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloatingFloorLeft, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloatingFloorLeft->format,255,0,255));


    sprintf(FileName,"./blockdudefs/skins/%s/floatingfloorright.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/floatingfloorright.png");
    IMGFloatingFloorRight = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloatingFloorRight, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloatingFloorRight->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/floatingfloormiddle.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/floatingfloormiddle.png");
    IMGFloatingFloorMiddle = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloatingFloorMiddle, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloatingFloorMiddle->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/tower.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/tower.png");
    IMGTower = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGTower, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGTower->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/starttower.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/starttower.png");
    IMGStartTower = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGStartTower, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGStartTower->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/towershaft.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/towershaft.png");
    IMGTowerShaft = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGTowerShaft, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGTowerShaft->format,255,0,255));

    sprintf(FileName,"./blockdudefs/skins/%s/titlescreen.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/titlescreen.png");
    IMGTitleScreen = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);

    sprintf(FileName,"./blockdudefs/skins/%s/background.png",SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
        Tmp = IMG_Load("./blockdudefs/graphics/background.png");
    IMGBackground = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);

	sprintf(FileName,"./blockdudefs/skins/%s/colors.txt",SkinName);
	ColorsFile = fopen(FileName,"rt");
	if (ColorsFile)
	{
		fscanf(ColorsFile,"[TextColor]\nR=%d\nG=%d\nB=%d\n",&R,&G,&B);
		MenuTextColor.r = R;
		MenuTextColor.g = G;
		MenuTextColor.b = B;
		fscanf(ColorsFile,"[MenuBoxColor]\nR=%d\nG=%d\nB=%d\nA=%d\n",&R,&G,&B,&A);
		MenuBoxColor.r = R;
		MenuBoxColor.g = G;
		MenuBoxColor.b = B;
		MenuBoxColor.unused = A;
		fscanf(ColorsFile,"[MenuBoxBorderColor]\nR=%d\nG=%d\nB=%d\nA=%d\n",&R,&G,&B,&A);
		MenuBoxBorderColor.r = R;
		MenuBoxBorderColor.g = G;
		MenuBoxBorderColor.b = B;
		MenuBoxBorderColor.unused = A;
		fclose(ColorsFile);
	}
	else
	{
		MenuTextColor.r = 104;
		MenuTextColor.g = 124;
		MenuTextColor.b = 107;
		MenuBoxColor.r = 204;
		MenuBoxColor.g = 224;
		MenuBoxColor.b = 207;
		MenuBoxColor.unused = 255;
		MenuBoxBorderColor.r = 145;
		MenuBoxBorderColor.g = 166;
		MenuBoxBorderColor.b = 166;
		MenuBoxBorderColor.unused = 255;
	}
}


void NextSkin()
{
    char Text[500];
    SDL_Color Color = {255,255,255,255};
    if(InstalledSkinsCount > 0)
    {
        SelectedSkin++;
        if (SelectedSkin >=InstalledSkinsCount)
            SelectedSkin = -1;
        if(SelectedSkin == -1)
        {
            sprintf(SkinName,"%s","");
            sprintf(Text,"Please Wait Loading Skin:\nDefault Skin");
        }
        else
        {
            sprintf(SkinName,"%s",InstalledSkins[SelectedSkin]);
            sprintf(Text,"Please Wait Loading Skin:\n%s",SkinName);
        }
        boxRGBA(Screen,0,0,WINDOW_WIDTH-1,48,255,255,255,200);
        boxRGBA(Screen,1,1,WINDOW_WIDTH-2,47,0,0,0,200);
        WriteText(Screen,BigFontSkin,Text,strlen(Text),5,0,0,Color);
        SDL_Flip(Screen);
        LoadGraphics();
    }
}

void PreviousSkin()
{
    char Text[500];
    SDL_Color Color = {255,255,255,255};
    if(InstalledSkinsCount > 0)
    {
        SelectedSkin--;
        if (SelectedSkin <=-2)
            SelectedSkin = InstalledSkinsCount-1;
        if(SelectedSkin == -1)
        {
            sprintf(SkinName,"%s","");
            sprintf(Text,"Please Wait Loading Skin:\nDefault Skin");
        }
        else
        {
            sprintf(SkinName,"%s",InstalledSkins[SelectedSkin]);
            sprintf(Text,"Please Wait Loading Skin:\n%s",SkinName);
        }
        boxRGBA(Screen,0,0,WINDOW_WIDTH-1,48,255,255,255,200);
        boxRGBA(Screen,1,1,WINDOW_WIDTH-2,47,0,0,0,200);
        WriteText(Screen,BigFontSkin,Text,strlen(Text),5,0,0,Color);
        SDL_Flip(Screen);
        LoadGraphics();
    }
}

void SearchForLevelPacks()
{
	struct dirent *Entry;
	DIR *Directory;
	struct stat Stats;
	int Teller=0;
	char FileName[FILENAME_MAX];
	Directory = opendir("./blockdudefs/levelpacks");
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"./blockdudefs/levelpacks/%s",Entry->d_name);
			stat(FileName,&Stats);
			if(S_ISDIR(Stats.st_mode))
			{
				if(strncmp(".", Entry->d_name, 1)  && (Teller< MaxLevelPacks) && (strlen(Entry->d_name) < 21))
				{
					sprintf(InstalledLevelPacks[Teller],"%s",Entry->d_name);
					RemoveUnderScores(InstalledLevelPacks[Teller]);
					Teller++;
				}
			}
			Entry=readdir(Directory);
		}
		closedir(Directory);
	}
	InstalledLevelPacksCount = Teller;
	SelectedLevelPack=0;
	if (InstalledLevelPacksCount > 0)
	{
		sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
		sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
		AddUnderScores(LevelPackFileName);

	}
	else
	{
		sprintf(LevelPackName,"%s","");
		sprintf(LevelPackFileName,"%s","");
	}
}

bool StageDone(CPlayer *Player)
{
	bool Temp = false;
	int Teller;
	for (Teller=0;Teller<WorldParts.ItemCount;Teller++)
		if (WorldParts.Items[Teller]->GetType() == IDExit)
		{
			if((WorldParts.Items[Teller]->GetPlayFieldX() == Player->GetPlayFieldX()) && (WorldParts.Items[Teller]->GetPlayFieldY() == Player->GetPlayFieldY()))
                Temp = true;
                break;
		}
    return Temp;
}

bool AskQuestion(char *Msg)
{
	CInput *Input = new CInput(InputDelay);
	boxRGBA(Screen,20,70,220,165,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
	rectangleRGBA(Screen,20,70,220,165,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	rectangleRGBA(Screen,21,71,219,164,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	WriteText(Screen,font,Msg,strlen(Msg),25,80,2,MenuTextColor);
    SDL_Flip(Screen);
	{
		while (!( Input->SpecialsHeld[SPECIAL_QUIT_EV] || Input->KeyboardHeld[BUT_A] || Input->KeyboardHeld[BUT_B]))
		{
		    Input->Update();
			if(GlobalSoundEnabled)
			if(!Mix_PlayingMusic())
			{
				Mix_PlayMusic(Music[0],0);
			}
			SDL_framerateDelay(&Fpsman);
		}
		if (Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;
		if (Input->KeyboardHeld[BUT_A])
			return true;
		else
			return false;

	}
	delete Input;
}

void PrintForm(char *msg)
{
    CInput *Input = new CInput(InputDelay);
	boxRGBA(Screen,20,70,220,165,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
	rectangleRGBA(Screen,20,70,220,165,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	rectangleRGBA(Screen,21,71,219,164,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	WriteText(Screen,font,msg,strlen(msg),22,80,2,MenuTextColor);
    SDL_Flip(Screen);
    while (!( Input->SpecialsHeld[SPECIAL_QUIT_EV] || Input->KeyboardHeld[BUT_A]))
    {
        Input->Update();
        if(GlobalSoundEnabled)
        if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(Music[0],0);
        }
        SDL_framerateDelay(&Fpsman);
    }

	delete Input;
}

void SaveUnlockData()
{
	FILE *Fp;
	int Teller;
	char Filename[FILENAME_MAX];
	unsigned char LevelHash[4];
	unsigned char HashBuffer[64];
	int CheckSum = 0;
	LevelHash[0] = HashTable[UnlockedLevels-1] ;
	LevelHash[1] = HashTable[UnlockedLevels];
	LevelHash[2] = HashTable[UnlockedLevels+1];
	LevelHash[3] = HashTable[UnlockedLevels+2];
	sprintf(Filename,"%s/.blockdude_%s.dat", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
	for (Teller=0;Teller<4;Teller++)
		LevelHash[Teller] = LevelHash[Teller] ^ LevelPackFileName[strlen(LevelPackFileName)-1];
	for (size_t i =0;i<strlen(LevelPackFileName);i++)
		LevelHash[i%4] = LevelHash[i%4] ^ LevelPackFileName[i];
	LevelHash[0] = LevelHash[0] ^ LevelHash[2];
	LevelHash[1] = LevelHash[1] ^ LevelHash[0];
	LevelHash[2] = LevelHash[2] ^ LevelHash[3];
	LevelHash[3] = LevelHash[3] ^ LevelHash[2];
	for (Teller=0;Teller<64;Teller++)
		if ((Teller+1) % 16 == 0)
		{
			HashBuffer[Teller] = LevelHash[(Teller)/16];
			CheckSum += HashBuffer[Teller];
		}
		else
		{
			HashBuffer[Teller] = rand() % 256;
			CheckSum += HashBuffer[Teller];
		}
	CheckSum = CheckSum ^ 50;
	Fp = fopen(Filename,"wb");
	if (Fp)
	{
		fwrite(HashBuffer,1,64,Fp);
		fwrite(&CheckSum,sizeof(int),1,Fp);
		fclose(Fp);
#ifdef GP2X
		sync();
#endif
	}
}

void LoadUnlockData()
{
	FILE *Fp;
	unsigned char LevelHash[4];
	int Teller=0;
	unsigned char HashBuffer[64];
	char Filename[FILENAME_MAX];
	sprintf(Filename,"%s/.blockdude_%s.dat", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
	Fp = fopen(Filename,"rb");
	int CheckSum,ValidCheckSum=0;
	if (Fp)
	{
		fflush(Fp);
		fread(HashBuffer,1,64,Fp);
		fread(&CheckSum,sizeof(int),1,Fp);
		fclose(Fp);
		for (Teller = 0 ;Teller<64;Teller++)
		{
			ValidCheckSum += HashBuffer[Teller];
			if ((Teller+1)%16 == 0)
				LevelHash[Teller/16] = HashBuffer[Teller];
		}
		CheckSum = CheckSum ^ 50;
		if (CheckSum == ValidCheckSum)
		{
			LevelHash[3] = LevelHash[3] ^ LevelHash[2];
			LevelHash[2] = LevelHash[2] ^ LevelHash[3];
			LevelHash[1] = LevelHash[1] ^ LevelHash[0];
			LevelHash[0] = LevelHash[0] ^ LevelHash[2];
			for (size_t i=0;i<strlen(LevelPackFileName);i++)
				LevelHash[i%4] = LevelHash[i%4] ^ LevelPackFileName[i];
			for (Teller=0;Teller<4;Teller++)
				LevelHash[Teller] = LevelHash[Teller] ^ LevelPackFileName[strlen(LevelPackFileName)-1];

			Teller=0;
			while (LevelHash[0] != HashTable[Teller] || LevelHash[1] != HashTable[Teller+1] || 	LevelHash[2] != HashTable[Teller+2] || LevelHash[3] != HashTable[Teller+3] && Teller+3 < 1004)
				Teller++;
			if (Teller < InstalledLevels)
				UnlockedLevels = Teller+1;
			else
				UnlockedLevels = 1;
		}
		else
			UnlockedLevels = 1;
	}
	else
	 	UnlockedLevels = 1;

}

char *GetString(const char *NameIn,const char *Msg)
{
	char *PackName = new char[21];
	bool End=false,SubmitChanges=false;
	int Teller,MaxSelection=0, Selection = 0,asci=97;
	CInput *Input = new CInput(InputDelay);
	sprintf(PackName,"%s",NameIn);
	MaxSelection = strlen(NameIn);
	PackName[Selection+1]='\0';
	if (MaxSelection == 0)
		PackName[MaxSelection]=chr(asci);
	char Tekst[100];
	while (!End)
	{
	    if(GlobalSoundEnabled)
		if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(Music[0],0);
        }
        Input->Update();

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if(Input->Ready() && Input->KeyboardHeld[BUT_LEFT])
        {
            if (Selection > 0)
            {	Selection--;
                asci = ord(PackName[Selection]);
            }
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_RIGHT])
        {
            if (Selection < 19)
            {
                Selection++;
                if (Selection > MaxSelection)
                {
                    PackName[Selection] = chr(97);
                    PackName[Selection+1] = '\0';
                    MaxSelection=Selection;
                }
                asci = ord(PackName[Selection]);
            }
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_UP])
        {
            asci++;
            if (asci==123)
                asci=32;
            if (asci==33)
                (asci=48);
            if (asci==58)
                asci=97;
            PackName[Selection] = chr(asci);
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_DOWN])
        {
            asci--;
            if(asci==96)
                    asci=57;
            if(asci==47)
                asci=32;
            if(asci==31)
                asci=122;
            PackName[Selection] = chr(asci);
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_A])
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
            End = true;
            SubmitChanges=true;
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_X])
        {
            End=true;
            SubmitChanges=false;
        }


		SDL_BlitSurface(IMGTitleScreen,NULL,Screen,NULL);
		boxRGBA(Screen,20,70,220,165,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Screen,20,70,220,165,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Screen,21,71,219,164,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		WriteText(Screen,font,Msg,strlen(Msg),25,85,2,MenuTextColor);
		WriteText(Screen,MonoFont,PackName,strlen(PackName),45,110,2,MenuTextColor);
		if (Selection > 0)
		{
			sprintf(Tekst," ");
			for (Teller=1;Teller<Selection;Teller++)
				sprintf(Tekst,"%s ",Tekst);
			sprintf(Tekst,"%s_",Tekst);
		}
		else
			sprintf(Tekst,"_");
		WriteText(Screen,MonoFont,Tekst,strlen(Tekst),45,112,2,MenuTextColor);
		sprintf(Tekst,"Use Up,Down,Left,right\n %s=Ok %s=Cancel","A","B");
		WriteText(Screen,font,Tekst,strlen(Tekst),25,135,2,MenuTextColor);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	PackName[MaxSelection+1] = '\0';
	while ((PackName[0] == ' ') && (MaxSelection>-1))
	{
		for (Teller=0;Teller<MaxSelection;Teller++)
			PackName[Teller] = PackName[Teller+1];
		MaxSelection--;
	}
	if (MaxSelection>-1)
		while ((PackName[MaxSelection] == ' ') && (MaxSelection>0))
		{
			PackName[MaxSelection] = '\0';
			MaxSelection--;
		}
	if (!SubmitChanges)
		sprintf(PackName,"%s",NameIn);
    delete Input;
	return PackName;
}

CPlayer* FindPlayer()
{
    int teller=0;
    CPlayer *Player=0;
	for (teller=0;teller<WorldParts.ItemCount;teller++)
	{
		if (WorldParts.Items[teller]->GetType() == IDPlayer)
		{
			Player = (CPlayer*) WorldParts.Items[teller];
			break;
		}
	}
	//should never happen
	if(!Player)
	{
            Player = new CPlayer(0,0);
            WorldParts.Add(Player);
    }
    return Player;
}

void SetLayer2Position(CPlayer *Player)
{
        if((WorldParts.ViewPort->VPLimitMaxY -Player->GetPlayFieldY() <= 6))
        {
            WorldParts.ViewPort->BackgroundY = 0;
        }
        else
		{
            if((WorldParts.ViewPort->VPLimitMaxY -Player->GetPlayFieldY() > 6) && (Player->GetPlayFieldY() - WorldParts.ViewPort->VPLimitMinY  >= 8))
            {
                WorldParts.ViewPort->BackgroundY =  -(WorldParts.ViewPort->VPLimitMaxY -Player->GetPlayFieldY() -6)*(TileWidth/2);
            }
            else
			{
                if((Player->GetPlayFieldY() - WorldParts.ViewPort->VPLimitMinY  < 8))
                {
                    WorldParts.ViewPort->BackgroundY = -(WorldParts.ViewPort->VPLimitMaxY - (WorldParts.ViewPort->VPLimitMinY+14 ))*(TileWidth/2);
                }
			}
		}

        if((WorldParts.ViewPort->VPLimitMaxX -Player->GetPlayFieldX() <= 6))
        {
            WorldParts.ViewPort->BackgroundX = 0;
        }
        else
		{
            if((WorldParts.ViewPort->VPLimitMaxX -Player->GetPlayFieldX() > 6) && (Player->GetPlayFieldX() - WorldParts.ViewPort->VPLimitMinX  >= 8))
            {
                WorldParts.ViewPort->BackgroundX =  -(WorldParts.ViewPort->VPLimitMaxX -Player->GetPlayFieldX() -6)*(TileWidth/2);
            }
            else
			{
                if((Player->GetPlayFieldX() - WorldParts.ViewPort->VPLimitMinX  < 8))
                {
                    WorldParts.ViewPort->BackgroundX = -(WorldParts.ViewPort->VPLimitMaxX - (WorldParts.ViewPort->VPLimitMinX+14 ))*(TileWidth/2);
                }
			}
		}
}

void DrawLayer2(SDL_Surface *Surface)
{
    SDL_Rect Layer2RectSrc,Layer2RectDst;
    if(WorldParts.ViewPort->BackgroundX  > IMGLayer2->w)
        WorldParts.ViewPort->BackgroundX = WorldParts.ViewPort->BackgroundX  - IMGLayer2->w;
    if(WorldParts.ViewPort->BackgroundX < 0)
        WorldParts.ViewPort->BackgroundX = IMGLayer2->w+WorldParts.ViewPort->BackgroundX;
    if((WorldParts.ViewPort->BackgroundX + WINDOW_WIDTH <= IMGLayer2->w))
    {
        Layer2RectSrc.x = WorldParts.ViewPort->BackgroundX;
        Layer2RectSrc.y = 0;
        Layer2RectSrc.w = WINDOW_WIDTH;
        Layer2RectSrc.h = IMGLayer2->h;
        Layer2RectDst.x = 0;
        Layer2RectDst.y = WINDOW_HEIGHT-IMGLayer2->h-WorldParts.ViewPort->BackgroundY;
        Layer2RectDst.w = WINDOW_WIDTH;
        Layer2RectDst.h = IMGLayer2->h;
        SDL_BlitSurface(IMGLayer2,&Layer2RectSrc,Surface,&Layer2RectDst);
    }

    if(WorldParts.ViewPort->BackgroundX + WINDOW_WIDTH > IMGLayer2->w)
    {
        Layer2RectSrc.x = WorldParts.ViewPort->BackgroundX;
        Layer2RectSrc.y = 0;
        Layer2RectSrc.w = (IMGLayer2->w - WorldParts.ViewPort->BackgroundX);
        Layer2RectSrc.h = IMGLayer2->h;
        Layer2RectDst.x = 0;
        Layer2RectDst.y = WINDOW_HEIGHT-IMGLayer2->h-WorldParts.ViewPort->BackgroundY;
        Layer2RectDst.w = (IMGLayer2->w - WorldParts.ViewPort->BackgroundX);
        Layer2RectDst.h = IMGLayer2->h;
        SDL_BlitSurface(IMGLayer2,&Layer2RectSrc,Surface,&Layer2RectDst);

        Layer2RectSrc.x = 0;
        Layer2RectSrc.y = 0;
        Layer2RectSrc.w = (WINDOW_WIDTH-(IMGLayer2->w-WorldParts.ViewPort->BackgroundX));
        Layer2RectSrc.h = IMGLayer2->h;
        Layer2RectDst.x = WINDOW_WIDTH-(WINDOW_WIDTH-(IMGLayer2->w-WorldParts.ViewPort->BackgroundX));
        Layer2RectDst.y = WINDOW_HEIGHT-IMGLayer2->h-WorldParts.ViewPort->BackgroundY;
        Layer2RectDst.w = (WINDOW_WIDTH-(IMGLayer2->w-WorldParts.ViewPort->BackgroundX));
        Layer2RectDst.h = IMGLayer2->h;
        SDL_BlitSurface(IMGLayer2,&Layer2RectSrc,Surface,&Layer2RectDst);
    }

}


void StageSelect()
{
    SDL_Surface *Tmp,*Tmp1;
	int Teller;
	int alpha = 0;
	char *FileName = new char[FILENAME_MAX];
	char Tekst[500];
	char Tekst1[500];
	CPlayer *Player;
    Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
    CInput *Input = new CInput(InputDelay);
	if (SelectedLevel > 0)
	{
		sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
		WorldParts.Load(FileName);
		Player = FindPlayer();
		SetLayer2Position(Player);

	}
	else
		WorldParts.RemoveAll();
	while (GameState == GSStageSelect)
	{
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                Mix_PlayMusic(Music[0],0);
            }
		SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
        if(SelectedLevel >0)
            DrawLayer2(Tmp);
		WorldParts.Draw(Tmp);
		boxRGBA(Tmp,0,0,WINDOW_WIDTH-1,13,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,0,-1,WINDOW_WIDTH-1,13,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		if (SelectedLevel ==0)
			sprintf(Tekst,"Level Pack: %s -> %d Levels - (%s) Create New",LevelPackName,InstalledLevels,"A");
		else
			if(LevelEditorMode)
				sprintf(Tekst,"Level Pack: %s Level:%d/%d - (%s) Edit (%s) Delete",LevelPackName,SelectedLevel,InstalledLevels,"A","B");
			else
				if(SelectedLevel <= UnlockedLevels)
					sprintf(Tekst,"Level Pack: %s Level:%d/%d - (%s) Play",LevelPackName,SelectedLevel,InstalledLevels,"A");
				else
					sprintf(Tekst,"Level Pack: %s Level:%d/%d - Level is locked!",LevelPackName,SelectedLevel,InstalledLevels);
		WriteText(Tmp,font,Tekst,strlen(Tekst),2,0,0,MenuTextColor);

        Input->Update();

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if(Input->KeyboardHeld[BUT_Y])
        {
            if(LevelEditorMode)
            {
                SelectedLevel = 0;
                WorldParts.RemoveAll();
                LevelHasChanged = false;
                GameState = GSLevelEditor;
            }
        }

        if(Input->KeyboardHeld[BUT_SELECT])
        {
            if(LevelEditorMode)
                GameState= GSLevelEditorMenu;
            else
                GameState= GSTitleScreen;
            WorldParts.RemoveAll();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_B])
        {
            if(LevelEditorMode && (SelectedLevel > 0))
            {
                sprintf(Tekst,"Are you sure you want to delete this level:\n%s - Level %d\n\nPress (%s) to Delete (%s) to Cancel",LevelPackName,SelectedLevel,"A","B");
                if (AskQuestion(Tekst))
                {
                    sprintf(Tekst,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                    remove(Tekst);
                    for(Teller=SelectedLevel;Teller<InstalledLevels;Teller++)
                    {
                        sprintf(Tekst,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,Teller+1);
                        sprintf(Tekst1,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
                        rename(Tekst,Tekst1);
                    }
                    InstalledLevels--;
                    if (SelectedLevel > InstalledLevels)
                        SelectedLevel = InstalledLevels;
                    if (SelectedLevel==0)
                        WorldParts.RemoveAll();
                    else
                    {
                        sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                        WorldParts.Load(FileName);
                        Player = FindPlayer();
                        SetLayer2Position(Player);
                    }
                }
                Input->Reset();
            }
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_A])
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
            if(LevelEditorMode)
            {
                LevelHasChanged = false;
                GameState = GSLevelEditor;
            }
            else
                if (SelectedLevel <= UnlockedLevels)
                    GameState = GSGame;
                else
                {
                    sprintf(Tekst,"This Level Hasn't been unlocked yet!\nDo you want to play the last unlocked\nlevel %d/%d\n\nPress (%s) to Play (%s) to Cancel",UnlockedLevels,InstalledLevels,"A","B");
                    if	(AskQuestion(Tekst))
                    {
                        SelectedLevel = UnlockedLevels;
                        sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                        WorldParts.Load(FileName);
                        GameState = GSGame;
                    }
                    Input->Reset();
                }
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_L])
        {
            SelectedLevel -= 5;
            if(LevelEditorMode)
            {
                if (SelectedLevel <= 0)
                {
                    SelectedLevel = 0;
                    WorldParts.RemoveAll();
                }
                else
                {
                    sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                    WorldParts.Load(FileName);
                    Player = FindPlayer();
                    SetLayer2Position(Player);
                }
            }
            else
            {
                if (SelectedLevel < 1)
                    SelectedLevel = 1;
                sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                WorldParts.Load(FileName);
                Player = FindPlayer();
                SetLayer2Position(Player);
            }
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_R])
        {
            SelectedLevel +=5;
            if (SelectedLevel > InstalledLevels)
                    SelectedLevel = InstalledLevels;
            sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
            WorldParts.Load(FileName);
            Player = FindPlayer();
            SetLayer2Position(Player);
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_LEFT])
        {
            SelectedLevel--;
            if(LevelEditorMode)
            {
                if (SelectedLevel <= 0)
                {
                    SelectedLevel = 0;
                    WorldParts.RemoveAll();
                }
                else
                {
                    sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                    WorldParts.Load(FileName);
                    Player = FindPlayer();
                    SetLayer2Position(Player);
                }
            }
            else
            {
                if (SelectedLevel < 1)
                    SelectedLevel = 1;
                sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                WorldParts.Load(FileName);
                Player = FindPlayer();
                SetLayer2Position(Player);
            }

            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_RIGHT])
        {
            SelectedLevel++;
            if (SelectedLevel > InstalledLevels)
                SelectedLevel = InstalledLevels;
            sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
            WorldParts.Load(FileName);
            Player = FindPlayer();
            SetLayer2Position(Player);
            Input->Delay();
        }

        if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
        }
        SDL_BlitSurface(Tmp,NULL,Screen,NULL);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	delete[] FileName;
	SDL_FreeSurface(Tmp);
}


void FindLevels()
{
	int Teller=1;
	char *FileName = new char[FILENAME_MAX];
	InstalledLevels = 0;
	sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
	while (FileExists(FileName))
	{
		Teller+=25;
		sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
	}
	while (!FileExists(FileName) && (Teller >=1) )
	{
		Teller--;
		sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
	}
	InstalledLevels=Teller;
	delete[] FileName;
}

void Credits()
{
    int alpha = 0;
    SDL_Surface *Tmp,*Tmp1;
	char *LevelPackCreator = new char[21];
	char FileName[FILENAME_MAX];
	FILE *Fp;
    Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
	CInput *Input = new CInput(InputDelay);
	char *Tekst = new char[500];
	sprintf(FileName,"./blockdudefs/levelpacks/%s/credits.dat",LevelPackFileName);
	if(InstalledLevelPacksCount > 0)
	{
		Fp = fopen(FileName,"rt");
		if (Fp)
		{
			fscanf(Fp,"[Credits]\nCreator='%[^']'\n",LevelPackCreator);
			fclose(Fp);
			sprintf(Tekst,"Blockdude GP2X was created by\nWillems Davy - Willems Soft 2007.\nHttp://www.willemssoft.be\n\nLevelpack %s was created\nby %s.",LevelPackName,LevelPackCreator);
		}
		else
			sprintf(Tekst,"Blockdude GP2X was created by\nWillems Davy - Willems Soft 2007.\nHttp://www.willemssoft.be\n\nLevelpack %s was created\nby unknown person.",LevelPackName);
	}
	else
		sprintf(Tekst,"Blockdude GP2X was created by\nWillems Davy - Willems Soft 2007\nHttp://www.willemssoft.be");
	while (GameState == GSCredits)
	{
	    if(GlobalSoundEnabled)
		if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(Music[0],0);
        }
		SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
		Input->Update();
        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

		if(Input->KeyboardHeld[BUT_SELECT] || Input->KeyboardHeld[BUT_START] || 
			Input->KeyboardHeld[BUT_A] || Input->KeyboardHeld[BUT_B] || 
			Input->KeyboardHeld[BUT_X] || Input->KeyboardHeld[BUT_Y])
        {
            GameState=GSTitleScreen;
        }
		boxRGBA(Tmp,20,70,220,165,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,20,70,220,165,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,21,71,219,164,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		WriteText(Tmp,font,Tekst,strlen(Tekst),25,80,2,MenuTextColor);
        if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
        }
        SDL_BlitSurface(Tmp,NULL,Screen,NULL);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	delete[] Tekst;
	delete[] LevelPackCreator;
	delete Input;
	SDL_FreeSurface(Tmp);
}


void TitleScreen()
{
	int alpha = 0;
    SDL_Surface *Tmp,*Tmp1;
	int Teller, Selection = 1;
	char *Tekst = new char[300];
	Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
	CInput *Input = new CInput(InputDelay);
	while (GameState == GSTitleScreen)
	{
		if(GlobalSoundEnabled)
 			if(!Mix_PlayingMusic())
            {
        	    Mix_PlayMusic(Music[0],0);
			}

		SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
		Input->Update();

		if(Input->KeyboardHeld[BUT_SELECT] || Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if (Input->Ready() && Input->KeyboardHeld[BUT_L])
        {
            PreviousSkin();
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_LEFT])
        {
            if(Selection==3)
                if (InstalledLevelPacksCount > 0)
                    if(SelectedLevelPack > 0)
                    {
                        SelectedLevelPack--;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_RIGHT])
        {
            if (Selection==3)
                if (InstalledLevelPacksCount > 0)
                    if(SelectedLevelPack < InstalledLevelPacksCount-1)
                    {
                        SelectedLevelPack++;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_DOWN])
        {
            if (Selection < 5)
            {
                Selection++;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }

            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_UP])
        {
            if (Selection > 1)
            {
                Selection--;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }
            Input->Delay();
        }

        if(Input->KeyboardHeld[BUT_A])
        {
            switch(Selection)
            {
                case 1:
                    if (InstalledLevelPacksCount >0)
                    {
                        FindLevels();
                        if(InstalledLevels > 0)
                        {
                            LoadUnlockData();
                            SelectedLevel=UnlockedLevels;
                            LevelEditorMode=false;
                            GameState=GSStageSelect;
                            if (GlobalSoundEnabled)
                                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        }
                        else
                        {
                            if (GlobalSoundEnabled)
                                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                            //BUG ADD BLITSURFACE OF TITLESCREEN HERE
                            sprintf(Tekst,"There are no levels found in levelpack\n%s\n\nPlease create a level for this level pack first!",LevelPackName);
                            PrintForm(Tekst);
                            Input->Reset();
                        }
                    }
                    break;
                case 2:
                    GameState=GSLevelEditorMenu;
                    LevelEditorMode=true;
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    break;
                case 4:
                    GameState=GSCredits;
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    break;
                case 5:
                    GameState = GSQuit;
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    break;

            }

        }
		boxRGBA(Tmp,20,70,220,165,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,20,70,220,165,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,21,71,219,164,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		sprintf(Tekst,"Play Selected LevelPack\nLevel Editor\n<%s>\nCredits\nQuit",LevelPackName);
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),50,83,2,MenuTextColor);
		if (Selection > 1)
		{
			sprintf(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				sprintf(Tekst,"%s%s",Tekst,"\n");
			sprintf(Tekst,"%s%s",Tekst,">>");
		}
		else
			sprintf(Tekst,">>");
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),25,83,2,MenuTextColor);
		if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
        }
        SDL_BlitSurface(Tmp,NULL,Screen,NULL);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);

	}
	delete[] Tekst;
	delete Input;
	SDL_FreeSurface(Tmp);
}


void LevelEditorMenu()
{
	int alpha = 0;
    SDL_Surface *Tmp,*Tmp1;
	FILE *Fp;
	int Teller, Selection = 1;
	char *PackName,*CreatorName;
	char FileName[FILENAME_MAX];
    Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
	char *Tekst = new char[300];
	CInput *Input = new CInput(InputDelay);
	while (GameState == GSLevelEditorMenu)
	{
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                Mix_PlayMusic(Music[0],0);
            }
        SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
		Input->Update();

		if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
			GameState = GSQuit;

		if(Input->KeyboardHeld[BUT_SELECT])
            GameState = GSTitleScreen;

        if (Input->Ready() && Input->KeyboardHeld[BUT_L])
        {
            PreviousSkin();
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_LEFT])
        {
            if(Selection==4)
                if (InstalledLevelPacksCount > 0)
                    if(SelectedLevelPack > 0)
                    {
                        SelectedLevelPack--;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_RIGHT])
        {
            if (Selection==4)
                if (InstalledLevelPacksCount > 0)
                    if(SelectedLevelPack < InstalledLevelPacksCount-1)
                    {
                        SelectedLevelPack++;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_DOWN])
        {
            if (Selection < 5)
            {
                Selection++;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }

            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_UP])
        {
            if (Selection > 1)
            {
                Selection--;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }
            Input->Delay();
        }

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if(Input->KeyboardHeld[BUT_A])
        {
            switch(Selection)
            {
                case 1:
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    PackName = GetString("","Enter the Levelpack name:");
                    Input->Reset();
                    if (strlen(PackName) > 0)
                    {
                        SDL_BlitSurface(IMGTitleScreen,NULL,Screen,NULL);
                        CreatorName = GetString("","Enter the Levelpack Creator name:");
                        Input->Reset();
                        if(strlen(CreatorName)>0)
                        {
                            sprintf(LevelPackName,"%s",PackName);
                            sprintf(LevelPackFileName,"%s",PackName);
                            AddUnderScores(LevelPackFileName);
                            sprintf(Tekst,"./blockdudefs/levelpacks/%s",LevelPackFileName);
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif
                            sprintf(FileName,"./blockdudefs/levelpacks/%s/credits.dat",LevelPackFileName);
                            Fp = fopen(FileName,"wt");
                            if (Fp)
                            {
                                fprintf(Fp,"[Credits]\nCreator='%s'\n",CreatorName);
                                fclose(Fp);
                            }
                            SearchForLevelPacks();
                            for (Teller=0;Teller<InstalledLevelPacksCount;Teller++)
                                if(strcmp(PackName,InstalledLevelPacks[Teller]) == 0)
                                {
                                    SelectedLevelPack = Teller;
                                    sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                                    sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                                    AddUnderScores(LevelPackFileName);
                                }
                        }
                        delete[] CreatorName;
                    }
                    delete[] PackName;
                    break;
                case 2:
                    if (InstalledLevelPacksCount >0)
                    {
                        FindLevels();
                        SelectedLevel=0;
                        GameState=GSStageSelect;
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    }
                    break;
                case 3:
                    if (InstalledLevelPacksCount >0)
                    {
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        sprintf(Tekst,"do you want to delete the selected level pack:\n\"%s\"\nAll Levels in Levelpack will be deleted !!!\n\nPress %s to Delete, %s to Cancel",InstalledLevelPacks[SelectedLevelPack],"A","B");
                        if(AskQuestion(Tekst))
                        {
                            for(Teller=1;Teller<INT_MAX;Teller++)
                            {
                                sprintf(Tekst,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
                                if(FileExists(Tekst))
                                {
                                    remove(Tekst);
                                }
                                else
                                    break;
                            }
                            sprintf(Tekst,"./blockdudefs/levelpacks/%s/credits.dat",LevelPackFileName);
                            if(FileExists(Tekst))
                            {
                                remove(Tekst);
                            }
                            sprintf(Tekst,"./blockdudefs/levelpacks/%s",LevelPackFileName);
                            rmdir(Tekst);

                            SearchForLevelPacks();
                        }
                        Input->Reset();

                    }
                    break;
                case 5:
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    GameState = GSTitleScreen;
                    break;
            }
        }
		boxRGBA(Tmp,20,70,220,165,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,20,70,220,165,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,21,71,219,164,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		sprintf(Tekst,"Create New Levelpack\nLoad Selected LevelPack\nDelete Selected Levelpack\n<%s>\nMain Menu",LevelPackName);
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),50,83,2,MenuTextColor);
		if (Selection > 1)
		{
			sprintf(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				sprintf(Tekst,"%s%s",Tekst,"\n");
			sprintf(Tekst,"%s%s",Tekst,">>");
		}
		else
			sprintf(Tekst,">>");
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),25,83,2,MenuTextColor);
		if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
        }
        SDL_BlitSurface(Tmp,NULL,Screen,NULL);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	delete[] Tekst;
	delete Input;
	SDL_FreeSurface(Tmp);
}

bool LevelErrorsFound()
{
	int Teller,NumPlayer=0,NumExit=0;
	char Tmp[255];
	for (Teller=0;Teller<WorldParts.ItemCount;Teller++)
	{
		if(WorldParts.Items[Teller]->GetType() == IDPlayer)
			NumPlayer++;
        if(WorldParts.Items[Teller]->GetType() == IDExit)
			NumExit++;
	}

	if (NumPlayer==0)
	{
		if (GlobalSoundEnabled)
			Mix_PlayChannel(-1,Sounds[SND_ERROR],0);
		sprintf(Tmp,"Can not save this level because there's\nno player in the level! Please add a Player\nand try again or cancel saving.\n\nPress '%s' to continue","A");
		PrintForm(Tmp);
		return true;
	}
	else
        if(NumExit == 0)
        {
            if (GlobalSoundEnabled)
			Mix_PlayChannel(-1,Sounds[SND_ERROR],0);
            sprintf(Tmp,"Can not save this level because there's\nno exit in the level! Please add an Exit\nand try again or cancel saving.\n\nPress '%s' to continue","A");
            PrintForm(Tmp);
            return true;
        }
	return false;

}


void Game()
{

	SDL_Surface *Tmp,*Tmp1;
	bool BoxMoving = false,CarryingBox=false;
	char FileName[FILENAME_MAX];
	char Text[500];
	int alpha=0,teller,teller2=0;
	Uint32 Time=0;
	CPlayer *Player=0;
	Player = FindPlayer();

	CInput *Input = new CInput(InputDelay);

	Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
	bool Que=false,FloorFound=false,ResetViewPort = false;
	Time = SDL_GetTicks();
    WorldParts.LimitVPLevel();
    SetLayer2Position(Player);

    while (GameState == GSGame)
    {

        if(!Mix_PlayingMusic())
        if(GlobalSoundEnabled)
        {
			Mix_PlayMusic(Music[0],0);
        }

        Input->Update();
        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;
        if(Input->Ready() && Input->KeyboardHeld[BUT_SELECT])
        {
            if(!LevelEditorMode)
                GameState = GSStageSelect;
        }
        if(Input->Ready() && Input->KeyboardHeld[BUT_START])
        {
            if(LevelEditorMode)
                GameState=GSLevelEditor;
            else
            {
                sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                WorldParts.Load(FileName);
                Player = FindPlayer();
                SetLayer2Position(Player);
            }
            Input->Delay();
        }
        if (Input->Ready() && Input->KeyboardHeld[BUT_L])
        {
            NextSkin();
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_LEFT])
        {
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_RIGHT])
        {
            Input->Delay();
        }

        if(Time+10<SDL_GetTicks())
        {
             BoxMoving = false;
             //find out if a box is moving or if a box is being carried by the player
             CarryingBox = false;
             Que = false;
                    for (teller=0;teller<WorldParts.ItemCount;teller++)
                    {
                        if ((WorldParts.Items[teller]->IsMoving) && (WorldParts.Items[teller]->GetType()==IDBox))
                        {
                            BoxMoving = true;

                        }
                        if (WorldParts.Items[teller]->GetType()==IDBox)
                        {
                            if(((CBox *)WorldParts.Items[teller])->IsAttachedToPlayer())
                            {
                                CarryingBox  = true;
                            }
                            if(WorldParts.Items[teller]->MovesInQue())
                                Que = true;
                        }
                    }

            if (Input->KeyboardHeld[BUT_Y])
            {
                if(Input->KeyboardHeld[BUT_LEFT])
                    WorldParts.ViewPort->Move(-2,0);
                if(Input->KeyboardHeld[BUT_RIGHT])
                    WorldParts.ViewPort->Move(2,0);
                if(Input->KeyboardHeld[BUT_UP])
                    WorldParts.ViewPort->Move(0,-2);
                if(Input->KeyboardHeld[BUT_DOWN])
                    WorldParts.ViewPort->Move(0,2);
                ResetViewPort = true;
            }
            else
            {
                if (ResetViewPort)
                {
                    WorldParts.CenterVPOnPlayer();
                    SetLayer2Position(Player);
                    ResetViewPort = false;
                }
            }

            if (!Player->IsMoving && !BoxMoving &&!Que && !(Input->KeyboardHeld[BUT_Y]))
            {

                //move up
                if (Input->KeyboardHeld[BUT_A] || Input->KeyboardHeld[BUT_UP])
                {
                        Player->MoveTo(Player->GetPlayFieldX(),Player->GetPlayFieldY()-1,false);
                }
                //pickup block
                if (Input->KeyboardHeld[BUT_B])
                {
                    for (teller=0;teller<WorldParts.ItemCount;teller++)
                        //dropping a block
                    {   //if there is a block on top of the player and were facing left
                        if(CarryingBox)
                        {
                        if((WorldParts.Items[teller]->GetPlayFieldX() == Player->GetPlayFieldX() ) &&
                           (WorldParts.Items[teller]->GetPlayFieldY() == Player->GetPlayFieldY()-1) &&
                           (WorldParts.Items[teller]->GetType() == IDBox) && (Player->GetAnimBase() ==AnimBaseLeft))
                        {  // and it can move to the left
                            if ((WorldParts.Items[teller]->CanMoveTo(WorldParts.Items[teller]->GetPlayFieldX()-1,WorldParts.Items[teller]->GetPlayFieldY())))
                            {
                                //move it to the left

                                WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX()-1,WorldParts.Items[teller]->GetPlayFieldY());
                                WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX()-1,WorldParts.Items[teller]->GetPlayFieldY()+1);
                                ((CBox*)WorldParts.Items[teller])->DeattachFromPlayer();
                            }
                        }
                        else // if there is block on top of theplayer were facing right
                        if((WorldParts.Items[teller]->GetPlayFieldX() == Player->GetPlayFieldX() ) &&
                           (WorldParts.Items[teller]->GetPlayFieldY() == Player->GetPlayFieldY()-1) &&
                           (WorldParts.Items[teller]->GetType() == IDBox) && (Player->GetAnimBase() ==AnimBaseRight))
                        {   //and it can be moved to the right
                            if ((WorldParts.Items[teller]->CanMoveTo(WorldParts.Items[teller]->GetPlayFieldX()+1,WorldParts.Items[teller]->GetPlayFieldY())))
                            {
                                //move it to right
                                WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX()+1,WorldParts.Items[teller]->GetPlayFieldY());
                                WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX()+1,WorldParts.Items[teller]->GetPlayFieldY()+1);
                                ((CBox*)WorldParts.Items[teller])->DeattachFromPlayer();
                            }

                        }
                        }
                        else
                        if(!CarryingBox)
                        {
                            FloorFound = false;
                            //picking up a block
                            //if there is a block left to the player and were facing left
                            if((WorldParts.Items[teller]->GetPlayFieldX() == Player->GetPlayFieldX() -1) &&
                            (WorldParts.Items[teller]->GetPlayFieldY() == Player->GetPlayFieldY()) &&
                            (WorldParts.Items[teller]->GetType() == IDBox) && (Player->GetAnimBase() ==AnimBaseLeft))
                            {

                                //see if there is a floor or block beneath the block
                                for (teller2=0;teller2< WorldParts.ItemCount;teller2++)
                                {
                                    if((WorldParts.Items[teller2]->GetPlayFieldX() == Player->GetPlayFieldX() -1) && (WorldParts.Items[teller2]->GetPlayFieldY() == Player->GetPlayFieldY()+1) &&
                                        ((WorldParts.Items[teller2]->GetGroup() == GroupFloor) || (WorldParts.Items[teller2]->GetGroup() == GroupBox)))
                                    {
                                        FloorFound = true;
                                        break;
                                    }
                                }
                                if(FloorFound)
                                //if there was see if there is space above the block and above the player
                                if ((WorldParts.Items[teller]->CanMoveTo(WorldParts.Items[teller]->GetPlayFieldX(),WorldParts.Items[teller]->GetPlayFieldY()-1)) &&
                                    (WorldParts.Items[teller]->CanMoveTo(WorldParts.Items[teller]->GetPlayFieldX()+1,WorldParts.Items[teller]->GetPlayFieldY()-1)))
                                {
                                    //Attach the block to the player & move the block
                                    ((CBox*)WorldParts.Items[teller])->AttachToPlayer(Player);
                                    WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX(),WorldParts.Items[teller]->GetPlayFieldY()-1);
                                    WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX()+1,WorldParts.Items[teller]->GetPlayFieldY()-1);
                                }
                            }
                            else
                            //if there is a block right to the player and were facing right
                            if((WorldParts.Items[teller]->GetPlayFieldX() == Player->GetPlayFieldX() +1) &&
                            (WorldParts.Items[teller]->GetPlayFieldY() == Player->GetPlayFieldY()) &&
                            (WorldParts.Items[teller]->GetType() == IDBox) && (Player->GetAnimBase() ==AnimBaseRight))
                            {
                                //see if there is a floor or block beneath the block
                                for (teller2=0;teller2< WorldParts.ItemCount;teller2++)
                                {
                                    if((WorldParts.Items[teller2]->GetPlayFieldX() == Player->GetPlayFieldX()+1) && (WorldParts.Items[teller2]->GetPlayFieldY() == Player->GetPlayFieldY()+1) &&
                                        ((WorldParts.Items[teller2]->GetGroup() == GroupFloor) || (WorldParts.Items[teller2]->GetGroup() == GroupBox)))
                                    {
                                        FloorFound = true;
                                        break;
                                    }
                                }
                                if(FloorFound)
                                //if there was see if there is space above the block and above the player
                                if (WorldParts.Items[teller]->CanMoveTo(WorldParts.Items[teller]->GetPlayFieldX(),WorldParts.Items[teller]->GetPlayFieldY()-1) &&
                                    WorldParts.Items[teller]->CanMoveTo(WorldParts.Items[teller]->GetPlayFieldX()-1,WorldParts.Items[teller]->GetPlayFieldY()-1))
                                {
                                    //Attach the block to the player & move the block
                                    ((CBox*)WorldParts.Items[teller])->AttachToPlayer(Player);
                                    WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX(),WorldParts.Items[teller]->GetPlayFieldY()-1);
                                    WorldParts.Items[teller]->AddToMoveQue(WorldParts.Items[teller]->GetPlayFieldX()-1,WorldParts.Items[teller]->GetPlayFieldY()-1);
                                }
                            }
                        }
                    }
                }
                if ( Input->KeyboardHeld[BUT_LEFT])
                {
                        Player->MoveTo(Player->GetPlayFieldX()-1,Player->GetPlayFieldY(),false);
                }
                if (Input->KeyboardHeld[BUT_RIGHT])
                {
                        Player->MoveTo(Player->GetPlayFieldX()+1,Player->GetPlayFieldY(),false);
                }

            }
            Time = SDL_GetTicks();

        }

        SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
        DrawLayer2(Tmp);
        WorldParts.Draw(Tmp);
        WorldParts.Move();

        if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
        }
        SDL_BlitSurface(Tmp,NULL,Screen,NULL);
        SDL_Flip(Screen);
        if (!Player->IsMoving && StageDone(Player))
		{
			SDL_Delay(250);
			if (GlobalSoundEnabled)
				Mix_PlayChannel(-1,Sounds[SND_STAGEEND],0);
			if (LevelEditorMode)
			{
				sprintf(Text,"Congratulations !\nYou Succesfully Solved this level\nDo you want to return to the\nlevel editor ?\n(%s) Leveleditor (%s) Play Again","A","B");
				if (AskQuestion(Text))
					GameState = GSLevelEditor;
				else
				{
					sprintf(FileName,"./temp.lev");
					WorldParts.Load(FileName);
					Player = FindPlayer();
                    SetLayer2Position(Player);
				}
				Input->Reset();

			}
			else
			{
				if (SelectedLevel == UnlockedLevels)
				{
					if ( UnlockedLevels < InstalledLevels)
					{
						sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\nThe next level has now been unlocked!\n\nPress (%s) to continue",SelectedLevel,InstalledLevels,"A");
						PrintForm(Text);
						UnlockedLevels++;
						SelectedLevel++;
						SaveUnlockData();
						GameState = GSStageSelect;
					}
					else
					{
						sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\nlevelpack %s\nis now finished, try out another one!\n\nPress (%s) to continue",SelectedLevel,InstalledLevels,LevelPackName,"A");
						PrintForm(Text);
						GameState = GSTitleScreen;
					}
				}
				else
				{
					sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\n\nPress (%s) to continue",SelectedLevel,InstalledLevels,"A");
					PrintForm(Text);
					GameState = GSStageSelect;

				}
			}
		}
		SDL_framerateDelay(&Fpsman);
    }
    delete Input;
    SDL_FreeSurface(Tmp);
}

void LevelEditor()
{
	int Teller,alpha=0;
	SDL_Surface *Tmp,*Tmp1;
	bool ShowPosition=true,SamePartFound,PlayerFound=false,ShowGrid=true;
	char Tekst[200];
	char *FileName = new char[FILENAME_MAX];
	CSelector Selector(&WorldParts);
	CInput *Input = new CInput(InputDelay-5);
	int MaxX=0,MaxY=0,MinY=NrOfRows,MinX=NrOfCols,Xi=0,Yi=0;
    Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
	if (StageReload)
	{
		WorldParts.Load("./temp.lev");
		remove("./temp.lev");
		StageReload=false;
	}
    for (Teller = 0;Teller< WorldParts.ItemCount;Teller++)
        if(WorldParts.Items[Teller]->GetType() == IDPlayer)
        {
            PlayerFound = true;
            Selector.SetPosition(WorldParts.Items[Teller]->GetPlayFieldX(),WorldParts.Items[Teller]->GetPlayFieldY());
            break;

        }
    WorldParts.ViewPort->SetVPLimit(0,0,NrOfCols-1,NrOfRows-1);
    if (!PlayerFound)
    {
        WorldParts.ViewPort->SetViewPort((NrOfCols/2)-6,(NrOfRows/2)-6,(NrOfCols/2)+8,(NrOfRows/2)+8);
        Selector.SetPosition((NrOfCols/2),(NrOfRows/2));
    }

	while (GameState == GSLevelEditor)
	{
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                Mix_PlayMusic(Music[0],0);
            }

		if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
        }
		SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
		if(ShowGrid)
            SDL_BlitSurface(IMGGrid,NULL,Tmp,NULL);
		WorldParts.Draw(Tmp);

		Selector.Draw(Tmp);
		if (ShowPosition)
		{
			sprintf(Tekst,"X: %d - Y: %d",Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
			boxRGBA(Tmp,177,0,WINDOW_WIDTH-1,13,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
			rectangleRGBA(Tmp,177,-1,WINDOW_WIDTH,13,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
			WriteText(Tmp,font,Tekst,strlen(Tekst),179,0,0,MenuTextColor);
		}
		SDL_BlitSurface(Tmp,NULL,Screen,NULL);
        SDL_Flip(Screen);
        Input->Update();

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if(Input->KeyboardHeld[BUT_SELECT])
        {
            if (LevelHasChanged)
            {

                sprintf(Tekst,"The current level isn't saved yet!\nDo you want to save it now ?\n\nPress (%s) to save, (%s) to cancel saving","A","B");
                if(AskQuestion(Tekst))
                {
                    SDL_BlitSurface(IMGBackground,NULL,Screen,NULL);
                    WorldParts.Draw(Screen);
                    Selector.Draw(Screen);
                    if (ShowPosition)
                    {
                        sprintf(Tekst,"X: %d - Y: %d",Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                        boxRGBA(Screen,217,0,WINDOW_WIDTH-1,13,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
                        rectangleRGBA(Screen,217,-1,WINDOW_WIDTH,13,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
                        WriteText(Screen,font,Tekst,strlen(Tekst),219,0,0,MenuTextColor);
                    }
                    if (!LevelErrorsFound())
                    {
                        if (SelectedLevel==0)
                                sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,InstalledLevels+1);
                        else
                            sprintf(FileName,"./blockdudefs/levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                        WorldParts.Save(FileName);
                        FindLevels();
                        if (SelectedLevel==0)
                            SelectedLevel = InstalledLevels;
                        LevelHasChanged=false;
                        GameState = GSStageSelect;
                    }
                }
                else
                    GameState = GSStageSelect;
                Input->Reset();
            }
            else
                GameState = GSStageSelect;
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_START])
        {
            if(!LevelErrorsFound())
            {
                WorldParts.Save("./temp.lev");
                StageReload = true;
                GameState=GSGame;
                WorldParts.CenterVPOnPlayer();
            }
            Input->Reset();
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_L])
        {
            Selector.DecSelection();
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_R])
        {
            Selector.IncSelection();
            Input->Delay();
        }

        if(Input->KeyboardHeld[BUT_A])
        {
            SamePartFound = false;
            for(Teller=0;Teller<WorldParts.ItemCount;Teller++)
                if((WorldParts.Items[Teller]->GetPlayFieldX() == Selector.GetPlayFieldX()) &&
                   (WorldParts.Items[Teller]->GetPlayFieldY() == Selector.GetPlayFieldY()))
                {
                    if(WorldParts.Items[Teller]->GetType() == Selector.GetSelection())
                    {
                        SamePartFound=true;
                    }
                    if(Selector.GetSelection() == IDEmpty)
                    {
                        LevelHasChanged=true;
                        break;
                    }
                }
            if(Selector.GetSelection() != IDEmpty)
                if(!LevelHasChanged)
                    LevelHasChanged=!SamePartFound;
            switch(Selector.GetSelection())
            {
                case IDEmpty:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    break;
                case IDBox:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add(new CBox(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDPlayer:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    for(Teller=0;Teller<WorldParts.ItemCount;Teller++)
                    {
                        if (WorldParts.Items[Teller]->GetType() == IDPlayer)
                            WorldParts.Remove(WorldParts.Items[Teller]->GetPlayFieldX(),WorldParts.Items[Teller]->GetPlayFieldY(),IDPlayer);
                    }
                    WorldParts.Add(new CPlayer(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloor:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add(new CFloor(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDExit:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add(new CExit(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDEarthGrassLeft:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CEarthGrassLeft(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDEarthGrassRight:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CEarthGrassRight(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDEarthLeft :
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CEarthLeft(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDEarthMiddle:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CEarthMiddle(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDEarthRight:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CEarthRight(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloatingFloor:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CFloatingFloor (Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloatingFloorLeft:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CFloatingFloorLeft(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloatingFloorMiddle:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add(new CFloatingFloorMiddle(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloatingFloorRight:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CFloatingFloorRight(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloorLeft:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CFloorLeft(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloorRight:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CFloorRight(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDTower:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CTower(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDStartTower:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CStartTower(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDTowerShaft:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CTowerShaft(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDRoof1 :
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CRoof1(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDRoof2:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CRoof2(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDRoofCornerLeft:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CRoofCornerLeft(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDRoofCornerRight:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CRoofCornerRight(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDRoofCornerBoth:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CRoofCornerBoth(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDRoofDownRight:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CRoofDownRight(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDRoofDownLeft:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add( new CRoofDownLeft(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
            }
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_LEFT])
        {
            Selector.MoveLeft();
            if(Selector.GetPlayFieldX() <  WorldParts.ViewPort->VPMinX+3)
                WorldParts.ViewPort->Move(-TileWidth,0);
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_RIGHT])
        {
            Selector.MoveRight();
            if(Selector.GetPlayFieldX() > WorldParts.ViewPort->VPMaxX - 3)
                WorldParts.ViewPort->Move(TileWidth,0);
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_UP])
        {
            Selector.MoveUp();
            if(Selector.GetPlayFieldY() < WorldParts.ViewPort->VPMinY+3)
                WorldParts.ViewPort->Move(0,-TileWidth);
            Input->Delay();
        }

        if (Input->Ready() && Input->KeyboardHeld[BUT_DOWN])
        {
            Selector.MoveDown();
            if(Selector.GetPlayFieldY() > WorldParts.ViewPort->VPMaxY-3)
                WorldParts.ViewPort->Move(0,TileWidth);
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_X])
        {
            if(ShowPosition && ShowGrid)
                ShowGrid = !ShowGrid;
            else
            if(ShowPosition && !ShowGrid)
                ShowPosition = !ShowPosition;
            else
            if(!ShowPosition && ! ShowGrid)
               ShowGrid = !ShowGrid;
            else
            if(!ShowPosition && ShowGrid)
                ShowPosition = !ShowPosition;
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_Y])
        {
            if (WorldParts.ItemCount > 0)
            {
                sprintf(Tekst,"You are about to delete all parts\nin this level, are you sure\nyou want to do this?\n\nPress (%s) to delete, (%s) to cancel","A","B");
                if(AskQuestion(Tekst))
                {
                    LevelHasChanged = true;
                    WorldParts.RemoveAll();
                }
            }
            Input->Reset();
            Input->Delay();
        }

        if(Input->Ready() && Input->KeyboardHeld[BUT_B])
        {
            MinX = NrOfCols-1;
            MinY = NrOfRows-1;
            MaxX = 0;
            MaxY = 0;
            for (Teller = 0;Teller<WorldParts.ItemCount;Teller++)
            {
                if (WorldParts.Items[Teller]->GetPlayFieldX() < MinX)
                    MinX = WorldParts.Items[Teller]->GetPlayFieldX();
                if (WorldParts.Items[Teller]->GetPlayFieldY() < MinY)
                    MinY = WorldParts.Items[Teller]->GetPlayFieldY();
                if (WorldParts.Items[Teller]->GetPlayFieldX() > MaxX)
                    MaxX = WorldParts.Items[Teller]->GetPlayFieldX();
                if (WorldParts.Items[Teller]->GetPlayFieldY() > MaxY)
                    MaxY = WorldParts.Items[Teller]->GetPlayFieldY();
            }
            Xi = ((NrOfCols-1) / 2) - (MaxX + MinX) / 2;
            Yi = ((NrOfRows-1) / 2) - (MaxY + MinY) / 2;
            for (Teller = 0;Teller<WorldParts.ItemCount;Teller++)
            {
                WorldParts.Items[Teller]->SetPosition(WorldParts.Items[Teller]->GetPlayFieldX() + Xi,WorldParts.Items[Teller]->GetPlayFieldY() + Yi);
            }
            WorldParts.LimitVPLevel();
            //center viewport;
            WorldParts.ViewPort->SetViewPort((NrOfCols/2)-6,(NrOfRows/2)-6,(NrOfCols/2)+8,(NrOfRows/2)+8);
            Selector.SetPosition((NrOfCols/2),(NrOfRows/2));
            //if player found center viewport on player !
            for (Teller = 0;Teller<WorldParts.ItemCount;Teller++)
            {
                if(WorldParts.Items[Teller]->GetType() == IDPlayer)
                {
                    Selector.SetPosition(WorldParts.Items[Teller]->GetPlayFieldX(),WorldParts.Items[Teller]->GetPlayFieldY());
                    WorldParts.CenterVPOnPlayer();
                    break;
                }
            }
            WorldParts.ViewPort->SetVPLimit(0,0,NrOfCols-1,NrOfRows-1);
            if (Xi != 0 || Yi!=0)
                LevelHasChanged = true;
            Input->Delay();
        }

		SDL_framerateDelay(&Fpsman);
	}
	delete[] FileName;
	delete Input;
	SDL_FreeSurface(Tmp);
}

void UnloadMusic()
{
	if (GlobalSoundEnabled)
	{
		Mix_HaltMusic();
		if (Music[0])
			Mix_FreeMusic(Music[0]);
	}
}

void LoadSounds()
{
	if (GlobalSoundEnabled)
	{
		Sounds[SND_MENU] = Mix_LoadWAV("./blockdudefs/sound/menu.wav");
		Sounds[SND_SELECT] = Mix_LoadWAV("./blockdudefs/sound/select.wav");
		Sounds[SND_ERROR] = Mix_LoadWAV("./blockdudefs/sound/error.wav");
		Sounds[SND_STAGEEND] = Mix_LoadWAV("./blockdudefs/sound/stageend.wav");
	}
}



void UnloadSounds()
{
	int Teller;
	for (Teller=0;Teller<NrOfSounds;Teller++)
		if(Sounds[Teller])
			Mix_FreeChunk(Sounds[Teller]);
}

int main(int argc, char **argv)
{
    SDL_Surface *Tmp;
	srand((int) time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0)
	{
		printf("SDL Succesfully initialized\n");
        Screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,16, SDL_HWSURFACE );
        SDL_WM_SetCaption( "Blockdude 1.3 - Created By Willems Soft", NULL );
		if(Screen)
		{
		    printf("Succesfully Set %dx%dx16\n",WINDOW_WIDTH,WINDOW_HEIGHT);
		    Tmp = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
			SDL_FreeSurface(Tmp);
		    SDL_FillRect(Screen,NULL,SDL_MapRGB(Screen->format,0,0,0));
            SDL_ShowCursor(SDL_DISABLE);
			if (Mix_OpenAudio(22050,AUDIO_S16,MIX_DEFAULT_CHANNELS,1024) < 0)
			{
				GlobalSoundEnabled = false;
				printf("Failed to initialise sound!\n");
				printf("%s\n",Mix_GetError());
			}
			else
			{
				printf("Audio Succesfully initialised!\n");

				if (TTF_Init() == 0)
				{
					printf("Succesfully initialized TTF\n");
					font = TTF_OpenFont("./blockdudefs/fonts/font.ttf",9);
					BigFontSkin = TTF_OpenFont("./blockdudefs/fonts/font.ttf",16);
					BigFont = TTF_OpenFont("./blockdudefs/fonts/font2.ttf",20);
					MonoFont = TTF_OpenFont("./blockdudefs/fonts/font1.ttf",17);
					if (font && BigFont && MonoFont && BigFontSkin)
					{
						printf("Succesfully Loaded fonts\n");
						SDL_initFramerate(&Fpsman);
						SDL_setFramerate(&Fpsman,FPS);
						TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
						if (GlobalSoundEnabled)
							Music[0] = Mix_LoadMUS("./blockdudefs/music/title.mod");
						SearchForLevelPacks();
						SearchForSkins();
						LoadSounds();
						FindLevels();
						LoadSettings();
						LoadGraphics();
						while (GameState != GSQuit)
						{
							switch(GameState)
							{
								case GSTitleScreen :
									TitleScreen();
									break;
								case GSCredits :
									Credits();
									break;
								case GSGame :
									Game();
									break;
								case GSStageClear:
									break;
								case GSStageSelect:
									StageSelect();
									break;
								case GSLevelEditor:
									LevelEditor();
									break;
								case GSLevelEditorMenu:
									LevelEditorMode = true;
									LevelEditorMenu();
									break;
								default:
									break;
							}
						}
						SaveSettings();
						UnLoadGraphics();
						UnloadSounds();
						UnloadMusic();
						TTF_CloseFont(font);
						TTF_CloseFont(BigFont);
						TTF_CloseFont(MonoFont);
						TTF_CloseFont(BigFontSkin);
						MonoFont=NULL;
						font=NULL;
						BigFont=NULL;
					}
					else
					{
						printf("Failed to Load fonts\n");
					}
					TTF_Quit();
				}
				else
				{
					printf("Failed to initialize TTF\n");
				}
				Mix_CloseAudio();
			}
            SDL_FreeSurface(Screen);
            Screen=NULL;
		}
		else
		{
			printf("Failed to Set Videomode %dx%dx16\n",WINDOW_WIDTH, WINDOW_HEIGHT);
		}

		SDL_Quit();
	}
	else
	{
		printf("Couldn't initialise SDL!\n");
	}
	return 0;
}
