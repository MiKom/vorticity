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
#include<cstdint>
#include<ostream>
#include"avrutil.h"

using namespace std;

namespace AVR
{

std::ios::pos_type
startChunk(std::ostream& os, const std::string& id)
{
	auto startPos = os.tellp();
	os.write(id.c_str(), id.length());
	os.seekp(4, std::ios::cur); //reserving space for chunk size
	return startPos;
}

void 
endChunk(std::ostream& os, 
         const std::string& id,
         const std::ios::pos_type& startPos)
{
	ios::pos_type endPos = os.tellp();
	ios::pos_type dataStart = startPos + 
	                static_cast<ios::pos_type>(id.length()) +
	                static_cast<ios::pos_type>(4);
	ios::pos_type sizePos = startPos +
	                static_cast<ios::pos_type>(id.length());
	uint32_t chunkSize = endPos - dataStart;
	os.seekp(sizePos);
	os.write((char*) &chunkSize, 4);
	os.seekp(endPos);
	if(os.tellp() % 2 == 1){ //Next chunk should always start on even byte
		os.put(0xFF);
	}
}

} //AVR
