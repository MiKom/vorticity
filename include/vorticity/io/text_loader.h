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

#ifndef __VORTICITY_TEXTFILE_LOADER_H
#define __VORTICITY_TEXTFILE_LOADER_H

namespace Vorticity {

class VAPI TextLoader : public FileReader
{
private:
	virtual void read();
public:
	TextLoader(const std::string& path);
	virtual ~TextLoader();
};

} // Vorticity

#endif