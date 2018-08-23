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
    void RenderMenu();
    void DisplayMenuBorder();

    TPCoordinate& GetCurrentCoord();

public:
    TPUI();
    ~TPUI();

    void Render();
    void Translate(int x, int y);
    void SetDrawingPoints(TPCoord_RP_T type, float size, TPPoint* pts, unsigned szPts);

    bool InIllusionSection(int x, int y);
    void StartTranslate(int x, int y);
    void StopTranslate(){ mTranslateStartAction.state = 0; }
    bool InTranslating(){ return mTranslateStartAction.state == 1; }

    void StartScale(int x, int y, float rate);
    void StartScaleAnimation(int x, int y, bool zoomOut);

    TPView& GetView();

    TPCoordinate* AddCoordinate(char* name);
    TPCoordinate* GetCoordinateByOrder(unsigned order);
    TPCoordinate* GetCoordinateByName(char* name);
    void setCurrentCoordByOrder(unsigned order);
    void setCurrentCoordByName(char* name);

private:

    unsigned mCurrCoordOrder;
    std::vector<TPCoordinate*>mVecCoordinate;
    
    TPMouse mTranslateStartAction;
    bool mDoingAnimation;
};