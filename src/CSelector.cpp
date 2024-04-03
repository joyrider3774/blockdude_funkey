#include <SDL.h>
#include "CSelector.h"
#include "blockdude.h"
#include "CWorldParts.h"
#include "CWorldPart.h"

CSelector::CSelector(CWorldParts *List)
{
    PList = List;
	Selection = IDEmpty;
	Part = new CEmpty(0,0);
	Part->ParentList = PList;
	Part->Selected = true;
}

void CSelector::SetPosition(const int PlayFieldXin,const int PlayFieldYin)
{
    Part->SetPosition(PlayFieldXin,PlayFieldYin);
}

void CSelector::MoveUp()
{

	Part->SetPosition(Part->GetPlayFieldX(),Part->GetPlayFieldY()-1);
}

void CSelector::MoveDown()
{
	Part->SetPosition(Part->GetPlayFieldX(),Part->GetPlayFieldY()+1);
}

void CSelector::MoveRight()
{
	Part->SetPosition(Part->GetPlayFieldX() +1,Part->GetPlayFieldY());
}

void CSelector::MoveLeft()
{
	Part->SetPosition(Part->GetPlayFieldX() -1,Part->GetPlayFieldY());
}

void CSelector::IncSelection()
{
	int X,Y;
	Selection++;
	if (Selection > 26)
		Selection = 1;
	X = Part->GetPlayFieldX();
	Y = Part->GetPlayFieldY();
	delete Part;
	switch (Selection)
	{
		case IDEmpty:
			Part = new CEmpty(X,Y);
			break;
		case IDBox:
			Part = new CBox(X,Y);
			break;
		case IDPlayer:
			Part = new CPlayer(X,Y);
			break;
		case IDFloor:
			Part = new CFloor(X,Y);
			break;
        case IDExit:
			Part = new CExit(X,Y);
			break;
		case IDEarthGrassLeft:
            Part = new CEarthGrassLeft(X,Y);
			break;
		case IDEarthGrassRight:
            Part = new CEarthGrassRight(X,Y);
			break;
        case IDEarthLeft :
            Part = new CEarthLeft(X,Y);
			break;
        case IDEarthMiddle:
            Part = new CEarthMiddle(X,Y);
			break;
        case IDEarthRight:
            Part = new CEarthRight(X,Y);
			break;
        case IDFloatingFloor:
            Part = new CFloatingFloor(X,Y);
			break;
        case IDFloatingFloorLeft:
            Part = new CFloatingFloorLeft(X,Y);
			break;
        case IDFloatingFloorMiddle:
            Part = new CFloatingFloorMiddle(X,Y);
			break;
        case IDFloatingFloorRight:
            Part = new CFloatingFloorRight(X,Y);
			break;
        case IDFloorLeft:
            Part = new CFloorLeft(X,Y);
			break;
        case IDFloorRight:
            Part = new CFloorRight(X,Y);
			break;
        case IDTower:
            Part = new CTower(X,Y);
			break;
        case IDStartTower:
            Part = new CStartTower(X,Y);
			break;
        case IDTowerShaft:
            Part = new CTowerShaft(X,Y);
			break;
        case IDRoof1 :
            Part = new CRoof1(X,Y);
			break;
        case IDRoof2:
            Part = new CRoof2(X,Y);
			break;
        case IDRoofCornerLeft:
            Part = new CRoofCornerLeft(X,Y);
			break;
        case IDRoofCornerRight:
            Part = new CRoofCornerRight(X,Y);
			break;
        case IDRoofCornerBoth:
            Part = new CRoofCornerBoth(X,Y);
			break;
        case IDRoofDownRight:
            Part = new CRoofDownRight(X,Y);
			break;
        case IDRoofDownLeft:
            Part = new CRoofDownLeft(X,Y);
			break;

	}
	Part->ParentList = PList;
	Part->Selected = true;
}

void CSelector::DecSelection()
{
	int X,Y;
	Selection--;
	if (Selection < 1)
		Selection = 26;
	X = Part->GetPlayFieldX();
	Y = Part->GetPlayFieldY();
	delete Part;
	switch (Selection)
	{
		case IDEmpty:
			Part = new CEmpty(X,Y);
			break;
		case IDBox:
			Part = new CBox(X,Y);
			break;
		case IDPlayer:
			Part = new CPlayer(X,Y);
			break;
        case IDFloor:
			Part = new CFloor(X,Y);
			break;
        case IDExit:
			Part = new CExit(X,Y);
			break;
		case IDEarthGrassLeft:
            Part = new CEarthGrassLeft(X,Y);
			break;
		case IDEarthGrassRight:
            Part = new CEarthGrassRight(X,Y);
			break;
        case IDEarthLeft :
            Part = new CEarthLeft(X,Y);
			break;
        case IDEarthMiddle:
            Part = new CEarthMiddle(X,Y);
			break;
        case IDEarthRight:
            Part = new CEarthRight(X,Y);
			break;
        case IDFloatingFloor:
            Part = new CFloatingFloor(X,Y);
			break;
        case IDFloatingFloorLeft:
            Part = new CFloatingFloorLeft(X,Y);
			break;
        case IDFloatingFloorMiddle:
            Part = new CFloatingFloorMiddle(X,Y);
			break;
        case IDFloatingFloorRight:
            Part = new CFloatingFloorRight(X,Y);
			break;
        case IDFloorLeft:
            Part = new CFloorLeft(X,Y);
			break;
        case IDFloorRight:
            Part = new CFloorRight(X,Y);
			break;
        case IDTower:
            Part = new CTower(X,Y);
			break;
        case IDStartTower:
            Part = new CStartTower(X,Y);
			break;
        case IDTowerShaft:
            Part = new CTowerShaft(X,Y);
			break;
        case IDRoof1 :
            Part = new CRoof1(X,Y);
			break;
        case IDRoof2:
            Part = new CRoof2(X,Y);
			break;
        case IDRoofCornerLeft:
            Part = new CRoofCornerLeft(X,Y);
			break;
        case IDRoofCornerRight:
            Part = new CRoofCornerRight(X,Y);
			break;
        case IDRoofCornerBoth:
            Part = new CRoofCornerBoth(X,Y);
			break;
        case IDRoofDownRight:
            Part = new CRoofDownRight(X,Y);
			break;
        case IDRoofDownLeft:
            Part = new CRoofDownLeft(X,Y);
			break;

	}
	Part->ParentList = PList;
	Part->Selected = true;
}

void CSelector::Draw(SDL_Surface *Surface)
{
	Part->Draw(Surface);
}

int CSelector::GetSelection()
{
	return Selection;
}

int CSelector::GetPlayFieldX()
{
	return Part->GetPlayFieldX();
}

int CSelector::GetPlayFieldY()
{
	return Part->GetPlayFieldY();
}

CSelector::~CSelector()
{
	delete Part;
}
