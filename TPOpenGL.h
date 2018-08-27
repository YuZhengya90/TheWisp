#pragma once
#include "afxwin.h"
#include "source\GL\GL.H"
#include "source\TPConst.h"
#include "source\TPUI.h"

class TPOpenGL : public CWnd
{
public:
    TPOpenGL();
    ~TPOpenGL();

    void SetupPixelFormat(HDC hDC);

    HDC hdc;
    HGLRC hglrc;

    GLfloat step, s;

    TPUI ui;

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

protected:
    void TPInitGL();
    void TPInitUI(HWND hwnd);
};

