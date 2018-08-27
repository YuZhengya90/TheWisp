
// TheWispDlg.h : header file
//

#pragma once

#include "TPOpenGL.h"
#include "afxdtctl.h"

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

public:
    TPOpenGL m_ctrlChart;
    CDateTimeCtrl m_ctrlPredictFrom;
    CDateTimeCtrl m_ctrlPredictTo;
    afx_msg void OnBnClickedBtnPredict();
};
