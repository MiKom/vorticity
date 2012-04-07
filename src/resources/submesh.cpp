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
#include "vorticity/io/iobase.h"
#include "vorticity/core/renderer.h"
#include "vorticity/resources/mesh.h"
#include "vorticity/resources/submesh.h"

using namespace Vorticity;

TriangleFace::TriangleFace(const uint32_t* vertex, const uint32_t* normal, const uint32_t* texcoord)
	: mesh(NULL)
{
	std::memset(this->vertex, 0, 3*sizeof(uint32_t));
	std::memset(this->normal, 0, 3*sizeof(uint32_t));
	std::memset(this->texcoord, 0, 3*sizeof(uint32_t));

	if(vertex)
		std::memcpy(this->vertex, vertex, 3*sizeof(uint32_t));
	if(normal)
		std::memcpy(this->normal, normal, 3*sizeof(uint32_t));
	if(texcoord)
		std::memcpy(this->texcoord, normal, 3*sizeof(uint32_t));
}


SubMesh::SubMesh(const string& name, Mesh* owner) 
	: Resource(), name(name), mesh(owner), material(NULL)
{
	if(mesh)
		mesh->addSubMesh(this);
}

SubMesh::~SubMesh()
{
	destroyRenderBuffers();

	clearVertices();
	clearNormals();
	clearTexCoords();
	clearFaces();
}

bool SubMesh::setVertexCount(const size_t count)
{
	if(count < vertices.size())
		return false;
	vertices.resize(count, vector3());
	return true;
}

bool SubMesh::setNormalCount(const size_t count)
{
	if(count <  normals.size())
		return false;
	normals.resize(count, vector3());
	return true;
}

bool SubMesh::setTexCoordCount(const size_t count)
{
	if(count <  texcoords.size())
		return false;
	texcoords.resize(count, vector2());
	return true;
}

bool SubMesh::setFaceCount(const size_t count)
{
	if(count < faces.size())
		return false;
	faces.resize(count, TriangleFace());
	return true;
}

const size_t SubMesh::getRenderElementCount() const
{
	return faces.size() * 3;
}

bool SubMesh::createRenderBuffers()
{
	if(hasRenderBuffers())
		return false;

	size_t renderElements = getRenderElementCount();
	id = device->createVertexBuffer();

	if(!device->allocVertexBuffer(id, VertexBuffer, renderElements))
		return false;
	if(!device->allocVertexBuffer(id, NormalBuffer, renderElements))
		return false;
	if(!device->allocVertexBuffer(id, TexCoordBuffer, renderElements))
		return false;

	float* vram_ptr;
	// Vertices
	vram_ptr = (float*)device->lockVertexBuffer(id, VertexBuffer);
	if(vram_ptr == NULL) return false;
	for(size_t i=0; i<faces.size(); i++)
	{
		std::memcpy(vram_ptr, faces[i].getVertex(0).cell, 3*sizeof(float)); vram_ptr += 3;
		std::memcpy(vram_ptr, faces[i].getVertex(1).cell, 3*sizeof(float)); vram_ptr += 3;
		std::memcpy(vram_ptr, faces[i].getVertex(2).cell, 3*sizeof(float)); vram_ptr += 3;
	}
	device->unlockVertexBuffer(id);
	
	// Normals
	vram_ptr = (float*)device->lockVertexBuffer(id, NormalBuffer);
	if(vram_ptr == NULL) return false;
	for(size_t i=0; i<faces.size(); i++)
	{
		std::memcpy(vram_ptr, faces[i].getNormal(0).cell, 3*sizeof(float)); vram_ptr += 3;
		std::memcpy(vram_ptr, faces[i].getNormal(1).cell, 3*sizeof(float)); vram_ptr += 3;
		std::memcpy(vram_ptr, faces[i].getNormal(2).cell, 3*sizeof(float)); vram_ptr += 3;
	}
	device->unlockVertexBuffer(id);

	// Texture coords
	vram_ptr = (float*)device->lockVertexBuffer(id, TexCoordBuffer);
	if(vram_ptr == NULL) return false;
	for(size_t i=0; i<faces.size(); i++)
	{
		std::memcpy(vram_ptr, faces[i].getTexCoord(0).cell, 2*sizeof(float)); vram_ptr += 2;
		std::memcpy(vram_ptr, faces[i].getTexCoord(1).cell, 2*sizeof(float)); vram_ptr += 2;
		std::memcpy(vram_ptr, faces[i].getTexCoord(2).cell, 2*sizeof(float)); vram_ptr += 2;
	}
	device->unlockVertexBuffer(id);
	return true;
}

bool SubMesh::hasRenderBuffers() const
{ return getID() != 0; }

void SubMesh::destroyRenderBuffers()
{
	if(!hasRenderBuffers())
		return;
	if(device->destroyVertexBuffer(id))
		id = 0;
}
