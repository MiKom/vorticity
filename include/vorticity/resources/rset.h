/* 
 * Copyright © 2009, 2010 Micha³ Siejak
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

#ifndef __VORTICITY_RSET_H
#define __VORTICITY_RSET_H

#include "vorticity/io/iobase.h"

namespace Vorticity {

class Resource;
class ResourceSet
{
protected:
	std::map<std::string, Resource*> resources;
public:
	ResourceSet() { }

	inline bool		hasResource(const std::string& path) const;
	inline void		clear();

	Resource* 		getResource(const std::string& path);
	bool			removeResource(const std::string& path);
	bool			addResource(const std::string& path, Resource* res);

	template <class T>
	T* loadResource(FileReader& reader)
	{
		T* resource;
		resource = dynamic_cast<T*>(getResource(reader.getPath()));
		if(resource == nullptr)
		{
			resource = new T(reader);
			addResource(reader.getPath(), resource);
		}
		return resource;
	}
};

} // Vorticity

#endif