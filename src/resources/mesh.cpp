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

#include "config.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/material.h"
#include "vorticity/resources/image.h"
#include "vorticity/core/renderer.h"
#include "vorticity/io/iobase.h"
#include "vorticity/resources/mesh.h"
#include "vorticity/resources/submesh.h"

using namespace Vorticity;

Mesh::Mesh(FileReader& reader) : Resource()
{
	reader.setResource(this);
	reader.readFile();
}

Mesh::~Mesh()
{
	clearMaterials();
	clearSubMeshes();
}

void Mesh::addSubMesh(SubMesh* submesh)
{
	submesh->setMesh(this);
	subMeshes.push_back(submesh);
}

SubMesh* Mesh::getSubMesh(const size_t index)
{
	if(index >= subMeshes.size())
		return NULL;
	return subMeshes[index];
}

bool Mesh::removeSubMesh(const size_t index)
{
	if(index >= subMeshes.size())
		return false;
	subMeshes.erase(subMeshes.begin() + index);
	return true;
}

void Mesh::addMaterial(Material* mat)
{
	materials.push_back(mat);
}

Material* Mesh::getMaterial(const size_t index)
{
	if(index >= materials.size())
		return NULL;
	return materials[index];
}

bool Mesh::removeMaterial(const size_t index)
{
	if(index >= materials.size())
		return false;
	materials.erase(materials.begin() + index);
	return true;
}

void Mesh::clearMaterials()
{
	for(auto i=materials.begin(); i<materials.end(); i++)
		delete *i;
	materials.clear();
}

void Mesh::clearSubMeshes()
{
	for(auto i=subMeshes.begin(); i<subMeshes.end(); i++)
		delete *i;
	subMeshes.clear();
}

bool Mesh::createRenderBuffers()
{
	for(auto i=subMeshes.begin(); i<subMeshes.end(); i++)
	{
		if(!(*i)->createRenderBuffers())
			return false;
	}
	return true;
}

void Mesh::destroyRenderBuffers()
{
	for(auto i=subMeshes.begin(); i<subMeshes.end(); i++)
		(*i)->destroyRenderBuffers();
}
