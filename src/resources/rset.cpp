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

#include "config.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/rset.h"

using namespace Vorticity;

inline bool ResourceSet::hasResource(const std::string& path) const
{
	return resources.find(path) != resources.end();
}

inline void ResourceSet::clear()
{ resources.clear(); }

Resource* ResourceSet::getResource(const std::string& path)
{
	auto loc = resources.find(path);
	if(loc == resources.end())
		return NULL;
	return loc->second;
}

bool ResourceSet::addResource(const std::string& path, Resource *res)
{
	if(hasResource(path))
		return false;
	resources[path] = res;
	return true;
}

bool ResourceSet::removeResource(const std::string& path)
{
	auto loc = resources.find(path);
	if(loc != resources.end())
	{
		resources.erase(loc);
		return true;
	}
	return false;
}
