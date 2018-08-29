#pragma once

#include <vector>
#include "TPConst.h"
#include "TPCoordinate.h"

struct TPMouse
{
    GLint X;
    GLint Y;
    DWORD state = 0;
    TPMouse() :X(0),Y(0), state(0){}
};

class TPUI
{
private:
    void RenderIllusion();
    void DisplayIllusionBorder();

	bool NoCurrentCoord(){ return mCurrCoordOrder < 0; }
    TPCoordinate& GetCurrentCoord();
	TPPoint ClientToIllusion(const TPPoint& p);

public:
    TPUI();
    ~TPUI();

    void Render();

    bool InIllusionSection(int x, int y);

    void StartTranslate(int x, int y);
	bool Translate(int x, int y);
    void StopTranslate(){ mTranslateStartAction.state = 0; }
    bool InTranslating(){ return mTranslateStartAction.state == 1; }
	void ScrollTranslate(bool scrollDown);

    void StartScale(int x, int y, float rate);
    void StartScaleAnimation(int x, int y, bool zoomOut);

	void HoverPoint(int x, int y);

	bool EnableIllusion() { return !EnableTable(); }
	bool EnableMesh() { return !EnableTable() && GetCurrentCoord().IsEnableFeatures(F_MESH); }
	bool EnableCrossLine() { return !EnableTable() && GetCurrentCoord().IsEnableFeatures(F_CROSSLINE); }
	bool EnableCurve() { return !EnableTable() && GetCurrentCoord().IsEnableFeatures(F_CURVE); }
	bool EnablePoint() { return !EnableTable() && GetCurrentCoord().IsEnableFeatures(F_POINT); }
	bool EnableTable() { return GetCurrentCoord().IsEnableFeatures(F_TABLE); }
	bool HasChart() { return GetCurrentCoord().IsEnableFeatures(F_HASCHART); }
	
	void SetEnableIllusion(bool bEnable) { GetCurrentCoord().SetEnableFeatures(F_TABLE, !bEnable); }
	void SetEnableMesh(bool bEnable) { GetCurrentCoord().SetEnableFeatures(F_MESH, bEnable); }
	void SetEnableCrossLine(bool bEnable) { GetCurrentCoord().SetEnableFeatures(F_CROSSLINE, bEnable); }
	void SetEnableCurve(bool bEnable) { GetCurrentCoord().SetEnableFeatures(F_CURVE, bEnable); }
	void SetEnablePoint(bool bEnable) { GetCurrentCoord().SetEnableFeatures(F_POINT, bEnable); }
	void SetEnableTable(bool bEnable) { GetCurrentCoord().SetEnableFeatures(F_TABLE, bEnable); }
	

    TPView& GetView();

    TPCoordinate* AddCoordinate(char* name);
    TPCoordinate* GetCoordinateByOrder(unsigned order);
    TPCoordinate* GetCoordinateByName(char* name);
    void setCurrentCoordByOrder(unsigned order);
    void setCurrentCoordByName(char* name);

private:

    int mCurrCoordOrder;
    std::vector<TPCoordinate*>mVecCoordinate;
    
    TPMouse mTranslateStartAction;
    bool mDoingAnimation;
};