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
    void SetDrawingPoints(TPCoord_RP_T type, float size, TPPoint* pts, unsigned szPts);

    bool InIllusionSection(int x, int y);
    void StartTranslate(int x, int y);
	bool Translate(int x, int y);
    void StopTranslate(){ mTranslateStartAction.state = 0; }
    bool InTranslating(){ return mTranslateStartAction.state == 1; }

    void StartScale(int x, int y, float rate);
    void StartScaleAnimation(int x, int y, bool zoomOut);

	void HoverPoint(int x, int y);

	bool EnableMesh() { return GetCurrentCoord().GetEnableMesh(); }
	bool EnableCrossLine() { return GetCurrentCoord().GetEnableCrossLine(); }
	bool EnableCurve() { return GetCurrentCoord().GetEnableCurve(); }
	void SetEnableMesh(bool bEnable) { GetCurrentCoord().SetEnableMesh(bEnable); }
	void SetEnableCrossLine(bool bEnable) { GetCurrentCoord().SetEnableCrossLine(bEnable); }
	void SetEnableCurve(bool bEnable) { GetCurrentCoord().SetEnableCurve(bEnable); }

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