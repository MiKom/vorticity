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

#pragma once

#if !defined (__AVRXX_H_INSIDE__) && !defined (AVR_COMPILATION)
#error "Only <avr/avr++.h> can be included directly."
#endif

namespace AVR
{

class AVRTexture
{
protected:
	std::string mPath;
public:
	std::string getPath() const { return mPath;}
	void setPath(const std::string& path) { mPath = path; }
	friend void write(std::ostream& os, const AVRTexture& texture);
};

void write(std::ostream& os, const AVRTexture& texture);
} //AVR
