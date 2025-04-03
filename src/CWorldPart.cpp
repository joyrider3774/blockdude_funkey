#include "CWorldPart.h"
#include "blockdude.h"

CWorldPart::CWorldPart(const int PlayFieldXin,const int PlayFieldYin)
{
	PlayFieldX=PlayFieldXin;
	PlayFieldY=PlayFieldYin;
	Xi=0;
	Yi=0;
	X=PlayFieldXin*TileWidth;
	Y=PlayFieldYin*TileHeight;
	Type=0;
	MoveDelay=0;
	MoveDelayCounter=0;
	IsMoving = false;
	MoveSpeed=0;
	Image = NULL;
	ParentList = 0;
	AnimPhase=0;
	Selected = false;
	FirstArriveEventFired = false;
	Z=0;
	Group=0;
}

CWorldPart::~CWorldPart()
{

}


void CWorldPart::Draw(SDL_Surface* Surface)
{
	if (*Image)
	{
		Event_BeforeDraw();
		SDL_Rect SrcRect,DstRect;
		SrcRect.x = AnimPhase * TileWidth;
		SrcRect.y = 0;
		SrcRect.w = TileWidth;
		SrcRect.h = TileHeight;
		if(ParentList)
		{
			DstRect.x = X- ParentList->ViewPort->MinScreenX;
			DstRect.y = Y- ParentList->ViewPort->MinScreenY;
		}
		else
		{
			DstRect.x = X;
			DstRect.y = Y;
		}
		DstRect.w = TileWidth;
		DstRect.h = TileHeight;
		SDL_BlitSurface((*Image),&SrcRect,Surface,&DstRect);
	}
	if (Selected)
	{
		if (ParentList)
		{
			boxRGBA(Surface,X- ParentList->ViewPort->MinScreenX,Y- ParentList->ViewPort->MinScreenY,X- ParentList->ViewPort->MinScreenX+TileWidth-1,Y- ParentList->ViewPort->MinScreenY+TileHeight-1,0,0,200,15);
			rectangleRGBA(Surface,X- ParentList->ViewPort->MinScreenX,Y- ParentList->ViewPort->MinScreenY,X- ParentList->ViewPort->MinScreenX+TileWidth-1,Y- ParentList->ViewPort->MinScreenY+TileHeight-1,0,0,255,50);
		}
		else
		{
			boxRGBA(Surface,X,Y,X+TileWidth-1,Y+TileHeight-1,0,0,200,15);
			rectangleRGBA(Surface,X,Y,X+TileWidth-1,Y+TileHeight-1,0,0,255,50);
		}
	}
}

void CWorldPart::Move()
{
	if (!FirstArriveEventFired)
	{
		Event_ArrivedOnNewSpot();
		FirstArriveEventFired=true;
	}
	if (IsMoving)
	{
		if (MoveDelayCounter == MoveDelay)
		{
			X += Xi;
			Y += Yi;
			Event_Moving(X,Y,Xi,Yi);
			if ((X == PlayFieldX * TileWidth) && (Y == PlayFieldY * TileHeight))
			{
				IsMoving = false;
				Xi = 0;
				Yi = 0;
				Event_ArrivedOnNewSpot();
			}
			MoveDelayCounter = -1;
		}
		MoveDelayCounter++;
	}
	else
		if (MoveQue.size() > 0)
		{
			if (CanMoveTo(MoveQue.back().X,MoveQue.back().Y))
			{
				MoveTo(MoveQue.back().X,MoveQue.back().Y,false);
				MoveQue.pop_back();
			}
			else
				MoveQue.clear();
		}
}

void CWorldPart::MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards)
{
	if(!IsMoving)
	{
		if((PlayFieldXin != PlayFieldX) || (PlayFieldYin != PlayFieldY))
			if(this->CanMoveTo(PlayFieldXin,PlayFieldYin) || BackWards)
			{
				PlayFieldX = PlayFieldXin;
				PlayFieldY = PlayFieldYin;
				if(X < PlayFieldX*TileWidth)
					Xi = MoveSpeed;
				if(X > PlayFieldX*TileWidth)
					Xi = -MoveSpeed;
				if(Y > PlayFieldY*TileHeight)
					Yi = -MoveSpeed;
				if(Y < PlayFieldY*TileHeight)
					Yi = MoveSpeed;
				IsMoving = true;
				Event_LeaveCurrentSpot();
			}
	}
}

