
// TheWispDlg.cpp : implementation file
//

#include "..\stdafx.h"
#include "..\TheWisp.h"
#include "TheWispDlg.h"
#include "afxdialogex.h"

#include <algorithm>
#include "TPUI.h"

#include "TPFont.h"
#include "TPTexture.h"
#include "TPJar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
    , m_iRadioBtnGroup0(0)
    , m_dStockQtyPredict4(0)
    , m_dSalesPricePredict4(0)
    , m_dStockQtyPredict3(0)
    , m_dSalesPricePredict3(0)
    , m_dPurQtyPredict4(0)
    , m_dPurPricePredict4(0)
    , m_dStockQtyPredict5(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTheWispDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PIC_CHART, m_ctrlChart);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateFromPredict1);
    DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dateToPredict1);
    DDX_Radio(pDX, IDC_RADIO1, m_iRadioBtnGroup0);
    DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dateFromPredict2);
    DDX_Control(pDX, IDC_DATETIMEPICKER4, m_dateToPredict2);
    DDX_Control(pDX, IDC_DATETIMEPICKER5, m_dateAtPredict3);
    DDX_Control(pDX, IDC_EDIT1, m_editStockQtyPredict3);
    DDX_Control(pDX, IDC_EDIT2, m_editSalesPricePredict3);
    DDX_Control(pDX, IDC_DATETIMEPICKER6, m_dateWhenPredict4);
    DDX_Control(pDX, IDC_DATETIMEPICKER7, m_dateToPredict4);
    DDX_Text(pDX, IDC_EDIT3, m_dStockQtyPredict4);
    DDX_Text(pDX, IDC_EDIT6, m_dSalesPricePredict4);
    DDX_Text(pDX, IDC_EDIT1, m_dStockQtyPredict3);
    DDX_Text(pDX, IDC_EDIT2, m_dSalesPricePredict3);
    DDX_Text(pDX, IDC_EDIT5, m_dPurQtyPredict4);
    DDX_Text(pDX, IDC_EDIT4, m_dPurPricePredict4);
    DDX_Control(pDX, IDC_DATETIMEPICKER8, m_dateWhenPredict5);
    DDX_Control(pDX, IDC_DATETIMEPICKER9, m_dateToPredict5);
    DDX_Text(pDX, IDC_EDIT7, m_dStockQtyPredict5);
}

BEGIN_MESSAGE_MAP(CTheWispDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CTheWispDlg::OnDtnDatetimechangeDatetimepicker1)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BTN_PREDICT, &CTheWispDlg::OnBnClickedBtnPredict)
    ON_BN_CLICKED(IDC_RADIO1, &CTheWispDlg::OnRadioBtnGroup0Clicked)
    ON_BN_CLICKED(IDC_RADIO2, &CTheWispDlg::OnRadioBtnGroup0Clicked)
    ON_BN_CLICKED(IDC_RADIO3, &CTheWispDlg::OnRadioBtnGroup0Clicked)
    ON_BN_CLICKED(IDC_RADIO4, &CTheWispDlg::OnRadioBtnGroup0Clicked)
    ON_BN_CLICKED(IDC_RADIO5, &CTheWispDlg::OnRadioBtnGroup0Clicked)
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
    UpdateData(TRUE);
    switch (m_iRadioBtnGroup0)
    {
        case 0:
        {
            CTime predictFrom, predictTo;

            m_dateFromPredict1.GetTime(predictFrom);
            m_dateToPredict1.GetTime(predictTo);

            m_ctrlChart.PredictModel1(predictFrom, predictTo);
        }
        break;
        case 1:
        {
            CTime predictFrom, predictTo;

            m_dateFromPredict2.GetTime(predictFrom);
            m_dateToPredict2.GetTime(predictTo);

            m_ctrlChart.PredictModel2(predictFrom, predictTo);
        }
        break;
        case 2:
        {
            CTime predictAt;
            m_dateAtPredict3.GetTime(predictAt);

            m_ctrlChart.PredictModel3(predictAt, m_dStockQtyPredict3, m_dSalesPricePredict3);
        }
        break;
        case 3:
        {
            CTime predictWhen, predictTo;

            m_dateWhenPredict4.GetTime(predictWhen);
            m_dateToPredict4.GetTime(predictTo);

            m_ctrlChart.PredictModel4(predictWhen, predictTo, m_dStockQtyPredict4, m_dSalesPricePredict4, m_dPurQtyPredict4, m_dPurPricePredict4);
        }
        break;
        case 4:
        {
            CTime predictWhen, predictTo;
            m_dateWhenPredict5.GetTime(predictWhen);
            m_dateToPredict5.GetTime(predictTo);

            m_ctrlChart.PredictModel5(predictWhen, predictTo, m_dStockQtyPredict5);
        }
        break;
    }
}


void CTheWispDlg::OnRadioBtnGroup0Clicked()
{
    UpdateData(TRUE);
    switch (m_iRadioBtnGroup0)
    {
        case 0:
        {
            m_ctrlChart.ui.setCurrentCoordByName("1");
        }
        break;
        case 1:
        {
            m_ctrlChart.ui.setCurrentCoordByName("2");
        }
        break;
        case 2:
        {
            m_ctrlChart.ui.setCurrentCoordByName("3");
        }
        break;
        case 3:
        {
            m_ctrlChart.ui.setCurrentCoordByName("4");
        }
        break;
        case 4:
        {
            m_ctrlChart.ui.setCurrentCoordByName("5");
        }
        break;
        default:
            break;
    }
}
