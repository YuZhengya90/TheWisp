#include "stdafx.h"
#include "TPTexture.h"

TPTextureCollection tpTextureCollection;

static int LoadGLTextures(GLuint& unTexture, const char* chFileName)
{
	AUX_RGBImageRec *TextureImage;
	TextureImage = auxDIBImageLoadA(chFileName);

	glGenTextures(1, &unTexture);
	glBindTexture(GL_TEXTURE_2D, unTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (TextureImage)
	{
		if (TextureImage->data)
			free(TextureImage->data);
		free(TextureImage);
	}

	return 0;
}

void TPLoadTexture()
{
	glEnable(GL_TEXTURE_2D);

	char* path[] =
	{
		"resource\\A.bmp",
		"resource\\B.bmp",
		"resource\\C.bmp"
	};

    tpTextureCollection.collectionSize = sizeof(path) / sizeof(path[0]);
    tpTextureCollection.tpTexture = (TPTexture *)malloc(sizeof(TPTexture)* tpTextureCollection.collectionSize);

    for (GLuint i = 0; i < tpTextureCollection.collectionSize; ++i)
	{
        tpTextureCollection.tpTexture[i].path = path[i];
        LoadGLTextures(tpTextureCollection.tpTexture[i].id, tpTextureCollection.tpTexture[i].path);
	}
}

GLuint TPGetTextureByOrder(unsigned order)
{
    if (order < tpTextureCollection.collectionSize)
        return tpTextureCollection.tpTexture[order].id;
	return -1;
}