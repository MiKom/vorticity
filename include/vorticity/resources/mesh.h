/* 
 * Copyright © 2009, 2010 Micha³ Siejak
 * Copyright © 2009, 2010 Mi³osz Kosobucki
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

#ifndef __VORTICITY_MESH_H
#define __VORTICITY_MESH_H

namespace Vorticity {

class FileReader;
class SubMesh;
class Material;

class VAPI Mesh : public Resource
{
protected:
	std::vector<SubMesh*>	subMeshes;
	std::vector<Material*>	materials;
public:
	Mesh(FileReader& reader);
	virtual ~Mesh();

	virtual string getType() const
	{ return "mesh"; }

	size_t getSubMeshCount() const	{ return subMeshes.size(); }
	size_t getMaterialCount() const	{ return materials.size(); }

	void		addSubMesh(SubMesh* submesh);
	SubMesh* 	getSubMesh(const size_t index);
	bool		removeSubMesh(const size_t index);

	void		addMaterial(Material* mat);
	Material* 	getMaterial(const size_t index);
	bool		removeMaterial(const size_t index);

	void		clearSubMeshes();
	void		clearMaterials();

	bool		createRenderBuffers();
	void		destroyRenderBuffers();
};

} // Vorticity

#endif