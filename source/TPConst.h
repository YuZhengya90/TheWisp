#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "GL/glut.h"

#pragma once

// screen height, width, border
#define SCRN_H              640
#define SCRN_W             1280
#define SCRN_B               20
#define WINDOW_TITLE     "TheWisp"

// menu width height item height, item gap, item width
#define MENU_W              200
#define MENU_H              600/*(SCRN_H - 2*SCRN_B)*/
#define MENU_ITEM_W         160
#define MENU_ITEM_H          40
#define MENU_ITEM_G          20


// illusion width, height, coordinate x point, y point.
#define ILLU_W             1000/*(SCRN_W - 4*SCRN_B - MENU_W)*/
#define ILLU_H              600/*(SCRN_H - 2*SCRN_B)*/

#define TP_MIN(a,b) ((a) < (b) ? (a) : (b))
#define TP_MAX(a,b) ((a) > (b) ? (a) : (b)) 

// when scroll mouse wheel, the factor in each case.
#define MOUSE_SCALE_FACTOR ((float)0.8)

// render screen in each 20ms. about 1000/20= 50frames
#define REDRAW_TIMER_ID  20