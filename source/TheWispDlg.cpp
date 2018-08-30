
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
    , m_profit_StockQty(0)
    , m_profit_SalesPrice(0)
    , m_salQty_StockQty(0)
    , m_SalQty_SalesPrice(0)
    , m_profit_PurQty(0)
    , m_profit_PurPrice(0)
    , m_advice_StockQty(0)
    , m_salQty_Temperature(0)
    , m_profit_Temperature(0)
    , m_advice_Temperature(0)
    , m_temp_DateFrom(0)
    , m_temp_DateTo(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTheWispDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PIC_CHART, m_ctrlChart);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_purPrice_DateFrom);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_purPrice_DateTo);
    DDX_Radio(pDX, IDC_RADIO1, m_iRadioBtnGroup0);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_salPrice_DateFrom);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_salPrice_DateTo);

    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER5, m_salQty_DateAt);
    DDX_Text(pDX, IDC_EDIT8, m_salQty_Temperature);
    DDX_Text(pDX, IDC_EDIT1, m_salQty_StockQty);
    DDX_Text(pDX, IDC_EDIT2, m_SalQty_SalesPrice);

    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER6, m_profit_DateWhen);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER7, m_profit_DateTo);
    DDX_Text(pDX, IDC_EDIT3, m_profit_StockQty);
    DDX_Text(pDX, IDC_EDIT6, m_profit_SalesPrice);

    DDX_Text(pDX, IDC_EDIT5, m_profit_PurQty);
    DDX_Text(pDX, IDC_EDIT4, m_profit_PurPrice);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER8, m_advice_DateWhen);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER9, m_advice_DateTo);
    DDX_Text(pDX, IDC_EDIT7, m_advice_StockQty);

    DDX_Text(pDX, IDC_EDIT9, m_profit_Temperature);
    DDX_Text(pDX, IDC_EDIT10, m_advice_Temperature);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER10, m_temp_DateFrom);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER11, m_temp_DateTo);
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
    ON_MESSAGE(WM_SALQUN_COMPLETE_MESSAGE, &CTheWispDlg::OnSalesQuantityLoadComplete)
    ON_MESSAGE(WM_PROFIT_COMPLETE_MESSAGE, &CTheWispDlg::OnProfitLoadComplete)
    ON_MESSAGE(WM_ADVICE_COMPLETE_MESSAGE, &CTheWispDlg::OnAdviceLoadComplete)
    ON_MESSAGE(WM_TEMPER_COMPLETE_MESSAGE, &CTheWispDlg::OnTemperatureLoadComplete)
END_MESSAGE_MAP()


// CTheWispDlg message handlers

