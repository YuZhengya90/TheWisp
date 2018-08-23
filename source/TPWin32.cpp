#include <algorithm>
#include "TPUI.h"
#include "TPMenu.h"

#include "TPFont.h"
#include "TPTexture.h"
#include "TPJar.h"
#include "TPWin32.h"

TPUI ui;

void TPInitGL()
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

void TPInitUI(HWND hwnd)
{
	TPJar jar;
	if (!jar.Init())
	{
		MessageBoxA(hwnd, "", "JAR init failed", MB_OK);
		return;
	}

	// TEST1

	TPDate dateFrom1(20170401), dateTo1(20170701);
	jar.PurchasePricePredictionSetModelPath("D:/GitHub/DataMining/models/");
	vector<double> dbrsult = jar.PurchasePricePredictionPredictPrice(dateFrom1, dateTo1);
	vector<TPDate> dtX = TPDate::GetVector(dateFrom1, dateTo1);
	TPCoordinate* coord1 = ui.AddCoordinate("1");
	coord1->SetXAnchor(dateFrom1 - 1, dateTo1 + 1);
	auto pairMinMax = std::minmax_element(dbrsult.begin(), dbrsult.end());
	double duration = *pairMinMax.second - *pairMinMax.first;
	coord1->SetYAnchor(*pairMinMax.first -  0.50 * duration, *pairMinMax.second + 0.50 * duration);
	coord1->SetDrawingPoints<TPDate, double>(RP_POINT, 8, dtX, dbrsult);

	// TEST2

	TPDate dateFrom2(20160101), dateTo2(20160401);
	vector<double> dbrsult2 = jar.PurchasePricePredictionPredictPrice(dateFrom2, dateTo2);
	TPPoint* pts = (TPPoint*)malloc(dbrsult2.size() * sizeof(TPPoint));
	for (unsigned i = 0; i < dbrsult2.size(); ++i)
	{
		pts[i].x = dateFrom2.ToInt() + i;
		pts[i].y = (float)dbrsult2[i];
	}

	TPCoordinate* coord2 = ui.AddCoordinate("2");
	coord2->SetXAnchor(dateFrom2 - 1, dateTo2 + 1);
	coord2->SetYAnchor(1.0, 4.0);
	coord2->SetDrawingPoints(RP_CURVE, 3, pts, dbrsult.size());
	free(pts);

	ui.setCurrentCoordByName("1");
}

static void SetupPixelFormat(HDC hDC)
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

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;

	switch (message) {
	case WM_CREATE:
	{
		hDC = GetDC(hwnd);
		SetupPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		SetTimer(hwnd, REDRAW_TIMER_ID, 10, NULL);

		TPInitGL();
		TPInitUI(hwnd);
		TPInitMenu((HINSTANCE)lParam, hwnd);
		return 0;
	}
	break;
	case WM_DESTROY:
	{
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
	}
	break;
	case WM_PAINT:
	{
		ui.Render();
		SwapBuffers(hDC);
		ValidateRect(hwnd, NULL);
	}
	break;
	case WM_TIMER:
	{
		if (wParam == REDRAW_TIMER_ID)
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		POINT p = {LOWORD(lParam), HIWORD(lParam)};
		ScreenToClient(hwnd, &p);

		short fwkey = LOWORD(wParam);
		short delta = HIWORD(wParam);
		
		if (delta > 0)
		{
			if (ui.InIllusionSection(p.x, p.y))
			{
				ui.StartScale(p.x, p.y, MOUSE_SCALE_FACTOR);
			}
		}
		else if (delta < 0)
		{
			if (ui.InIllusionSection(p.x, p.y))
			{
				ui.StartScale(p.x, p.y, 1 / MOUSE_SCALE_FACTOR);
			}
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (ui.InIllusionSection(x, y))
		{
			ui.StartTranslate(x, y);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (ui.InIllusionSection(x, y))
		{
			ui.Translate(x, y);
		}
		else
		{
			ui.StopTranslate();
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (ui.InTranslating())
		{
			ui.StopTranslate();
		}
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (ui.InIllusionSection(x, y))
		{
			ui.StartScaleAnimation(x, y, true);
		}
	}
	break;
	case WM_RBUTTONDBLCLK:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (ui.InIllusionSection(x, y))
		{
			ui.StartScaleAnimation(x, y, false);
		}
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case PUR_PREDICT_BTN:
			ui.setCurrentCoordByOrder(0);
			break;
		case SAL_PREDICT_BTN:
			ui.setCurrentCoordByOrder(1);
			break;
		}
	}
	break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
	char strClsNM[] = WINDOW_TITLE;
	char strTitle[] = WINDOW_TITLE;

	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = strClsNM;

	if (!RegisterClass(&wndclass))
	{
		return nullptr;
	}

	HWND hwnd;
	hwnd = CreateWindow(
		strClsNM,
		strTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	RECT rec1;
	GetWindowRect(hwnd, &rec1);
	RECT rec2;
	GetClientRect(hwnd, &rec2);
	int newWidth = SCRN_W + (rec1.right - rec1.left) - (rec2.right - rec2.left);
	int newHeight = SCRN_H + (rec1.bottom - rec1.top) - (rec2.bottom - rec2.top);
	MoveWindow(
		hwnd, 
		GetSystemMetrics(SM_CXSCREEN) / 2 - SCRN_W / 2, 
		GetSystemMetrics(SM_CYSCREEN) / 2 - SCRN_H / 2, 
		newWidth, 
		newHeight, 
		FALSE);

	return hwnd;
}