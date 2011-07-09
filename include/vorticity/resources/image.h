#ifndef __VORTICITY_RES_IMAGE_H
#define __VORTICITY_RES_IMAGE_H

namespace Vorticity {

class FileReader;

class VAPI Image : public Resource
{
protected:
	unsigned char* pixels;
	size_t	width;
	size_t	height;
	size_t	bpp;
	std::string path;
public:
	Image(FileReader& reader, TextureFilter filter=Bilinear);
	virtual ~Image();

	virtual string getType() const
	{ return "image"; }

	unsigned char* getPixels() const
	{ return pixels; }
	void setPixels(unsigned char* pixels);


	size_t getWidth() const
	{ return width; }
	void setWidth(size_t width)
	{ this->width = width; }

	size_t getHeight() const
	{ return height; }
	void setHeight(size_t height)
	{ this->height = height; }

	/**
	 * @return bytes per pixel
	 */
	size_t getBpp() const
	{ return bpp; }
	void setBpp(size_t bpp)
	{ this-> bpp = bpp; }
};

} // Vorticity

#endif