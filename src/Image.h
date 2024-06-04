#ifndef IMAGE_H
#define IMAGE_H

#include "Rect.h"
#include "Texture.h"

class Image
{
public:
	// public methods: -------------------------------------------------------------
	Image()
		: pImageRect(nullptr), pText(nullptr) {}

	Image(Texture* pTexture, Rect* pRect)
		: pImageRect(nullptr), pText(nullptr) {}

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