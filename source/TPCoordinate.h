#pragma once

#include "TPConst.h"
#include <vector>
#include "TPEnvironment.h"
#include "TPView.h"

typedef enum TPCoord_RP_T
{
    TP_CURVE,
    TP_POINT
};
class TPCoordinate
{
private:

	int   mId;
	char* mName;
	char* mXName;
	char* mYName;
	
	float mMinX;
	float mMaxX;
	float mMinY;
	float mMaxY;

    TPView mView;

    std::vector<TPPoint> mDrawingPoints;
    TPCoord_RP_T mDrawingType;
    float mDrawingSize;

    float mMeshStartX;
    float mMeshEndX;
    float mMeshStepX;

    float mMeshStartY;
    float mMeshEndY;
    float mMeshStepY;	
	
public:
    TPCoordinate(int id, char * name = nullptr )
        : mId(id), mName(name), mXName(nullptr), mYName(nullptr)
        , mMinX(0), mMaxX(10), mMinY(0), mMaxY(10) 
    { }
    
    void SetAnchor(float minX, float maxX, float minY, float maxY)
    {
        mMinX = minX; mMaxX = maxX;
        mMinY = minY; mMaxY = maxY;
        mView.SetAnchor(minX, maxX, minY, maxY);
    }

    float GetMinX()const { return mMinX; }
    float GetMaxX()const { return mMaxX; }
    float GetMinY()const { return mMinY; }
    float GetMaxY()const { return mMaxY; }

    TPView& GetView(){ return mView; }
    char* GetName() { return mName; }


    void SetDrawingPoints(TPCoord_RP_T type, float size, TPPoint* pts, unsigned szPts);
    void RenderPoints();

    void RenderMesh();
    void RenderReferenceValue();
};