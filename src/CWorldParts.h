#ifndef CWORLDPARTS_H
#define CWORLDPARTS_H

#include <SDL.h>
#include "blockdude.h"
#include "CViewPort.h"

class CWorldPart;
class CWorldParts
{
 private:

 	void Sort();
 	bool DisableSorting;
 public:
    CViewPort *ViewPort;
 	CWorldPart *Items[NrOfRows*NrOfCols];
 	int ItemCount;
 	CWorldParts();
 	void Add(CWorldPart *WorldPart);
 	void CenterVPOnPlayer();
 	void LimitVPLevel();
 	void Move();
 	void HistoryAdd();
 	void HistoryGoBack();
 	void Draw(SDL_Surface *Surface);
 	void Remove(int PlayFieldXin,int PlayFieldYin);
 	void Remove(int PlayFieldXin,int PlayFieldYin,int Type);
 	void RemoveAll();
	void Save(const char *Filename);
	void Load(const char *Filename);
 	~CWorldParts();
};

#endif