void CWorldPart::SetPosition(const int PlayFieldXin,const int PlayFieldYin)
{
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		PlayFieldX=PlayFieldXin;
		PlayFieldY=PlayFieldYin;
		X=PlayFieldXin*TileWidth;
		Y=PlayFieldYin*TileHeight;
		//Event_ArrivedOnNewSpot();
	}
}

void CWorldPart::AddToMoveQue(int PlayFieldXIn,int PlayFieldYIn)
{
	SPoint Temp;
	Temp.X = PlayFieldXIn;
	Temp.Y = PlayFieldYIn;
	if (MoveQue.size() == 0)
		MoveQue.push_back(Temp);
	else
		MoveQue.insert(MoveQue.begin(),Temp);
}

bool CWorldPart::MovesInQue() 
{ 
	return (MoveQue.size() > 0);
}

CBox::CBox(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGBox;
	Type = IDBox;
	MoveDelay = 0;
	MoveSpeed = 2;
	PickupLeft = false;
	PickupRight = false;
	AttachedToPlayer = false;
	Z = ZBox;
	Group = GroupBox;
}

void CBox::Event_ArrivedOnNewSpot()
{
    int Teller = 0;
    bool PlayerBelow = false;
    if (ParentList)
    {
        //see if there is a Player below the block
            for (Teller=0;Teller< ParentList->ItemCount;Teller++)
            {
                if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY+1) &&
                    (ParentList->Items[Teller]->GetType() == IDPlayer))
                {
                     PlayerBelow = true;
                     break;

                }
            }
        if (PlayerBelow)
            this->AttachToPlayer((CPlayer *)ParentList->Items[Teller]);
        else
            if(this->AttachedToPlayer)
                this->DeattachFromPlayer();

    }
}

void CBox::Move()
{
    //move moet hergedaan worden zodat de player als er op z'n start positie geen block onder hem zit toch naar beneden valt en niet blijft staan
    bool SomethingBelow = false;
    int Teller = 0;

        if (ParentList)
        {
            //see if there is something below the block
            for (Teller=0;Teller< ParentList->ItemCount;Teller++)
            {
                if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY+1) &&
                    ((ParentList->Items[Teller]->GetGroup() == GroupFloor) ||
                    (ParentList->Items[Teller]->GetGroup() == GroupBox) || (ParentList->Items[Teller]->GetGroup() == GroupPlayer)))
                {
                     SomethingBelow = true;
                     break;
                }
            }
        }
        //if nothing is below and there are no moves in the queue move it down
        if ((!SomethingBelow) && (!this->MovesInQue()) )
        {
            if(AttachedToPlayer)
            {
                if(!Player->IsMoving)
                {
                    AttachedToPlayer = false;
                    Player = NULL;
                }
            }
            MoveTo(PlayFieldX,PlayFieldY+1,false);
        }

        if (!FirstArriveEventFired)
 		{
			Event_ArrivedOnNewSpot();
			FirstArriveEventFired=true;
 		}
 		if (IsMoving)
		{
			if (MoveDelayCounter == MoveDelay)
			{
				X += Xi;
				Y += Yi;
				Event_Moving(X,Y,Xi,Yi);
				if ((X == PlayFieldX * TileWidth) && (Y == PlayFieldY * TileHeight))
				{
					IsMoving = false;
					Xi = 0;
					Yi = 0;
					Event_ArrivedOnNewSpot();
				}
				MoveDelayCounter = -1;
			}
			MoveDelayCounter++;
		}
		else
            if (MoveQue.size() > 0)
            {
                if (CanMoveTo(MoveQue.back().X,MoveQue.back().Y))
                {
                    MoveTo(MoveQue.back().X,MoveQue.back().Y,false);
                    MoveQue.pop_back();
                }
                else
                    MoveQue.clear();
            }

 	}


