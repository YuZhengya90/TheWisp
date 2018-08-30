
// TheWispDlg.h : header file
//

#pragma once

#include "TPOpenGL.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "atltime.h"

// CAboutDlg dialog used for App About
enum PredictRadioIndex
{
    PURCHASE_PRICE = 0,
    SALES_PRICE,
    TEMPERATURE,
    SALES_QUANTITY,
    PROFIT,
    OPERATION_ADVICE
};

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


// CTheWispDlg dialog
class CTheWispDlg : public CDialogEx
{
// Construction
public:
	CTheWispDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_THEWISP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    afx_msg void OnBnClickedBtnPredict();

    afx_msg void OnRadioBtnGroup0Clicked();

    afx_msg LRESULT OnSalesQuantityLoadComplete(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnProfitLoadComplete(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdviceLoadComplete(WPARAM wParam, LPARAM lParam);

public:

    TPOpenGL m_ctrlChart;

    int     m_iRadioBtnGroup0;

    CTime   m_purPrice_DateFrom;
    CTime   m_purPrice_DateTo;

    CTime   m_salPrice_DateFrom;
    CTime   m_salPrice_DateTo;

    CTime   m_temp_DateFrom;
    CTime   m_temp_DateTo;

    CTime   m_salQty_DateAt;
    double  m_salQty_Temperature;
    double  m_salQty_StockQty;
    double  m_SalQty_SalesPrice;

    CTime   m_profit_DateWhen;
    CTime   m_profit_DateTo;
    double  m_profit_Temperature;
    double  m_profit_StockQty;
    double  m_profit_SalesPrice;
    double  m_profit_PurQty;
    double  m_profit_PurPrice;

    CTime   m_advice_DateWhen;
    CTime   m_advice_DateTo;
    double  m_advice_Temperature;
    double  m_advice_StockQty;
};
