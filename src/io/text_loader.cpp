/* 
 * Copyright © 2010 Michał Siejak
 * Copyright © 2010 Miłosz Kosobucki
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
#include "vorticity/io/iobase.h"
#include "vorticity/io/text_loader.h"
#include "vorticity/resources/resource.h"

using namespace Vorticity;

TextLoader::TextLoader(const std::string& path) : FileReader(path)
{
}

TextLoader::~TextLoader()
{
}

void TextLoader::read()
{
	TextResource *text = (TextResource*) getResource();

	size_t fileSize;

	std::ifstream::pos_type begin = file.tellg();
	file.seekg(0, std::ios::end);
	std::ifstream::pos_type end = file.tellg();
	file.seekg(0, std::ios::beg);

	fileSize = (size_t)(end - begin);

	char *buf = new char[fileSize+1];

	file.read(buf, fileSize);
	buf[fileSize] = 0;
	text->setText(buf);

	delete[] buf;
}