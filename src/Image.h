#ifndef IMAGE_H
#define IMAGE_H

#include "Rect.h"
#include "Texture.h"

class Image
{
public:
	// public methods: -------------------------------------------------------------
	Image()
		: pText(nullptr), pImageRect(nullptr) {}

	Image(Texture* pTexture, Rect* pRect)
		: pText(pTexture), pImageRect(pRect) {}

	void Set(Texture* pTexture, Rect* pRect)
	{
		pText = pTexture;
		pImageRect = pRect;
	}

	~Image() {}

public:
	// data: -----------------------------------------------------------------------
	Rect* pImageRect;
	Texture* pText;
};


#endif