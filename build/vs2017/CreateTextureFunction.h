#pragma once
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>

gef::Texture* CreateTextureWithPNG(char* fileName, gef::Platform& platform);