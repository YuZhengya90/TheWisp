#include "TPEnvironment.h"
#include "TPUI.h"

#include <iostream>

using namespace std;

void TPMenuItem::SetTexture(GLuint textureId)
{
    mTextureId = textureId;
}

void TPMenuItem::SetClickFunc(MENUCLICKCB clickCallback)
{
    mCallback = clickCallback;
}

void TPMenuItem::SetChecked(bool checked)
{
    mChecked = checked;
}

void TPMenuItem::ToggleChecked()
{
    mChecked = !mChecked;
    if (mCallback)
    {
        mCallback(this);
    }
}

void TPUI::RenderIllusion()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(GetCurrentCoord().GetMinX(), GetCurrentCoord().GetMaxX(), GetCurrentCoord().GetMinY(), GetCurrentCoord().GetMaxY(), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(MENU_W + 3 * SCRN_B, SCRN_B, ILLU_W, ILLU_H);

    DisplayIllusionBorder();
}

void TPUI::DisplayIllusionBorder()
{
    glColor3f(0.3f, 0.4f, 0.4f);
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(GetCurrentCoord().GetMinX(), GetCurrentCoord().GetMinY(), 0);
    glVertex3f(GetCurrentCoord().GetMinX(), GetCurrentCoord().GetMaxY(), 0);
    glVertex3f(GetCurrentCoord().GetMaxX(), GetCurrentCoord().GetMaxY(), 0);
    glVertex3f(GetCurrentCoord().GetMaxX(), GetCurrentCoord().GetMinY(), 0);
    glVertex3f(GetCurrentCoord().GetMinX(), GetCurrentCoord().GetMinY(), 0);
    glEnd();
    glLineWidth(1.0);
}

void TPUI::DisplayMenu(unsigned count)
{
    GLfloat x = MENU_ITEM_W / 2;
    GLfloat y = MENU_ITEM_H / 2;
    GLfloat startPos = MENU_H / 2 - MENU_ITEM_G - MENU_ITEM_H / 2;
    GLfloat step = -(2 * y + MENU_ITEM_G);

    glColor3f(0.7f, 0.7f, 0.7f);
    glLoadIdentity();
    glTranslatef(0, startPos, 0);

    for (unsigned int i = 0; i < count; ++i)
    {
        int xMin = SCRN_B * 2;
        int xMax = xMin + (int)x * 2;
        int yMin = SCRN_B + MENU_ITEM_G + i * (MENU_ITEM_H + MENU_ITEM_G);
        int yMax = yMin + MENU_ITEM_H;

        bool onMenu = (mMousePassiveAction.X > xMin && mMousePassiveAction.X < xMax)
            && (mMousePassiveAction.Y > yMin && mMousePassiveAction.Y < yMax);

        if (onMenu)
        {
            glPushMatrix();
            glScalef(0.95, 0.95, 1);
        }

        glBindTexture(GL_TEXTURE_2D, mVecMenuItems[i]->mTextureId);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex3f(-x, y, 0);
        glTexCoord2f(0.0, 0.0); glVertex3f(-x,-y, 0);
        glTexCoord2f(1.0, 0.0); glVertex3f( x,-y, 0);
        glTexCoord2f(1.0, 1.0); glVertex3f( x, y, 0);
        glEnd();

        if (onMenu)
        {
            glPopMatrix();
        }

        glTranslatef(0, step, 0);
    }
}

TPCoordinate& TPUI::GetCurrentCoord()
{
    return *mVecCoordinate[mCurrCoordOrder];
}

void TPUI::DisplayMenuBorder()
{
    GLfloat x = MENU_W / 2;
    GLfloat y = MENU_H / 2;

    glColor3f(0.3, 0.4, 0.4);
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-x, y, 0);
    glVertex3f(-x, -y, 0);
    glVertex3f(x, -y, 0);
    glVertex3f(x, y, 0);
    glVertex3f(-x, y, 0);
    glEnd();
    glLineWidth(1.0);
}

void TPUI::RenderMenu()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-MENU_W / 2, MENU_W / 2, -MENU_H / 2, MENU_H / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(SCRN_B, SCRN_B, MENU_W, MENU_H);

    DisplayMenuBorder();
    DisplayMenu(mVecMenuItems.size());
}

TPUI::TPUI()
: mDoingAnimation(false)
{
}

TPUI::~TPUI()
{
    for (std::vector<TPMenuItem*>::iterator it = mVecMenuItems.begin(); it != mVecMenuItems.end(); ++it)
    {
        delete *it;
    }

    for (std::vector<TPCoordinate*>::iterator it = mVecCoordinate.begin(); it != mVecCoordinate.end(); ++it)
    {
        delete *it;
    }
}

void TPUI::Render()
{
    GetCurrentCoord().GetView().Render();
    GetCurrentCoord().RenderMesh();
    GetCurrentCoord().RenderPoints();

    RenderMenu();
    RenderIllusion();
    GetCurrentCoord().RenderReferenceValue();
}

void TPUI::MouseAction(int x, int y)
{
    if (mDoingAnimation)
    {
        return;
    }

    if (mTranslateStartAction.state == 1)
    {
        //cout << "XMOVE" << xMove << " " << "YMOVE" << yMove << "" << endl;

        // notice that screen (x,y) is not the view's (x,y) screen from left top but view from left bottom.
        // so mTranslateStaretAction.Y - y is right.

        float moveDeltaX = (GetView().GetPosX() - GetView().GetNegX()) / ILLU_W * (x - mTranslateStartAction.X);
        float moveDeltaY = (GetView().GetPosY() - GetView().GetNegY()) / ILLU_H * (mTranslateStartAction.Y - y);
        GetView().Translate(-moveDeltaX, -moveDeltaY);

        mTranslateStartAction.X = x;
        mTranslateStartAction.Y = y;
    }
}