bool CBox::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller;
	bool Result = true;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		if (ParentList)
		{
		        for (Teller=0;Teller<ParentList->ItemCount;Teller++)
                    if(((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin)))
                     if((ParentList->Items[Teller]->GetGroup() == GroupFloor) || (ParentList->Items[Teller]->GetGroup() == GroupBox))
                {

                    Result = false;
                    break;
                }

		}
		else
		{
		    return false;
		}
	}
	else
	{
		return false;
	}
	return Result;
}

CPlayer::CPlayer(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image=&IMGPlayer;
	AnimBase=AnimBaseLeft;
	AnimPhase=0;
	AnimPhases=4;
	AnimCounter = 1;
	AnimDelay = 8; //als er eerst gedrawed wordt en dan gemoved, als het andersom is gebeurd er 1 tje
	MoveDelay = 0;
	MoveSpeed = 2;
	AnimDelayCounter =0;
	Type = IDPlayer;
	NeedToMoveLeft = false;
	NeedToMoveRight = false;
	Z = ZPlayer;
	Group = GroupPlayer;
	HalfWindowWidth = ORIG_WINDOW_WIDTH / 2;
	HalfWindowHeight = ORIG_WINDOW_HEIGHT / 2;
	while(HalfWindowWidth % TileWidth !=0)
		HalfWindowWidth++;	
	while(HalfWindowHeight % TileHeight !=0)
		HalfWindowHeight++;	
}

void CPlayer::Move()
{
    int Teller;
	//AnimPhase = 0;
	bool FloorFound = false;
	//printf("Arrive Event fired\n");
	if (ParentList)
	{
		//printf("Parent List Set\n");
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
            if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY+1) &&
              ((ParentList->Items[Teller]->GetGroup() == GroupFloor) || (ParentList->Items[Teller]->GetGroup() == GroupBox)))
            {
                AnimPhases=4;
                FloorFound = true;
                break;
            }
		}
	}

	if (!FloorFound && ParentList)
    {
        AnimPhases=1;
        AnimCounter=0;

        MoveTo(PlayFieldX,PlayFieldY+1,false);
    }

 	if (!FirstArriveEventFired)
 	{
		Event_ArrivedOnNewSpot();
		FirstArriveEventFired=true;
 	}
 	if (IsMoving)
	{
		if (MoveDelayCounter == MoveDelay)
		{
			X += Xi;
			Y += Yi;
			Event_Moving(X,Y,Xi,Yi);
			if ((X == PlayFieldX * TileWidth) && (Y == PlayFieldY * TileHeight))
			{
				IsMoving = false;
				Xi = 0;
				Yi = 0;
				Event_ArrivedOnNewSpot();
			}
			MoveDelayCounter = -1;
		}
		MoveDelayCounter++;
	}
	else
        if (MoveQue.size() > 0)
        {
            if (CanMoveTo(MoveQue.back().X,MoveQue.back().Y))
            {
                MoveTo(MoveQue.back().X,MoveQue.back().Y,false);
                MoveQue.pop_back();
            }
            else
                MoveQue.clear();
        }
}

