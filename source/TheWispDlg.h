
// TheWispDlg.h : header file
//

#pragma once

#include "TPOpenGL.h"
#include "afxdtctl.h"
#include "afxwin.h"

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

public:

    TPOpenGL        m_ctrlChart;

    int             m_iRadioBtnGroup0;

    CDateTimeCtrl   m_dateFromPredict1;
    CDateTimeCtrl   m_dateToPredict1;

    CDateTimeCtrl   m_dateFromPredict2;
    CDateTimeCtrl   m_dateToPredict2;

    CDateTimeCtrl   m_dateAtPredict3;
    CEdit           m_editStockQtyPredict3;
    CEdit           m_editSalesPricePredict3;
    double          m_dStockQtyPredict3;
    double          m_dSalesPricePredict3;

    CDateTimeCtrl   m_dateWhenPredict4;
    CDateTimeCtrl   m_dateToPredict4;
    double          m_dStockQtyPredict4;
    double          m_dSalesPricePredict4;
    double          m_dPurQtyPredict4;
    double          m_dPurPricePredict4;

    CDateTimeCtrl   m_dateWhenPredict5;
    CDateTimeCtrl   m_dateToPredict5;
    double          m_dStockQtyPredict5;
};
