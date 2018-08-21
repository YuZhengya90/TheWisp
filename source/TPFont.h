#pragma once 

#include "TPConst.h"

struct TPFont
{
	// character
	char c;

	// fontpath
	char* fontPath;

	// bitmap pointer
	unsigned char* bitmap;

	// height
	signed int h;

	//width
	signed int w;

	// pitch
	signed int p;

	// left 
	signed int l;

	// top (baseline)
	signed int t;
};

struct TPFontCollecton
{
	TPFont* tpFont;       // 128 * n (types)
	unsigned collectSize;
};

// can't change size but you can set color.
extern void TPDisplayString(const char* txt, float x, float y);

// you can choose fonttype and size before.
extern int TPDisplayString2(const char* txt, GLfloat x, GLfloat y, GLfloat size, GLuint fontType);
extern int TPLoadFont();