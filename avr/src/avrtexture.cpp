/* 
 * Copyright © 2012 Miłosz Kosobucki
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

#include <vector>
#include <ostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include "avrtexture.h"

using namespace AVR;

void AVR::write(std::ostream& os, const AVRTexture& texture)
{
	os.write("TEXT", 4);
	auto sizePos = os.tellp();
	os.seekp(4, std::ios::cur);
	auto startPos = os.tellp();

	os.write(texture.mPath.c_str(), texture.mPath.length() + 1);

	auto endPos = os.tellp();
	uint32_t size = endPos - startPos;
	os.seekp(sizePos);
	os.write((char*) &size, 4);
	os.seekp(endPos);
}
