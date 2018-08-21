#pragma once

#include <stdlib.h>
#include "GL\glut.h"
#include "GL\GLAux.h"

struct TPTexture
{
	GLuint id;
	char* path;
};

struct TPTextureCollection
{
    TPTexture* tpTexture;
    unsigned collectionSize;
};

extern void TPLoadTexture();
extern GLuint TPGetTextureByOrder(unsigned order);