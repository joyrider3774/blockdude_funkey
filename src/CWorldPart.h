#ifndef CWORLDPART_H
#define CWORLDPART_H

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <vector>
#include "blockdude.h"
#include "CWorldParts.h"

using namespace std;

class CWorldPart
{
 protected:
    int MoveDelayCounter;
    bool FirstArriveEventFired;
 	int Type,MoveSpeed,MoveDelay,Xi,Yi,X,Y,AnimPhase,PlayFieldX,PlayFieldY,Z,Group;
 	bool BHistory;
 	SDL_Surface ** Image;
 	vector<SPoint> MoveQue;
 public:
    CWorldParts *ParentList;
 	bool IsMoving;
	bool Selected;
	void AddToMoveQue(int PlayFieldXIn,int PlayFieldYIn);
    bool MovesInQue();
 	CWorldPart(const int PlayFieldXin,const int PlayFieldYin);
 	int GetGroup() {return Group;};
 	int GetType() {return Type;};
 	int GetX() {return X;}
 	int GetY() {return Y;}
 	int GetPlayFieldX() {return PlayFieldX;}
 	int GetPlayFieldY() {return PlayFieldY;}
 	int GetZ() {return Z;}
 	int GetAnimPhase() {return AnimPhase;}
 	void SetAnimPhase(int AnimPhaseIn) { AnimPhase = AnimPhaseIn;}
 	virtual void Event_ArrivedOnNewSpot(){}
 	virtual void Event_BeforeDraw() {}
 	virtual void Event_LeaveCurrentSpot() {}
 	virtual void Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi) {}
 	void SetPosition(const int PlayFieldXin,const int PlayFieldYin);
 	virtual void MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards);
 	virtual bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin) {return false;}
 	virtual void Move();
 	virtual void Draw(SDL_Surface* Surface);
 	virtual ~CWorldPart();
};

class CPlayer : public CWorldPart
{
 private:
    bool NeedToMoveLeft;
    bool ItemAttached;
    bool NeedToMoveRight;
 	int AnimCounter,AnimBase,AnimDelay,AnimDelayCounter,AnimPhases,HalfWindowWidth, HalfWindowHeight;
 	void Event_BeforeDraw();
 	void Event_ArrivedOnNewSpot();
 	void Move();
 	void Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi);
 public:
    int  GetAnimBase() { return AnimBase;};
 	bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	void MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards);
 	CPlayer(const int PlayFieldXin,const int PlayFieldYin);
};

class CBox : public CWorldPart
{
 private:
 	bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	bool PickupLeft,PickupRight;
 	void Event_ArrivedOnNewSpot();
 	void Move();
 	CPlayer *Player;
 	bool AttachedToPlayer;
 public:
    bool IsAttachedToPlayer();
	void AttachToPlayer(CPlayer *PlayerIn);
    void DeattachFromPlayer();
	CBox(const int PlayFieldXin,const int PlayFieldYin);
};

class CEmpty : public CWorldPart
{
 public:
	CEmpty(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGEmpty;
		Type = IDEmpty;
	}
};

class CRoof1 : public CWorldPart
{
 public:
	CRoof1(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGRoof1;
		Type = IDRoof1;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CRoof2 : public CWorldPart
{
 public:
	CRoof2(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGRoof2;
		Type = IDRoof2;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CRoofCornerLeft : public CWorldPart
{
 public:
	CRoofCornerLeft(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGRoofCornerLeft;
		Type = IDRoofCornerLeft;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CRoofCornerRight : public CWorldPart
{
 public:
	CRoofCornerRight(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGRoofCornerRight;
		Type = IDRoofCornerRight;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CRoofDownLeft : public CWorldPart
{
 public:
	CRoofDownLeft(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGRoofDownLeft;
		Type = IDRoofDownLeft;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CRoofDownRight : public CWorldPart
{
 public:
	CRoofDownRight(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGRoofDownRight;
		Type = IDRoofDownRight;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CRoofCornerBoth : public CWorldPart
{
 public:
	CRoofCornerBoth(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGRoofCornerBoth;
		Type = IDRoofCornerBoth;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CExit : public CWorldPart
{
 public:
	CExit(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGExit;
		Type = IDExit;
		Z = ZExit;
		Group = GroupExit;
	}
};

class CFloor : public CWorldPart
{
 public:
	CFloor(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGFloor;
		Type = IDFloor;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CFloorLeft : public CWorldPart
{
 public:
	CFloorLeft(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGFloorLeft;
		Type = IDFloorLeft;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CFloorRight : public CWorldPart
{
 public:
	CFloorRight(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGFloorRight;
		Type = IDFloorRight;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CEarthGrassLeft : public CWorldPart
{
 public:
	CEarthGrassLeft(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGEarthGrassLeft;
		Type = IDEarthGrassLeft;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CEarthGrassRight : public CWorldPart
{
 public:
	CEarthGrassRight(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGEarthGrassRight;
		Type = IDEarthGrassRight;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CEarthLeft : public CWorldPart
{
 public:
	CEarthLeft(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGEarthLeft;
		Type = IDEarthLeft;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CEarthRight : public CWorldPart
{
 public:
	CEarthRight(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGEarthRight;
		Type = IDEarthRight;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CEarthMiddle : public CWorldPart
{
 public:
	CEarthMiddle(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGEarthMiddle;
		Type = IDEarthMiddle;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CFloatingFloor : public CWorldPart
{
 public:
	CFloatingFloor(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGFloatingFloor;
		Type = IDFloatingFloor;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CFloatingFloorLeft : public CWorldPart
{
 public:
	CFloatingFloorLeft(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGFloatingFloorLeft;
		Type = IDFloatingFloorLeft;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CFloatingFloorMiddle : public CWorldPart
{
 public:
	CFloatingFloorMiddle(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGFloatingFloorMiddle;
		Type = IDFloatingFloorMiddle;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CFloatingFloorRight : public CWorldPart
{
 public:
	CFloatingFloorRight(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGFloatingFloorRight;
		Type = IDFloatingFloorRight;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CStartTower : public CWorldPart
{
 public:
	CStartTower(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGStartTower ;
		Type = IDStartTower ;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CTower : public CWorldPart
{
 public:
	CTower(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGTower;
		Type = IDTower;
		Z = ZFloor;
		Group = GroupFloor;
	}
};

class CTowerShaft : public CWorldPart
{
 public:
	CTowerShaft(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
	{
		Image = &IMGTowerShaft;
		Type = IDTowerShaft;
		Z = ZFloor;
		Group = GroupFloor;
	}
};


#endif