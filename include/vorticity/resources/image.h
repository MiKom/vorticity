/* 
 * Copyright © 2009 Michał Siejak
 * Copyright © 2009 Miłosz Kosobucki
 *
 * All rights reserved.
 * 
 * This file is part of Vorticity.
 *
 * Vorticity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vorticity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vorticity.  If not, see <http://www.gnu.org/licenses/>.
 */

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