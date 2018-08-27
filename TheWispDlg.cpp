
// TheWispDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TheWisp.h"
#include "TheWispDlg.h"
#include "afxdialogex.h"

#include <algorithm>
#include "source\TPUI.h"

#include "source\TPFont.h"
#include "source\TPTexture.h"
#include "source\TPJar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTheWispDlg dialog



CTheWispDlg::CTheWispDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTheWispDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTheWispDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PIC_CHART, m_ctrlChart);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ctrlPredictFrom);
    DDX_Control(pDX, IDC_DATETIMEPICKER2, m_ctrlPredictTo);
}

BEGIN_MESSAGE_MAP(CTheWispDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CTheWispDlg::OnDtnDatetimechangeDatetimepicker1)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BTN_PREDICT, &CTheWispDlg::OnBnClickedBtnPredict)
END_MESSAGE_MAP()


// CTheWispDlg message handlers

BOOL CTheWispDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTheWispDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTheWispDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTheWispDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTheWispDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}


int CTheWispDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}


void CTheWispDlg::OnBnClickedBtnPredict()
{
    //CTime predictFrom, predictTo;

    //m_ctrlPredictFrom.GetTime(predictFrom);
    //m_ctrlPredictTo.GetTime(predictTo);

    //TPDate dateFrom(predictFrom.GetYear(), predictFrom.GetMonth(), predictFrom.GetDay());
    //TPDate dateTo(predictTo.GetYear(), predictTo.GetMonth(), predictTo.GetDay());

    //jar.PurchasePricePredictionSetModelPath("D:/GitHub/DataMining/models/");
    //vector<double> dbrsult = jar.PurchasePricePredictionPredictPrice(dateFrom, dateTo);
    //vector<TPDate> dtX = TPDate::GetVector(dateFrom, dateTo);
    //TPCoordinate* coord1 = ui.AddCoordinate("1");
    //coord1->SetXAnchor(dateFrom - 1, dateTo + 1);
    //auto pairMinMax = std::minmax_element(dbrsult.begin(), dbrsult.end());
    //double duration = *pairMinMax.second - *pairMinMax.first;
    //coord1->SetYAnchor(*pairMinMax.first - 0.50 * duration, *pairMinMax.second + 0.50 * duration);
    //coord1->SetDrawingPoints<TPDate, double>(RP_POINT, 8, dtX, dbrsult);
}