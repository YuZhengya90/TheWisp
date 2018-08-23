#include "TPEnvironment.h"
#include "TPUI.h"

using namespace std;

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
	const float limitDeltaX = 0.0001 * (GetCurrentCoord().GetMaxX() - GetCurrentCoord().GetMinX());
	const float limitDeltaY = 0.0001 * (GetCurrentCoord().GetMaxY() - GetCurrentCoord().GetMinY());
	
    glColor3f(0.3f, 0.4f, 0.4f);
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(GetCurrentCoord().GetMinX() + limitDeltaX, GetCurrentCoord().GetMinY() + limitDeltaY, 0);
	glVertex3f(GetCurrentCoord().GetMinX() + limitDeltaX, GetCurrentCoord().GetMaxY() - limitDeltaY, 0);
	glVertex3f(GetCurrentCoord().GetMaxX() - limitDeltaX, GetCurrentCoord().GetMaxY() - limitDeltaY, 0);
    glVertex3f(GetCurrentCoord().GetMaxX() - limitDeltaX, GetCurrentCoord().GetMinY() + limitDeltaY, 0);
    glVertex3f(GetCurrentCoord().GetMinX() + limitDeltaX, GetCurrentCoord().GetMinY() + limitDeltaY, 0);
    glEnd();
    glLineWidth(1.0);
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
}

TPUI::TPUI()
: mDoingAnimation(false)
{
}

TPUI::~TPUI()
{
    for (std::vector<TPCoordinate*>::iterator it = mVecCoordinate.begin(); it != mVecCoordinate.end(); ++it)
    {
        delete *it;
    }
}

void TPUI::Render()
{
	glClearColor(0.96, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

    GetCurrentCoord().GetView().Render();
    GetCurrentCoord().RenderMesh();
    GetCurrentCoord().RenderPoints();

    RenderMenu();
    RenderIllusion();
    GetCurrentCoord().RenderReferenceValue();
}

void TPUI::Translate(int x, int y)
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
        GetView().ScaleAnimation(TPPoint(pX, pY), animationFactor, GetTickCount(), 300, 20);
    }
    else
    {
        GetView().ScaleAnimation(TPPoint(pX, pY), 1 / animationFactor, GetTickCount(), 300, 20);
    }
}

TPView& TPUI::GetView()
{
    return GetCurrentCoord().GetView();
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