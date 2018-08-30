#pragma once
#include "afxwin.h"
#include "GL\GL.H"
#include "TPConst.h"
#include "TPUI.h"
#include "TPJar.h"

#define HI_HACKATHON "Hi BusinessOne, Hi Hackathon!"
#define PRE_PURCHASE_PRICE "Predict Purchase Price"
#define PRE_SALES_PRICE "Predict Sales Price"
#define PRE_TEMPERATURE "Predict Temperature"
#define PRE_SALES_QUANTITY "Predict Sales Quantity"
#define PRE_PROFIT "Predict Profit"
#define OPR_ADVICE "Operation Advice"

class TPOpenGL : public CWnd
{
public:
    TPOpenGL();
    ~TPOpenGL();

    bool Init(CWnd* mainCwnd);

    void SetupPixelFormat(HDC hDC);

    HDC hdc;
    HGLRC hglrc;

    GLfloat step, s;

    TPUI    ui;
    TPJar   jar;	

    virtual void PreSubclassWindow();

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnInitMenuPopup(CMenu *pMenuPopup, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnEnableTable();
	afx_msg void OnEnableChart();
	afx_msg void OnEnableMesh();
	afx_msg void OnEnableCrossLine();
	afx_msg void OnEnableCurve();
	afx_msg void OnEnablePoint();
	afx_msg void OnAbout();

protected:
    void TPInitGL();
    void TPInitUI(HWND hwnd);

private:
	void HiHackathon();

public:
    void PredictPurchasePrice(CTime predictFrom, CTime predictTo);
    void PredictSalesPrice(CTime predictFrom, CTime predictTo);
    void PredictTemperature(CTime predictFrom, CTime predictTo);
    void PredictSalesQuantity(CTime predictAt, double temp, double dStockQty, double dSalesPrice);
    void PredictProfit(CTime predictWhen, CTime predictTo, double temp, double dStockQty, double dSalesPrice, double dPurQty, double dPurPrice);
    void OperationAdvice(CTime predictWhen, CTime predictTo, double temp, double dStockQty);
};

