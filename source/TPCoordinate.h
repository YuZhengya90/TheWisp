#pragma once
#include <vector>
#include "TPConst.h"
#include "TPEnvironment.h"
#include "TPView.h"

using namespace std;

// y coordinate
typedef enum TPCoord_Y_T
{
	XY_FLOAT,
	XY_INT,	
};

// features
typedef enum TPCoord_F_T
{
	// 0000 0000 0000 0001
	F_MESH       = 0x0001,

	// 0000 0000 0000 0010
	F_CROSSLINE  = 0x0002,

	// 0000 0000 0000 0100
	F_POINT      = 0x0004,

	// 0000 0000 0000 1000
	F_CURVE      = 0x0008,

	// 0000 0000 0001 0000
	F_HASCHART   = 0x0010,

	// 0000 0000 0010 0000
	F_TABLE      = 0x0020
};

class TPCoordAnchorHelper
{
public:
	float mMinX;
	float mMaxX;
	float mMinY;
	float mMaxY;

	float mNegX;
	float mPosX;
	float mNegY;
	float mPosY;

public:
	void Push(float minX, float maxX, float minY, float maxY, TPView view){
		mMinX = minX; mMaxX = maxX; mMinY = minY; mMaxY = maxY;
		mNegX = view.GetNegX(); mPosX = view.GetPosX();
		mNegY = view.GetNegY(); mPosY = view.GetPosY();
	}

	void Pop(float &minX, float &maxX, float &minY, float &maxY, TPView& view) {
		minX = mMinX; maxX = mMaxX; minY = mMinY; maxY = mMaxY;
		view.SetAnchor(mNegX, mPosX, mNegY, mPosY);
	}
};

class TPCoordinate
{	
public:
	TPCoordinate(char * name);
	void Init();
	
	//           ------------------------------------------------------------
	//           |   Advice                                                 |                   
	//           | 20180101-20180201                                        |
	//           ============================================================
	//           |  SalePrice                                  5.99         |
	//           |  PurchaseQuantity                           1130         |
	//           |     Profit                                  5320         |
	//           ------------------------------------------------------------

	void SetValues(std::vector<TPDate> dates, std::vector<double> values);
	void SetValues(std::vector<TPDate> dates, std::vector<double> values, std::vector<string> titles, bool onlyTable);
	void SetEnableFeatures(TPCoord_F_T efType, bool bEnable);
	bool IsEnableFeatures(TPCoord_F_T efType);
	 int HoverPoint(TPPoint p);

unsigned RenderPoints();
    void RenderMesh();
	void RenderCrossLine();
    void RenderReferenceValue();
	void RenderTables();

private:  // for friend class TPUI

	friend class TPUI;

	float GetMinX()const { return mMinX; }
	float GetMaxX()const { return mMaxX; }
	float GetMinY()const { return mMinY; }
	float GetMaxY()const { return mMaxY; }

	TPView& GetView()    { return mView; }
	char* GetName()const { return mName; }

public:
	// X Anchor initialized with Date value.
	void SetXAnchor(TPDate minX, TPDate maxX);

	// X Anchor initialized with in value.
	void SetXAnchor(int minX, int maxX);

	// Y Anchor initialized with double value.
	void SetYAnchor(double minY, double maxY);

	// Y Anchor intialized with int value.
	void SetYAnchor(int minY, int maxY);

private:
	float GetSuitableXStep();
	float GetSuitableYStep();
	 void Exchange2TableAnchor();
	 void Exchange2IllusionAnchor();
	float GetCurveWidth();

	// identity
	char* mName;
	char* mXName;
	char* mYName;

	// Anchor info
	float mMinX;
	float mMaxX;
	float mMinY;
	float mMaxY;

	TPCoordAnchorHelper mIllusionAnchor;
	TPCoordAnchorHelper mTableAnchor;

	TPCoord_Y_T mYType;

	TPView mView;

	int mHoveredPoint;

	int mEnableFeatures;
	vector<TPDate> mTableFromto;
	vector<string> mTableTitles;
	vector<double> mTableValues;
};