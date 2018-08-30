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
    glViewport(SCRN_B, SCRN_B, ILLU_W, ILLU_H);

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

TPPoint TPUI::ClientToIllusion(const TPPoint& p)
{
	float pX = (p.x - SCRN_B) / (float)ILLU_W  * (GetView().GetPosX() - GetView().GetNegX()) + GetView().GetNegX();
	float pY = (ILLU_H + SCRN_B - p.y) / (float)ILLU_H * (GetView().GetPosY() - GetView().GetNegY()) + GetView().GetNegY();

	return TPPoint(pX, pY);
}

TPUI::TPUI()
    : mDoingAnimation(false), mCurrCoordOrder(-1), mZoomOut(true)
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
	if (mVecCoordinate.size() == 0)
	{
		return;
	}

    if (EnableTable())
    {
        glClearColor(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        glClearColor(0.96, 1.0, 1.0, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT);


    GetCurrentCoord().GetView().Render();
	if (EnableMesh())
	{
		GetCurrentCoord().RenderMesh();
	}
    
	if (EnableCrossLine())
	{
		GetCurrentCoord().RenderCrossLine();
	}

	if (EnableTable())
	{
		GetCurrentCoord().RenderTables();
	}
	else
	{
		unsigned renderPoint = GetCurrentCoord().RenderPoints();
		if (renderPoint > 0)
		{
			RenderIllusion();
			GetCurrentCoord().RenderReferenceValue();
		}
	}	
}

bool TPUI::InIllusionSection(int x, int y)
{
	if (x > SCRN_B && x < SCRN_B + ILLU_W
        && y > SCRN_B && y < SCRN_B + ILLU_H)
    {
        return !NoCurrentCoord();
    }

    return false;
}

void TPUI::StartTranslate(int x, int y)
{
    mTranslateStartAction.X = x;
    mTranslateStartAction.Y = y;
    mTranslateStartAction.state = 1;
}

bool TPUI::Translate(int x, int y)
{
	if (mDoingAnimation || NoCurrentCoord())
	{
		return false;
	}

	if (mTranslateStartAction.state == 1)
	{
		//cout << "XMOVE" << xMove << " " << "YMOVE" << yMove << "" << endl;

		// notice that screen (x,y) is not the view's (x,y) screen from left top but view from left bottom.
		// so mTranslateStaretAction.Y - y is right.

		float moveDeltaX = 0.0f;
		float moveDeltaY = (GetView().GetPosY() - GetView().GetNegY()) / ILLU_H * (mTranslateStartAction.Y - y);
		if (EnableIllusion())
		{
			moveDeltaX = (GetView().GetPosX() - GetView().GetNegX()) / ILLU_W * (x - mTranslateStartAction.X);
		}
		
		GetView().Translate(-moveDeltaX, -moveDeltaY);

		mTranslateStartAction.X = x;
		mTranslateStartAction.Y = y;

		return true;
	}

	return false;
}

void TPUI::ScrollTranslate(bool scrollDown)
{
	if (mDoingAnimation || NoCurrentCoord() || mTranslateStartAction.state == 1)
	{
		return;
	}

	if (scrollDown)
	{
		GetView().Translate(0, 40);
	}
	else
	{
		GetView().Translate(0,-40);
	}
}

void TPUI::StartScale(int x, int y, float rate)
{
	if (mDoingAnimation || NoCurrentCoord())
    {
        return;
    }
	
	GetView().Scale(ClientToIllusion(TPPoint(x, y)), rate);
}

void TPUI::StartScaleAnimation(int x, int y)
{
	if (mDoingAnimation || NoCurrentCoord())
    {
        return;
    }
   
    // 300 milliseconds in 20 frames (about 60frames per second)
    float animationFactor = sqrtf(MOUSE_SCALE_FACTOR);

    if (mZoomOut)
    {
        GetView().ScaleAnimation(ClientToIllusion(TPPoint(x, y)), animationFactor, GetTickCount(), 400, 10);
        mZoomOut = false;
    }
    else
    {
		GetView().ScaleAnimation(ClientToIllusion(TPPoint(x, y)), 1 / animationFactor, GetTickCount(), 400, 10);
        mZoomOut = true;
    }
}

void TPUI::HoverPoint(int x, int y)
{
	if (NoCurrentCoord())
	{
		return;
	}
	
	TPPoint p = ClientToIllusion(TPPoint(x, y));
	GetCurrentCoord().HoverPoint(ClientToIllusion(TPPoint(x, y)));
}

void TPUI::DisableHoverStatus()
{
    if (NoCurrentCoord())
    {
        return;
    }

    GetCurrentCoord().DisableHoverStatus();
}

TPView& TPUI::GetView()
{
    return GetCurrentCoord().GetView();
}

TPCoordinate* TPUI::AddCoordinate(char* name)
{
    TPCoordinate* coord = new TPCoordinate(name);
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