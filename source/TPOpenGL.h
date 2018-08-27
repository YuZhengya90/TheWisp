#pragma once
#include "afxwin.h"
#include "GL\GL.H"
#include "TPConst.h"
#include "TPUI.h"
#include "TPJar.h"

class TPOpenGL : public CWnd
{
public:
    TPOpenGL();
    ~TPOpenGL();

    void SetupPixelFormat(HDC hDC);

    HDC hdc;
    HGLRC hglrc;

    GLfloat step, s;

    TPUI    ui;
    TPJar   jar;	

    virtual void PreSubclassWindow();

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
	afx_msg void OnEnableMesh();
	afx_msg void OnEnableCrossLine();
	afx_msg void OnEnableCurve();
	afx_msg void OnAbout();

protected:
    void TPInitGL();
    void TPInitUI(HWND hwnd);

public:
    void PredictModel1(CTime predictFrom, CTime predictTo);
    void PredictModel2(CTime predictFrom, CTime predictTo);
};

