/* 
 * Copyright © 2010 Micha³ Siejak
 * Copyright © 2010 Mi³osz Kosobucki
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
#
include "config.h"
#include "vorticity/io/iobase.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/image.h"
#include "vorticity/io/image_loader.h"

#include <IL/il.h>

using namespace Vorticity;

bool ImageLoader::devilInit = false;

ImageLoader::ImageLoader(const std::string& path) : FileReader(path)
{
	if(!devilInit)
	{
		ilInit();
		devilInit = true;
	}
}

ImageLoader::~ImageLoader()
{
}

void ImageLoader::read()
{
	Image *img = static_cast<Image *> (getResource());
	
	ILuint imageId;
	imageId = ilGenImage();
	ilBindImage(imageId);
	
	ilLoadImage(path.c_str());

	ILenum error;
	if((error = ilGetError()) != IL_NO_ERROR){
		while((error = ilGetError()) != IL_NO_ERROR);
		ilBindImage(NULL);
		ilDeleteImages(1, &imageId);
		throw IOException("Cannot load image", path, IOBase::ReadError);
	}

	if(!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE))
		throw IOException("Image conversion failed", path, IOBase::ReadError);

	img->setWidth(ilGetInteger(IL_IMAGE_WIDTH));
	img->setHeight(ilGetInteger(IL_IMAGE_HEIGHT));
	img->setBpp(ilGetInteger(IL_IMAGE_BPP));

	img->setPixels(ilGetData());
	
	ilBindImage(NULL);
	ilDeleteImages(1, &imageId);
}
