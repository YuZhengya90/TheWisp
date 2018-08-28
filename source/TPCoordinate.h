#pragma once
#include <vector>
#include "TPConst.h"
#include "TPEnvironment.h"
#include "TPView.h"

typedef enum TPCoord_RP_T
{
    RP_CURVE = 0x0001,
    RP_POINT = 0x0002
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
		, mXType(XY_FLOAT), mYType(XY_FLOAT), mClickedPoints(-1)
		, mEnableCrossLine(false), mEnableCurve(false), mEnableMesh(false)
		, mDraw(false)
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
		mDrawingType = 0;
		mDrawingType |= (int)type;
		mDrawingType |= mEnableCurve ? (int)RP_CURVE : 0;

		mDrawingSize = size;

		int xSize = xVecValues.size();
		int ySize = yVecValues.size();
		if (xSize > ySize)
		{
			return;
		}
		mDrawingPoints.clear();
		for (int i = 0; i < xSize; ++i)
		{
			mDrawingPoints.push_back(TPPoint(xVecValues[i], yVecValues[i]));
		}

		mClickedPoints = -1;
	}

	//           ------------------                   ----------------------
	//           |   Advice       |                   |20180101~20180201   |
	//           ------------------                   ----------------------
	//           ------------------------------------------------------------
	//           |  SalePrice       |    PurchaseQuantity     |    Profit   |
	//           ------------------------------------------------------------
	//           |   5.99           |         1130            |     5320    |
	//           ------------------------------------------------------------
	//
	//           Advice = caption 
	//           from ~ to
	//           rows = 2 cols = 3 titles = {"SalePrice", "PurchaseQuantitiy", "Profit"}, values = {5.99, 1130, 5320}

	void SetTable(const std::string& caption, TPDate from, TPDate to, 
		unsigned rows, unsigned cols, /*rows is the real row (include titles.)*/
		std::vector<std::string> titles, std::vector<double> values);
    

	void DrawPoints(bool bDraw) { mDraw = bDraw; }
	bool EnableDrawPoints() const {
		return mDraw;
	}

	void RenderPoints();
    void RenderMesh();
	void RenderCrossLine();
    void RenderReferenceValue();
	

	int HoverPoint(TPPoint p);
	
	bool GetEnableMesh() const {
		return mEnableMesh;
	}

	bool GetEnableCrossLine() const {
		return mEnableCrossLine;
	}

	bool GetEnableCurve()  const {
		return mEnableCurve;
	}

	void SetEnableMesh(bool enable) {
		mEnableMesh = enable;
	}

	void SetEnableCrossLine(bool enable) {
		mEnableCrossLine = enable;
	}

	void SetEnableCurve(bool enable){
		mEnableCurve = enable;

		if (mEnableCurve)
		{
			mDrawingType = (int)mDrawingType | RP_CURVE;
		}
		else
		{
			mDrawingType = (int)mDrawingType & (~RP_CURVE);
		}
	}

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
	int mDrawingType;
	float mDrawingSize;

	int mClickedPoints;

	float mMeshMinX;
	float mMeshMaxX;
	float mMeshMinY;
	float mMeshMaxY;

	bool mEnableCrossLine;
	bool mEnableCurve;
	bool mEnableMesh;

	bool mDraw;
};