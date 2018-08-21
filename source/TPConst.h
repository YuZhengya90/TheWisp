#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include "GL/GLAux.h"
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
#define MENU_ITEM_H          60
#define MENU_ITEM_G          20


// illusion width, height, coordinate x point, y point.
#define ILLU_W             1000/*(SCRN_W - 4*SCRN_B - MENU_W)*/
#define ILLU_H              600/*(SCRN_H - 2*SCRN_B)*/
#define ILLU_COOR_X         500 //400
#define ILLU_COOR_Y         300 //220
#define ILLU_COOR_V         100

#define ILLU_COOR_SCALE_RATE  2

#define TP_MIN(a,b) ((a) < (b) ? (a) : (b))
#define TP_MAX(a,b) ((a) > (b) ? (a) : (b)) 

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 0x0003
#endif

#ifndef GLUT_WHEEL_DOWN
#define GLUT_WHEEL_DOWN 0x0004
#endif

#define MOUSE_SCALE_FACTOR ((float)0.8)