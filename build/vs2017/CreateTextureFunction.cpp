#include "CreateTextureFunction.h"

gef::Texture* CreateTextureWithPNG(char* fileName, gef::Platform& platform)
{
	gef::Texture* tempTexture = nullptr;
	gef::PNGLoader pngLoader;
	gef::ImageData imageData;

	pngLoader.Load(fileName, platform, imageData); //load data into from png

	if (imageData.image() != nullptr) { //if image is not nullptr
		tempTexture = gef::Texture::Create(platform, imageData);
	}

	return tempTexture; //return texture from png
}