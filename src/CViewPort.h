#ifndef CVIEWPORT_H 
#define CVIEWPORT_H

#include "blockdude.h"

class CViewPort
{
    public:
        int VPMinX,VPMinY,VPMaxX,VPMaxY,MinScreenX,MinScreenY,MaxScreenX,MaxScreenY,Width,Height,VPLimitMinX,VPLimitMaxX,VPLimitMinY,VPLimitMaxY,BackgroundX,BackgroundY;
        CViewPort(int MinX,int MinY, int MaxX,int MaxY,int MinX2,int MinY2,int MaxX2,int MaxY2);
        void SetVPLimit(int MinX,int MinY, int MaxX,int MaxY);
        void Move(int Xi,int Yi);
        void SetViewPort(int MinX,int MinY, int MaxX,int MaxY);
        ~CViewPort();
};
#endif