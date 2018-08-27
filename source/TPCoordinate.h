#pragma once

#include "TPConst.h"
#include <vector>
#include "TPEnvironment.h"
#include "TPView.h"

typedef enum TPCoord_RP_T
{
    RP_CURVE,
    RP_POINT
};

typedef enum TPCoord_XY_T
{
	XY_FLOAT,
	XY_INT,
	XY_DATE
};

class TPCoordinate
{	
public:
    TPCoordinate(int id, char * name = nullptr )
        : mId(id), mName(name), mXName(nullptr), mYName(nullptr)
        , mMinX(0), mMaxX(10), mMinY(0), mMaxY(10) 
		, mXType(XY_FLOAT), mYType(XY_FLOAT)
    { }
    
	// X Anchor initialized with Date value.
	void SetXAnchor(TPDate minX, TPDate maxX);

	// X Anchor initialized with double value.
	void SetXAnchor(double minX, double maxX);

	// X Anchor initialized with in value.
	void SetXAnchor(int minX, int maxX);

	// Y Anchor initialized with double value.
	void SetYAnchor(double minY, double maxY);

	// Y Anchor intialized with int value.
	void SetYAnchor(int minY, int maxY);

    float GetMinX()const { return mMinX; }
    float GetMaxX()const { return mMaxX; }
    float GetMinY()const { return mMinY; }
    float GetMaxY()const { return mMaxY; }

    TPView& GetView(){ return mView; }
    char* GetName() { return mName; }

	// Set Points here to show it!
    void SetDrawingPoints(TPCoord_RP_T type, float size, TPPoint* pts, unsigned szPts);
	
	// You can either use this method for easily call from JAR. vector<TPDate> is easy to get from TPDate::GetVector().
	template<typename T, typename P>
	void SetDrawingPoints(TPCoord_RP_T type, float size, std::vector<T> xVecValues, std::vector<P> yVecValues)
	{
		mDrawingType = type;
		mDrawingSize = size;

		int xSize = xVecValues.size();
		int ySize = yVecValues.size();
		if (xSize > ySize)
		{
			return;
		}
		for (int i = 0; i < xSize; ++i)
		{
			mDrawingPoints.push_back(TPPoint(xVecValues[i], yVecValues[i]));
		}
	}
    

	void RenderPoints();
    void RenderMesh();
    void RenderReferenceValue();

	int PointClicked(TPPoint p);

private:

	float GetSuitableXStep();
	float GetSuitableYStep();

	int   mId;
	char* mName;
	char* mXName;
	char* mYName;

	float mMinX;
	float mMaxX;
	float mMinY;
	float mMaxY;

	TPCoord_XY_T mXType;
	TPCoord_XY_T mYType;

	TPView mView;

	std::vector<TPPoint> mDrawingPoints;
	TPCoord_RP_T mDrawingType;
	float mDrawingSize;

	int mClickedPoints;
};