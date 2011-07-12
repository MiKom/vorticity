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

#ifndef __VORTICITY_IMAGE_LOADER_H
#define __VORTICITY_IMAGE_LOADER_H

namespace Vorticity {

class VAPI ImageLoader : public FileReader
{
private:
	virtual void read();
	static bool devilInit;
	
	//prevent copying
	ImageLoader(const ImageLoader&);
	ImageLoader& operator=(const ImageLoader&);
public:
	ImageLoader(const std::string& path);
	virtual ~ImageLoader();
};

} // Vorticity

#endif