void TPUI::MousePassiveAction(int x, int y)
{
    mMousePassiveAction.X = x;
    mMousePassiveAction.Y = y;
}

void TPUI::SetDrawingPoints(TPCoord_RP_T type, float size, TPPoint* pts, unsigned szPts)
{
    GetCurrentCoord().SetDrawingPoints(type, size, pts, szPts);
}

bool TPUI::InIllusionSection(int x, int y)
{
    float left = MENU_W + 3 * SCRN_B;
    if (x > left&& x < left + ILLU_W
        && y > SCRN_B && y < SCRN_B + ILLU_H)
    {
        return true;
    }

    return false;
}

void TPUI::StartTranslate(int x, int y)
{
    mTranslateStartAction.X = x;
    mTranslateStartAction.Y = y;
    mTranslateStartAction.state = 1;
}

void TPUI::StartScale(int x, int y, float rate)
{
    if (mDoingAnimation)
    {
        return;
    }

    float left = MENU_W + 3 * SCRN_B;

    float pX = (x - left) / (float)ILLU_W  * (GetView().GetPosX() - GetView().GetNegX()) + GetView().GetNegX();
    float pY = (ILLU_H + SCRN_B - y) / (float)ILLU_H * (GetView().GetPosY() - GetView().GetNegY()) + GetView().GetNegY();

    GetView().Scale(TPPoint(pX, pY), rate);
}

bool TPUI::IsDoubleClick(int x, int y)
{
    bool result = false;
    if (x == mDoubleClickAction.X && y == mDoubleClickAction.Y
        && GetTickCount() - mDoubleClickAction.state < 600)
    {
        cout << "IsDoubleClick!";
        result = true;
    }

    mDoubleClickAction.X = x;
    mDoubleClickAction.Y = y;
    mDoubleClickAction.state = GetTickCount();

    return result;
}

void TPUI::StartScaleAnimation(int x, int y, bool zoomOut)
{
    if (mDoingAnimation)
    {
        return;
    }

    float left = MENU_W + 3 * SCRN_B;

    float pX = (x - left) / (float)ILLU_W  * (GetView().GetPosX() - GetView().GetNegX()) + GetView().GetNegX();
    float pY = (ILLU_H + SCRN_B - y) / (float)ILLU_H * (GetView().GetPosY() - GetView().GetNegY()) + GetView().GetNegY();
    
    // 300 milliseconds in 20 frames (about 60frames per second)
    float animationFactor = sqrtf(sqrtf(MOUSE_SCALE_FACTOR));

    if (zoomOut)
    {
        GetView().ScaleAnimation(TPPoint(pX, pY), animationFactor, mDoubleClickAction.state, 300, 20);
    }
    else
    {
        GetView().ScaleAnimation(TPPoint(pX, pY), 1 / animationFactor, mDoubleClickAction.state, 300, 20);
    }
}

TPView& TPUI::GetView()
{
    return GetCurrentCoord().GetView();
}

TPMenuItem* TPUI::InMenuItem(int x, int y)
{
    for (unsigned int i = 0; i < mVecMenuItems.size(); ++i)
    {
        int xMin = SCRN_B * 2;
		int xMax = xMin + MENU_ITEM_W;
        int yMin = SCRN_B + MENU_ITEM_G + i * (MENU_ITEM_H + MENU_ITEM_G);
        int yMax = yMin + MENU_ITEM_H;

        bool onMenu = (x > xMin && x < xMax) && (y > yMin && y < yMax);

        if (onMenu)
        {
            return mVecMenuItems[i];
        }
    }

    return nullptr;    
}

TPMenuItem* TPUI::AddMenuItem(char* name)
{
    TPMenuItem* menuItem = new TPMenuItem(name);
    mVecMenuItems.push_back(menuItem);
    return menuItem;
}

TPMenuItem* TPUI::GetMenuItemByOrder(unsigned order)
{
    if (order < mVecMenuItems.size())
    {
        return mVecMenuItems[order];
    }

    return nullptr;
}

TPMenuItem* TPUI::GetMenuItemByName(char* name)
{
    for (unsigned i = 0; i < mVecMenuItems.size(); ++i)
    {
        if (mVecMenuItems[i]->mName == name)
        {
            return mVecMenuItems[i];
        }
    }

    return nullptr;
}

TPCoordinate* TPUI::AddCoordinate(char* name)
{
    TPCoordinate* coord = new TPCoordinate(mVecCoordinate.size(), name);
    mVecCoordinate.push_back(coord);
    mCurrCoordOrder = mVecCoordinate.size() - 1;
    return coord;
}

TPCoordinate* TPUI::GetCoordinateByOrder(unsigned order)
{
    if (order < mVecCoordinate.size())
    {
        return mVecCoordinate[order];
    }

    return nullptr;
}

TPCoordinate* TPUI::GetCoordinateByName(char* name)
{
    for (std::vector<TPCoordinate*>::iterator it = mVecCoordinate.begin(); it != mVecCoordinate.end(); ++it)
    {
        if ((*it)->GetName() == name)
        {
            return *it;
        }
    }

    return nullptr;
}

void TPUI::setCurrentCoordByOrder(unsigned order)
{
    if (order < mVecCoordinate.size())
    {
        mCurrCoordOrder = order;
    }
}

void TPUI::setCurrentCoordByName(char* name)
{
    for (unsigned i = 0; i < mVecCoordinate.size(); ++i)
    {
        if (mVecCoordinate[i]->GetName() == name)
        {
            mCurrCoordOrder = i;
        }
    }
}