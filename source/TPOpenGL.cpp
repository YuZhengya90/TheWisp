#include <algorithm>
#include "..\resource.h"

#define _WIN32_WINNT _WIN32_WINNT_MAXVER

#include "afxdialogex.h"
#include "TheWispDlg.h"

#include "TPUI.h"
#include "TPFont.h"
#include "TPTexture.h"
#include "TPOpenGL.h"

TPOpenGL::TPOpenGL()
{
    if (!jar.Init("data-mining/models/"))
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
    ui.GetCoordinateByName("1");
    ui.GetCoordinateByName("2");
    ui.GetCoordinateByName("3");
    ui.GetCoordinateByName("4");
    ui.GetCoordinateByName("5");
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
	ON_WM_MOUSEHOVER()
	ON_WM_CONTEXTMENU()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_ILLUSION_ENABLEMESH, OnEnableMesh)
	ON_COMMAND(ID_ILLUSION_ENABLECROSSLINE, OnEnableCrossLine)
	ON_COMMAND(ID_ILLUSION_ENABLECURVE, OnEnableCurve)
	ON_COMMAND(ID_ILLUSION_ABOUT, OnAbout)
	
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
    // Do not call CWnd::OnPaint() for painting messages

    ui.Render();
    SwapBuffers(hdc);
    ::ValidateRect(m_hWnd, NULL);
}

void TPOpenGL::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == REDRAW_TIMER_ID)
    {
        ::InvalidateRect(m_hWnd, NULL, FALSE);
    }

    CWnd::OnTimer(nIDEvent);
}

BOOL TPOpenGL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
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
    if (ui.InIllusionSection(point.x, point.y))
    {
        ui.StartTranslate(point.x, point.y);
    }

    CWnd::OnLButtonDown(nFlags, point);
}


void TPOpenGL::OnMouseMove(UINT nFlags, CPoint point)
{
    if (ui.InIllusionSection(point.x, point.y))
    {
		bool translate = ui.Translate(point.x, point.y);
		if (!translate)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_HOVER;
			tme.dwHoverTime = 250;
			tme.hwndTrack = m_hWnd;
			::TrackMouseEvent(&tme);
		}
    }
    else
    {
        ui.StopTranslate();
    }

    CWnd::OnMouseMove(nFlags, point);
}


void TPOpenGL::OnLButtonUp(UINT nFlags, CPoint point)
{
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
    if (ui.InIllusionSection(point.x, point.y))
    {
        ui.StartScaleAnimation(point.x, point.y, false);
    }

    CWnd::OnRButtonDblClk(nFlags, point);
}

void TPOpenGL::OnMouseHover(UINT nFlags, CPoint point)
{
	if (ui.InIllusionSection(point.x, point.y))
	{
		ui.HoverPoint(point.x, point.y);
	}
}

void TPOpenGL::OnContextMenu(CWnd*, CPoint point)
{
	CPoint pCopy = point;
	::ScreenToClient(m_hWnd, &point);
	if (ui.InIllusionSection(point.x, point.y))
	{
		CMenu m_Menu;
		m_Menu.LoadMenu(IDR_MENU1);
		CMenu *m_SubMenu = m_Menu.GetSubMenu(0);
		m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pCopy.x, pCopy.y, this);
	}
}

void TPOpenGL::OnInitMenuPopup(CMenu *pMenuPopup, UINT nIndex, BOOL bSysMenu)
{
	if (!bSysMenu)
	{
		int nCount = pMenuPopup->GetMenuItemCount();
		for (int i = 0; i < nCount; ++i)
		{
			if (pMenuPopup->GetMenuItemID(i) == ID_ILLUSION_ENABLEMESH)
			{
				pMenuPopup->CheckMenuItem(ID_ILLUSION_ENABLEMESH, MF_BYCOMMAND | (ui.EnableMesh() ? MF_CHECKED : MF_UNCHECKED));
			}

			if (pMenuPopup->GetMenuItemID(i) == ID_ILLUSION_ENABLECURVE)
			{
				pMenuPopup->CheckMenuItem(ID_ILLUSION_ENABLECURVE, MF_BYCOMMAND | (ui.EnableCurve() ? MF_CHECKED : MF_UNCHECKED));
			}

			if (pMenuPopup->GetMenuItemID(i) == ID_ILLUSION_ENABLECROSSLINE)
			{
				pMenuPopup->CheckMenuItem(ID_ILLUSION_ENABLECROSSLINE, MF_BYCOMMAND | (ui.EnableCrossLine() ? MF_CHECKED : MF_UNCHECKED));
			}
		}
	}
}

void TPOpenGL::OnEnableMesh()
{
	if (ui.EnableMesh())
	{
		ui.SetEnableMesh(false);
	}
	else
	{
		ui.SetEnableMesh(true);
	}
}

