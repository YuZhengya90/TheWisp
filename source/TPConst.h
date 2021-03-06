#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "GL/glut.h"

#pragma once

// screen height, width, border
#define SCRN_H              540
#define SCRN_W             1160
#define SCRN_B               20
#define WINDOW_TITLE     "TheWisp"

// menu width height item height, item gap, item width
//#define MENU_W              200
//#define MENU_H              600/*(SCRN_H - 2*SCRN_B)*/
//#define MENU_ITEM_W         160
//#define MENU_ITEM_H          40
//#define MENU_ITEM_G          20


// illusion width, height, coordinate x point, y point.
#define ILLU_W             1120/*(SCRN_W - 2*SCRN_B)*/
#define ILLU_H              500/*(SCRN_H - 2*SCRN_B)*/

#define TP_MIN(a,b) ((a) < (b) ? (a) : (b))
#define TP_MAX(a,b) ((a) > (b) ? (a) : (b)) 

// when scroll mouse wheel, the factor in each case.
#define MOUSE_SCALE_FACTOR ((float)0.8)

// render screen in each 20ms. about 1000/20= 50frames
#define REDRAW_TIMER_ID  20
#define HOVER_TIMER_ID   30

#define I2F(a) (a < 0 ? -(float)((int)(-(a))) : (float)a)
#define FEZ(a) ((a > -0.00000001) && (a < 0.00000001))

#define WM_SALQUN_COMPLETE_MESSAGE   (WM_USER + 1100)
#define WM_PROFIT_COMPLETE_MESSAGE   (WM_USER + 1101)
#define WM_ADVICE_COMPLETE_MESSAGE   (WM_USER + 1102)
#define WM_TEMPER_COMPLETE_MESSAGE   (WM_USER + 1103)