bool CPlayer::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller,Teller2;
	bool Result = true,CanJump=false,FloorFound =false;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{

	    if (ParentList)
		{
		    //if we move up
            if(PlayFieldYin - PlayFieldY   < 0 )
		    //check to see
		    for (Teller=0;Teller<ParentList->ItemCount;Teller++)
			{


                    //if a block is left or right besides the player to jump onto,
                    if((((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX-1) && (AnimBase == AnimBaseLeft)) || ((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX+1) && (AnimBase==AnimBaseRight)) ) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY))
                    {
                        CanJump = true;
                        //if it is a box check if the box has another box or a floor below it (io check if it's not falling down)
                        if(ParentList->Items[Teller]->GetType() == IDBox)
                        {
                            for (Teller2=0;Teller2< ParentList->ItemCount;Teller2++)
                                {
                                    if( (((ParentList->Items[Teller2]->GetPlayFieldX() == PlayFieldX -1) && (AnimBase == AnimBaseLeft)) || ((ParentList->Items[Teller2]->GetPlayFieldX() == PlayFieldX +1) && (AnimBase == AnimBaseRight))) &&
                                        (ParentList->Items[Teller2]->GetPlayFieldY() == PlayFieldY+1) && ((ParentList->Items[Teller2]->GetGroup() == GroupFloor) || (ParentList->Items[Teller2]->GetGroup() == GroupBox)))
                                    {
                                        FloorFound = true;
                                        break;
                                    }
                                }
                                if(!FloorFound)
                                    CanJump = false;
                        }

//                        printf("Can jump\n");
                    }
                    //if the place on top the block is not empty
                    if((((((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX-1) && (AnimBase == AnimBaseLeft)) || (((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX+1)) && (AnimBase==AnimBaseRight)))) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin)))
                    {
                        //it's not so we can't move / jump on it
                        if(ParentList->Items[Teller]->GetType() != IDExit)
                        {
                            Result = false;
                            break;
                        }
                    }


                // if the result is always true we will have checked all boxes to see if they are attached or not
                //check for an attached box
			}

			for (Teller=0;Teller<ParentList->ItemCount;Teller++)
			{

                if (ParentList->Items[Teller]->GetGroup() == GroupBox)
                    if (((CBox *)ParentList->Items[Teller])->IsAttachedToPlayer())
                    {
                        if(PlayFieldYin-PlayFieldY == 0)
                        {
                        }
                        if(ParentList->Items[Teller]->MovesInQue())
                        {
                            Result = false;
                            break;
                        }
                    }
			}
			//if we can still move to it after the jump check
			if(Result)
		    for (Teller=0;Teller<ParentList->ItemCount;Teller++)
            {
			    //if there is an item on the new position
				if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
				{
				    //and it's a wall or floor, we can't move to it
					if((ParentList->Items[Teller]->GetGroup() == GroupFloor)  )
					{
						Result = false;
						break;
					}
					else
					// it's a box
					if (ParentList->Items[Teller]->GetGroup() == GroupBox  )
					{
					    //we're moving horizontal so we can't move into the box
					    if (PlayFieldYin-PlayFieldY == 0)
					    {
                            Result = false;
                            break;
					    }
					}

                }


			}
		}
	}
	else
		Result = false;
    //if we want to move up
	if(PlayFieldYin - PlayFieldY   < 0 )
        return (CanJump && Result);
    else
        return Result;
}
void CPlayer::Event_BeforeDraw()
{
	if (IsMoving)
	{
		AnimPhase = AnimBase + AnimCounter;
		AnimDelayCounter++;
		if (AnimDelayCounter ==AnimDelay)
		{
			AnimDelayCounter = 0;
			AnimCounter++;
			if (AnimCounter == AnimPhases)
				AnimCounter = 0;
		}
	}
	else
		AnimPhase = AnimBase;
}

void CPlayer::Event_ArrivedOnNewSpot()
{
    //floorfound check moet hier gebeuren zodat wanneer men van een "blok springt" naar benede valt en niet blijft voortschuiven
    int Teller;
	bool FloorFound = false;
	if (ParentList)
	{
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
            if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY+1) &&
              ((ParentList->Items[Teller]->GetGroup() == GroupFloor) || (ParentList->Items[Teller]->GetGroup() == GroupBox) ))
            {
                AnimPhases = 4;
                FloorFound = true;
                break;
            }
		}
	}

    if (NeedToMoveLeft)
    {
        MoveTo(PlayFieldX-1,PlayFieldY,false);
        NeedToMoveLeft = false;
    }
    if (NeedToMoveRight)
    {
        MoveTo(PlayFieldX+1,PlayFieldY,false);

        NeedToMoveRight = false;
    }

    if (!FloorFound && ParentList)
    {
        AnimCounter = 0;
        AnimPhases = 1;
        MoveTo(PlayFieldX,PlayFieldY+1,false);
    }
}