void TPOpenGL::OnEnableCrossLine()
{
	if (ui.EnableCrossLine())
	{
		ui.SetEnableCrossLine(false);
	}
	else
	{
		ui.SetEnableCrossLine(true);
	}
}

void TPOpenGL::OnEnableCurve()
{
	if (ui.EnableCurve())
	{
		ui.SetEnableCurve(false);
	}
	else
	{
		ui.SetEnableCurve(true);
	}
}

void TPOpenGL::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void TPOpenGL::PredictModel1(CTime predictFrom, CTime predictTo)
{
    TPDate dateFrom(predictFrom.GetYear(), predictFrom.GetMonth(), predictFrom.GetDay());
    TPDate dateTo(predictTo.GetYear(), predictTo.GetMonth(), predictTo.GetDay());
#if 1

    vector<double> dbrsult = jar.PurchasePricePredictionPredictPrice(dateFrom, dateTo);
    vector<TPDate> dtX = TPDate::GetVector(dateFrom, dateTo);

	//vector<double> dbrsult = jar.SalePricePredictionPredictPrice(dateFrom, dateTo);
	//vector<TPDate> dtX = TPDate::GetVector(dateFrom, dateTo);

	//vector<double> dbrsult = jar.SaleQuantityPredictionPredictSaleQuantity(dateTo, 5.99f, 100);

	//vector<double> dbrsult = jar.ProfitPredictionPredictProfit(dateFrom, dateTo, 100, 2.59, 100, 5.99);
	//vector<double> dbrsult = jar.OperationAdviceAdvice(dateFrom,dateTo, 100);

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
	coordinate->DrawPoints(true);

    ui.setCurrentCoordByName("1");
#else

	TPCoordinate* coordinate = ui.GetCoordinateByName("1");
	if (coordinate == nullptr)
	{
		coordinate = ui.AddCoordinate("1");
	}

	vector<string> sdf;
	vector<double> sd;
	coordinate->SetTable("Profit Prediction", dateFrom, dateTo, 1, 1, sdf, sd);
#endif
}

void TPOpenGL::PredictModel2(CTime predictFrom, CTime predictTo)
{
    TPDate dateFrom(predictFrom.GetYear(), predictFrom.GetMonth(), predictFrom.GetDay());
    TPDate dateTo(predictTo.GetYear(), predictTo.GetMonth(), predictTo.GetDay());

    vector<double> dbrsult = jar.SalePricePredictionPredictPrice(dateFrom, dateTo);
    vector<TPDate> dtX = TPDate::GetVector(dateFrom, dateTo);

    TPCoordinate* coordinate = ui.GetCoordinateByName("2");
    if (coordinate == nullptr)
    {
        coordinate = ui.AddCoordinate("2");
    }

    coordinate->SetXAnchor(dateFrom - 1, dateTo + 1);
    auto pairMinMax = std::minmax_element(dbrsult.begin(), dbrsult.end());
    double duration = *pairMinMax.second - *pairMinMax.first;
    coordinate->SetYAnchor(*pairMinMax.first - 0.50 * duration, *pairMinMax.second + 0.50 * duration);
    coordinate->SetDrawingPoints<TPDate, double>(RP_POINT, 8, dtX, dbrsult);
    coordinate->DrawPoints(true);

    ui.setCurrentCoordByName("2");
}

void TPOpenGL::PredictModel3(CTime predictAt, double dStockQty, double dSalesPrice)
{

}

void TPOpenGL::PredictModel4(CTime predictWhen, CTime predictTo, double dStockQty, double dSalesPrice, double dPurQty, double dPurPrice)
{
    TPDate dateWhen(predictWhen.GetYear(), predictWhen.GetMonth(), predictWhen.GetDay());
    TPDate dateTo(predictTo.GetYear(), predictTo.GetMonth(), predictTo.GetDay());

    vector<double> dbrsult = jar.ProfitPredictionPredictProfit(dateWhen, dateTo, dStockQty, dSalesPrice, dPurQty, dPurPrice);
    vector<TPDate> dtX = TPDate::GetVector(dateWhen, dateTo);

    TPCoordinate* coordinate = ui.GetCoordinateByName("4");
    if (coordinate == nullptr)
    {
        coordinate = ui.AddCoordinate("4");
    }

    coordinate->SetXAnchor(dateWhen - 1, dateTo + 1);
    auto pairMinMax = std::minmax_element(dbrsult.begin(), dbrsult.end());
    double duration = *pairMinMax.second - *pairMinMax.first;
    coordinate->SetYAnchor(*pairMinMax.first - 0.50 * duration, *pairMinMax.second + 0.50 * duration);
    coordinate->SetDrawingPoints<TPDate, double>(RP_POINT, 8, dtX, dbrsult);
    coordinate->DrawPoints(true);

    ui.setCurrentCoordByName("4");
}

void TPOpenGL::PredictModel5(CTime predictWhen, CTime predictTo, double dStockQty)
{

}