BOOL CTheWispDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    if (!m_ctrlChart.Init(this))
    {
        return false;
    }

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
    CTime originalDate(2017, 4, 1, 0, 0, 0);

    m_purPrice_DateFrom = originalDate;
    m_purPrice_DateTo = originalDate;

    m_salPrice_DateFrom = originalDate;
    m_salPrice_DateTo = originalDate;

    m_temp_DateFrom = originalDate;
    m_temp_DateTo = originalDate;

    m_salQty_DateAt = originalDate;

    m_profit_DateWhen = originalDate;
    m_profit_DateTo = originalDate;

    m_advice_DateWhen = originalDate;
    m_advice_DateTo = originalDate;

    UpdateData(FALSE);

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
        case PURCHASE_PRICE:
        {
            if (m_purPrice_DateFrom > m_purPrice_DateTo)
            {
                MessageBox(CString("The date range is not correct!!!"));
                return;
            }

            m_ctrlChart.PredictPurchasePrice(m_purPrice_DateFrom, m_purPrice_DateTo);
        }
        break;
        case SALES_PRICE:
        {
            if (m_salPrice_DateFrom > m_salPrice_DateTo)
            {
                MessageBox(CString("The date range is not correct!!!"));
                return;
            }

            m_ctrlChart.PredictSalesPrice(m_salPrice_DateFrom, m_salPrice_DateTo);
        }
        break;
        case TEMPERATURE:
        {
            m_ctrlChart.PredictTemperature(m_temp_DateFrom, m_temp_DateTo);
        }
        break;
        case SALES_QUANTITY:
        {
            if (m_salQty_StockQty < 0)
            {
                MessageBox(CString("The stock quantity must be greater than 0."));
                return;
            }
            if (m_SalQty_SalesPrice < 0)
            {
                MessageBox(CString("The sales price must be greater than 0."));
                return;
            }

            m_ctrlChart.PredictSalesQuantity(m_salQty_DateAt, m_salQty_Temperature, m_salQty_StockQty, m_SalQty_SalesPrice);
        }
        break;
        case PROFIT:
        {
            if (m_profit_DateWhen > m_profit_DateTo)
            {
                MessageBox(CString("The date range is not correct!!!"));
                return;
            }
            if (m_profit_StockQty < 0)
            {
                MessageBox(CString("The stock quantity must be greater than 0."));
                return;
            }
            if (m_profit_SalesPrice < 0)
            {
                MessageBox(CString("The sales price must be greater than 0."));
                return;
            }
            if (m_profit_PurQty < 0)
            {
                MessageBox(CString("The purchase quantity must be greater than 0."));
                return;
            }
            if (m_profit_PurPrice < 0)
            {
                MessageBox(CString("The purchase price must be greater than 0."));
                return;
            }

            m_ctrlChart.PredictProfit(m_profit_DateWhen, m_profit_DateTo, m_profit_Temperature, m_profit_StockQty, m_profit_SalesPrice, m_profit_PurQty, m_profit_PurPrice);
        }
        break;
        case OPERATION_ADVICE:
        {
            if (m_advice_DateWhen > m_advice_DateTo)
            {
                MessageBox(CString("The date range is not correct!!!"));
                return;
            }
            if (m_advice_StockQty < 0)
            {
                MessageBox(CString("The stock quantity must be greater than 0."));
                return;
            }

            m_ctrlChart.OperationAdvice(m_advice_DateWhen, m_advice_DateTo, m_advice_Temperature, m_advice_StockQty);
        }
        break;
    }
}


void CTheWispDlg::OnRadioBtnGroup0Clicked()
{
    UpdateData(TRUE);
    switch (m_iRadioBtnGroup0)
    {
        case PURCHASE_PRICE:
        {
            m_ctrlChart.ui.setCurrentCoordByName(PRE_PURCHASE_PRICE);
        }
        break;
        case SALES_PRICE:
        {
            m_ctrlChart.ui.setCurrentCoordByName(PRE_SALES_PRICE);
        }
        break;
        case TEMPERATURE:
        {
            m_ctrlChart.ui.setCurrentCoordByName(PRE_TEMPERATURE);
        }
        break;
        case SALES_QUANTITY:
        {
            m_ctrlChart.ui.setCurrentCoordByName(PRE_SALES_QUANTITY);
        }
        break;
        case PROFIT:
        {
            m_ctrlChart.ui.setCurrentCoordByName(PRE_PROFIT);
        }
        break;
        case OPERATION_ADVICE:
        {
            m_ctrlChart.ui.setCurrentCoordByName(OPR_ADVICE);
        }
        break;
        default:
            break;
    }
}

LRESULT CTheWispDlg::OnSalesQuantityLoadComplete(WPARAM wParam, LPARAM lParam)
{
    ((CButton *)GetDlgItem(IDC_RADIO3))->EnableWindow(true);
    return 1;
}

LRESULT CTheWispDlg::OnProfitLoadComplete(WPARAM wParam, LPARAM lParam)
{
    ((CButton *)GetDlgItem(IDC_RADIO4))->EnableWindow(true);
    return 1;
}

LRESULT CTheWispDlg::OnAdviceLoadComplete(WPARAM wParam, LPARAM lParam)
{
    ((CButton *)GetDlgItem(IDC_RADIO6))->EnableWindow(true);
    return 1;
}

LRESULT CTheWispDlg::OnTemperatureLoadComplete(WPARAM wParam, LPARAM lParam)
{
    ((CButton *)GetDlgItem(IDC_RADIO5))->EnableWindow(true);
    return 1;
}

