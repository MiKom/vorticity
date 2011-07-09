#include "config.h"
#include "resources/resource.h"
#include "io/iobase.h"
#include "image.h"
#include "IL/il.h"

using namespace Vorticity;

Image::Image(FileReader& reader, TextureFilter filter) : Resource()
{
	width = height = bpp = 0;
	pixels = NULL;

	reader.setResource(this);
	reader.readFile();
	getCore()->getDevice()->createTexture(this, filter);
	path = reader.getPath(); // debug
}


Image::~Image()
{
	if(pixels)
		delete[] pixels;
	if(device)
		device->destroyTexture(getID());
}

void Image::setPixels(unsigned char *pixels)
{
	size_t imgSize = width * height * bpp;
	this->pixels = new unsigned char[imgSize];
	std::memcpy(this->pixels, pixels, imgSize);
}
