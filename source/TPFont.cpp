#include "freetype/ft2build.h"

#include "freetype/freetype.h"
#include "freetype/ftglyph.h"

#include "TPFont.h"

#define ASCII_READ_MAX 128
#define ORI_FONT_SIZE   40

GLuint TextFont;
TPFontCollecton tpFontCollection;

void TPDisplayString(const char *s, float x, float y)
{
    glRasterPos3f(x, y, 0);
    glPushAttrib(GL_LIST_BIT);

    for (; *s != '\0'; ++s)
        glCallList(TextFont + *s);

    glPopAttrib();
}

int TPDisplayString2(const char* txt, GLfloat x, GLfloat y, GLfloat size, GLuint fontType)
{
	glPushAttrib(GL_LIST_BIT);
	
	unsigned txtLength = strlen(txt);
	float xAnch = x;
    float XscreenToCoordFactor = (float)10 / 1000;
    float YscreenToCoordFactor = (float)10 / 600;

	
	float rateOfSize = size / ORI_FONT_SIZE;
	for (unsigned i = 0; i < strlen(txt); ++i)
	{
		TPFont font = tpFontCollection.tpFont[txt[i] + fontType * ASCII_READ_MAX];
		if (i != 0)
		{
			TPFont fontPrev = tpFontCollection.tpFont[txt[i - 1] + fontType * ASCII_READ_MAX];
            xAnch += (fontPrev.w + font.l) * rateOfSize * XscreenToCoordFactor;
            
		}
		
		glRasterPos2f(xAnch, y - (font.h - font.t) * rateOfSize * YscreenToCoordFactor);
		glPixelZoom(rateOfSize, rateOfSize);
		glDrawPixels(font.p, font.h, GL_ALPHA, GL_UNSIGNED_BYTE, font.bitmap);
	}
	glPopAttrib();
	return 0;
}

static void revMemcpy(unsigned char* dst, unsigned char* src, unsigned pitch, unsigned rows)
{
	for (unsigned i = 0; i < rows; ++i)
		for (unsigned j = 0; j < pitch; ++j)
			dst[i * pitch + j] = src[(rows - i - 1) * pitch + j];
}

static void LoadFontByOneChar(TPFont* tpFont, FT_Face pFTFace, char ch)
{
	FT_Load_Glyph(pFTFace, FT_Get_Char_Index(pFTFace, ch), FT_LOAD_DEFAULT);
	FT_Glyph glyph;
	FT_Error error = FT_Get_Glyph(pFTFace->glyph, &glyph);
	if (!error)
	{		
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;
		
		int bitmapSize = bitmap.pitch * bitmap.rows;
		tpFont->bitmap = (unsigned char*)malloc(bitmapSize);
		revMemcpy(tpFont->bitmap, bitmap.buffer, bitmap.pitch, bitmap.rows);
		tpFont->h = bitmap.rows;
		tpFont->w = bitmap.width;
		tpFont->p = bitmap.pitch;
		tpFont->l = bitmap_glyph->left;
		tpFont->t = bitmap_glyph->top;

		FT_Done_Glyph(glyph);
		glyph = NULL;
	}
}

static int LoadFontFromPath(unsigned typeOrder, char* path)
{
	FT_Library pFTLib = NULL;
	FT_Face pFTFace = NULL;
	FT_Error error = FT_Init_FreeType(&pFTLib);
	if (error) return -1;

	signed int dotWidth = 0;

	error = FT_New_Face(pFTLib, path, 0, &pFTFace);
	if (!error)
	{		
		FT_Set_Char_Size(pFTFace, 1024, 1024, 300, 300);
		for (unsigned char c = 0; c < ASCII_READ_MAX; c++)
		{
			unsigned order = typeOrder * ASCII_READ_MAX + c;
			tpFontCollection.tpFont[order].fontPath = path;
			tpFontCollection.tpFont[order].c = c;

			LoadFontByOneChar(&(tpFontCollection.tpFont[order]), pFTFace, c);

			// patch for blank
			if (c == ',')
			{
				dotWidth = tpFontCollection.tpFont[order].w;
			}
		}

		tpFontCollection.tpFont[typeOrder * ASCII_READ_MAX + ' '].w = dotWidth;
		tpFontCollection.tpFont[typeOrder * ASCII_READ_MAX + ' '].p = dotWidth;

		FT_Done_Face(pFTFace);
	}
	
	FT_Done_FreeType(pFTLib);
    return 0;
}

int TPLoadFont()
{
	const unsigned tpType = 1;
	tpFontCollection.collectSize = ASCII_READ_MAX * tpType;
	tpFontCollection.tpFont = (TPFont *)malloc(sizeof(TPFont) * tpFontCollection.collectSize);
	
	if (!tpFontCollection.tpFont) {
		return - 1;
	}


	// test;
	{
		LoadFontFromPath(0, "C:\\Windows\\Fonts\\ARIALN.TTF");
	}

    // new string format
    TextFont = glGenLists(ASCII_READ_MAX);
    wglUseFontBitmaps(wglGetCurrentDC(), 0, ASCII_READ_MAX, TextFont);

	return 0;
}