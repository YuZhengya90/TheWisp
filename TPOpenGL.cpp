#include "stdafx.h"
#include <algorithm>
#include "TPOpenGL.h"

#include "TPUI.h"

#include "TPFont.h"
#include "TPTexture.h"


TPOpenGL::TPOpenGL()
{
    if (!jar.Init())
    {
        MessageBoxA(m_hWnd, "", "JAR init failed", MB_OK);
    }
}

TPOpenGL::~TPOpenGL()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ::ReleaseDC(m_hWnd, hdc);
}


void TPOpenGL::SetupPixelFormat(HDC hDC)
{
    int nPixelFormat;
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}


void TPOpenGL::TPInitGL()
{
    TPLoadFont();
    TPLoadTexture();

    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void TPOpenGL::TPInitUI(HWND hwnd)
{
}

BEGIN_MESSAGE_MAP(TPOpenGL, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


void TPOpenGL::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    hdc = ::GetDC(m_hWnd);
    SetupPixelFormat(hdc);
    hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    ::SetTimer(m_hWnd, REDRAW_TIMER_ID, 10, NULL);

    TPInitGL();
    TPInitUI(m_hWnd);



    CWnd::PreSubclassWindow();
}

int TPOpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here

    return 0;
}

void TPOpenGL::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CWnd::OnPaint() for painting messages

    ui.Render();
    SwapBuffers(hdc);
    ::ValidateRect(m_hWnd, NULL);
}

void TPOpenGL::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == REDRAW_TIMER_ID)
    {
        ::InvalidateRect(m_hWnd, NULL, FALSE);
    }

    CWnd::OnTimer(nIDEvent);
}

BOOL TPOpenGL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    ::ScreenToClient(m_hWnd, &pt);

    if (zDelta > 0)
    {
        if (ui.InIllusionSection(pt.x, pt.y))
        {
            ui.StartScale(pt.x, pt.y, MOUSE_SCALE_FACTOR);
        }
    }
    else if (zDelta < 0)
    {
        if (ui.InIllusionSection(pt.x, pt.y))
        {
            ui.StartScale(pt.x, pt.y, 1 / MOUSE_SCALE_FACTOR);
        }
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void TPOpenGL::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (ui.InIllusionSection(point.x, point.y))
    {
        ui.StartTranslate(point.x, point.y);
    }

    CWnd::OnLButtonDown(nFlags, point);
}


void TPOpenGL::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (ui.InIllusionSection(point.x, point.y))
    {
        ui.Translate(point.x, point.y);
    }
    else
    {
        ui.StopTranslate();
    }

    CWnd::OnMouseMove(nFlags, point);
}


void TPOpenGL::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (ui.InTranslating())
    {
        ui.StopTranslate();
    }

    CWnd::OnLButtonUp(nFlags, point);
}


void TPOpenGL::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (ui.InIllusionSection(point.x, point.y))
    {
        ui.StartScaleAnimation(point.x, point.y, true);
    }

    CWnd::OnLButtonDblClk(nFlags, point);
}


void TPOpenGL::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (ui.InIllusionSection(point.x, point.y))
    {
        ui.StartScaleAnimation(point.x, point.y, false);
    }

    CWnd::OnRButtonDblClk(nFlags, point);
}


void TPOpenGL::PredictModel1(CTime predictFrom, CTime predictTo)
{
    TPDate dateFrom(predictFrom.GetYear(), predictFrom.GetMonth(), predictFrom.GetDay());
    TPDate dateTo(predictTo.GetYear(), predictTo.GetMonth(), predictTo.GetDay());

    jar.PurchasePricePredictionSetModelPath("D:/GitHub/DataMining/models/");
    vector<double> dbrsult = jar.PurchasePricePredictionPredictPrice(dateFrom, dateTo);
    vector<TPDate> dtX = TPDate::GetVector(dateFrom, dateTo);

    TPCoordinate* coordinate = ui.GetCoordinateByName("1");
    if (coordinate == nullptr)
    {
        coordinate = ui.AddCoordinate("1");
    }

    coordinate->SetXAnchor(dateFrom - 1, dateTo + 1);
    auto pairMinMax = std::minmax_element(dbrsult.begin(), dbrsult.end());
    double duration = *pairMinMax.second - *pairMinMax.first;
    coordinate->SetYAnchor(*pairMinMax.first - 0.50 * duration, *pairMinMax.second + 0.50 * duration);
    coordinate->SetDrawingPoints<TPDate, double>(RP_POINT, 8, dtX, dbrsult);

    ui.setCurrentCoordByName("1");
}

void TPOpenGL::PredictModel2(CTime predictFrom, CTime predictTo)
{
    TPDate dateFrom(predictFrom.GetYear(), predictFrom.GetMonth(), predictFrom.GetDay());
    TPDate dateTo(predictTo.GetYear(), predictTo.GetMonth(), predictTo.GetDay());

    vector<double> dbrsult = jar.PurchasePricePredictionPredictPrice(dateFrom, dateTo);
    TPPoint* pts = (TPPoint*)malloc(dbrsult.size() * sizeof(TPPoint));
    for (unsigned i = 0; i < dbrsult.size(); ++i)
    {
        pts[i].x = dateFrom.ToInt() + i;
        pts[i].y = (float)dbrsult[i];
    }

    TPCoordinate* coordinate = ui.GetCoordinateByName("2");
    if (coordinate == nullptr)
    {
        coordinate = ui.AddCoordinate("2");
    }

    coordinate->SetXAnchor(dateFrom - 1, dateTo + 1);
    coordinate->SetYAnchor(1.0, 4.0);
    coordinate->SetDrawingPoints(RP_CURVE, 3, pts, dbrsult.size());
    free(pts);

    ui.setCurrentCoordByName("2");
}