void CPlayer::Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi,int ScreenYi)
{
    if((ScreenPosX > (ParentList->ViewPort->MaxScreenX) - HalfWindowWidth) && (Xi > 0))
        ParentList->ViewPort->Move(Xi,Yi);
    if((ScreenPosX < (ParentList->ViewPort->MaxScreenX) - HalfWindowWidth) && (Xi < 0))
        ParentList->ViewPort->Move(Xi,Yi);
    if((ScreenPosY > (ParentList->ViewPort->MaxScreenY) - HalfWindowHeight) && (Yi > 0))
        ParentList->ViewPort->Move(Xi,Yi);
    if((ScreenPosY < (ParentList->ViewPort->MaxScreenY) - HalfWindowHeight) && (Yi < 0))
        ParentList->ViewPort->Move(Xi,Yi);

}
void CPlayer::MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards)
{
 	int Teller;
 	if(!IsMoving)
	{

		if(this->CanMoveTo(PlayFieldXin,PlayFieldYin) || BackWards)
		{
			PlayFieldX = PlayFieldXin;
			PlayFieldY = PlayFieldYin;
			//Right
			if(X < PlayFieldX*TileWidth)
			{
				Xi = MoveSpeed;
				if(BackWards)
				{
					AnimBase = AnimBaseRight;
				}
				else
				{
				    AnimBase = AnimBaseRight;
					if (ParentList)
					{
						for(Teller=0;Teller<ParentList->ItemCount;Teller++)
						{
						    //check for a box on top of the player.
							if(((ParentList->Items[Teller]->GetGroup() == GroupBox) ) && ((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX-1) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY-1)))
							{
							    //if one is found move it to the players X position
                                ParentList->Items[Teller]->MoveTo(PlayFieldX,PlayFieldY-1,false);
								break;
							}
						}
					}
				}
            }
			//left
			if(X > PlayFieldX*TileWidth)
			{
				Xi = -MoveSpeed;
				if(BackWards)
				{
					AnimBase = AnimBaseLeft;
				}
				else
				{
				    AnimBase = AnimBaseLeft;
					if (ParentList)
					{
						for(Teller=0;Teller<ParentList->ItemCount;Teller++)
						{
						    //check for a box on top of the player
							if(((ParentList->Items[Teller]->GetGroup() == GroupBox)) && ((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX+1) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY-1)))
							{
                                ParentList->Items[Teller]->MoveTo(PlayFieldX,PlayFieldY-1,false);
								break;
							}
						}
					}
				}
			}
			//up
			if(Y > PlayFieldY*TileHeight)
			{
				Yi = -MoveSpeed;
				//animbase = 0 is start of animation for going left, 2 is start for going right
				if (AnimBase == AnimBaseLeft)
				{
                    NeedToMoveLeft = true;
                    AnimBase = AnimBaseLeftJump;
				}
                else
                    if(AnimBase == AnimBaseRight)
                    {
                        AnimBase = AnimBaseRightJump;
                        NeedToMoveRight = true;
                    }
				if(!BackWards)
				{
					if (ParentList)
					{
						for(Teller=0;Teller<ParentList->ItemCount;Teller++)
						{
							if(((ParentList->Items[Teller]->GetGroup() == GroupBox)) && ((PlayFieldY == ParentList->Items[Teller]->GetPlayFieldY())  && (ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX)))
							{
								ParentList->Items[Teller]->MoveTo(PlayFieldX,PlayFieldY-1,false);
								break;
							}
                        }
					}
				}
			}
			//down
			if(Y < PlayFieldY*TileHeight)
			{
				Yi = MoveSpeed;
				if(BackWards)
				{
				}
				else
					if(ParentList)
					{
						for(Teller=0;Teller<ParentList->ItemCount;Teller++)
						{
							if(((ParentList->Items[Teller]->GetGroup() == GroupBox)) && ((ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY-1)  && (ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX )))
							{
								ParentList->Items[Teller]->MoveTo(PlayFieldX,PlayFieldY+1,false);
								break;
							}
						}
					}
			}

			IsMoving = true;
		}
		else
		{
		    //Right
			if (PlayFieldXin > PlayFieldX)
			{
				AnimBase = AnimBaseRight;
			}
			//Left
			if (PlayFieldXin < PlayFieldX)
			{
				AnimBase = AnimBaseLeft;
			}
            AnimPhase = AnimBase + AnimCounter;
		}

 	}
 }


bool CBox::IsAttachedToPlayer()
{
	return AttachedToPlayer;
}
void CBox::AttachToPlayer(CPlayer *PlayerIn)
{
	Player = PlayerIn;
	AttachedToPlayer = true;
}
void CBox::DeattachFromPlayer()
{
	Player = NULL;
	AttachedToPlayer = false;
}