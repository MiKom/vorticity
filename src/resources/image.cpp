/* 
 * Copyright © 2009, 2010 Michał Siejak
 * Copyright © 2009, 2010 Miłosz Kosobucki
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

#include "config.h"
#include "vorticity/resources/resource.h"
#include "vorticity/io/iobase.h"
#include "vorticity/resources/image.h"

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
