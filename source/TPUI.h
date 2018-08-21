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

typedef void(*MENUCLICKCB)(void* p);

class TPMenuItem
{
    friend class TPUI;
public:
    TPMenuItem(char* name = nullptr)
        : mName(name), mChecked(false)
    {}

    void SetTexture(GLuint textureId);
    void SetClickFunc(MENUCLICKCB clickCallback);
    void SetChecked(bool checked);
    void ToggleChecked();

    char* GetName() { return mName; }

private:

    char* mName;
    bool mChecked;
    GLuint mTextureId;
    MENUCLICKCB mCallback;
};

class TPUI
{    
private:
    void RenderIllusion();
    void DisplayIllusionBorder();
    void RenderMenu();
    void DisplayMenuBorder();
    void DisplayMenu(unsigned count);

    TPCoordinate& GetCurrentCoord();

public:
    TPUI();
    ~TPUI();
    void Render();
    void MouseAction(int x, int y);
    void MousePassiveAction(int x, int y);
    void SetDrawingPoints(TPCoord_RP_T type, float size, TPPoint* pts, unsigned szPts);

    bool InIllusionSection(int x, int y);
    void StartTranslate(int x, int y);
    void StopTranslate(){ mTranslateStartAction.state = 0; }
    bool InTranslating(){ return mTranslateStartAction.state == 1; }

    void StartScale(int x, int y, float rate);

    bool IsDoubleClick(int x, int y);
    void StartScaleAnimation(int x, int y, bool zoomOut);

    TPView& GetView();

    TPMenuItem* InMenuItem(int x, int y);

    TPMenuItem* AddMenuItem(char* name);
    TPMenuItem* GetMenuItemByOrder(unsigned order);
    TPMenuItem* GetMenuItemByName(char* name);

    TPCoordinate* AddCoordinate(char* name);
    TPCoordinate* GetCoordinateByOrder(unsigned order);
    TPCoordinate* GetCoordinateByName(char* name);
    void setCurrentCoordByOrder(unsigned order);
    void setCurrentCoordByName(char* name);

private:

    unsigned mCurrCoordOrder;
    std::vector<TPCoordinate*>mVecCoordinate;
    std::vector<TPMenuItem*> mVecMenuItems;
    
    TPMouse mMousePassiveAction;
    TPMouse mTranslateStartAction;
    TPMouse mDoubleClickAction;

    bool mDoingAnimation;
};