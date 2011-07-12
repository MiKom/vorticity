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

#ifndef __VORTICITY_STDMESH_LOADER_H
#define __VORTICITY_STDMESH_LOADER_H

namespace Vorticity {

class SubMesh;
class Mesh;
class Material;

/**
 * \author Miłosz Kosobucki
 *
 * Mesh loader for Vorticity AVR format, for description 
 * see doc/new_model_format.txt
 **/
class VAPI StdMeshLoader : public IFFReader<StdMeshLoader>
{
protected:
	std::vector<std::pair<uint32_t, SubMesh*>> meshes;
	std::map<Material*, std::vector<uint32_t>> materials;
	std::vector<std::string> textures;
protected:
	bool VMSHCallback();
	bool GEOMCallback();
	bool MESHCallback();
	bool MATSCallback();
	bool MATRCallback();
	bool TEXSCallback();
	bool TEXTCallback();

	void postload();
	
	/**
	 * Gets properly casted Mesh that is being furnished
	 **/
	Mesh* getMesh()
	{ return (Mesh*)getResource(); }
private:
	//preventing copying
	StdMeshLoader(const StdMeshLoader&);
	StdMeshLoader& operator=(const StdMeshLoader&);

public:
	static const uint16_t LATEST_VERSION = 0x0001;
	StdMeshLoader(const std::string& path);
	virtual ~StdMeshLoader();
};

} // Vorticity

#endif
