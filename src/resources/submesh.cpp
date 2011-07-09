#include "config.h"
#include "resource.h"
#include "io\iobase.h"
#include "core\renderer.h"
#include "mesh.h"
#include "submesh.h"

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

inline const uint32_t& TriangleFace::getVertexIndex(const size_t i) const
{ return vertex[i]; 	}

inline const uint32_t& TriangleFace::getNormalIndex(const size_t i) const
{ return normal[i]; 	}

inline const uint32_t& TriangleFace::getTexCoordIndex(const size_t i) const
{ return texcoord[i]; 	}

inline const bool TriangleFace::getVertexIndex(const size_t i, uint32_t &index)
{
	if(i >= 3) return false;
	index = vertex[i];
	return true;
}

inline const bool TriangleFace::getNormalIndex(const size_t i, uint32_t &index)
{
	if(i >= 3) return false;
	index = normal[i];
	return true;
}

inline const bool TriangleFace::getTexCoordIndex(const size_t i, uint32_t &index)
{
	if(i >= 3) return false;
	index = texcoord[i];
	return true;
}

inline bool TriangleFace::setVertexIndex(const size_t i, const uint32_t index)
{
	if(i >= 3) return false;
	vertex[i] = index;
	return true;
}

inline bool TriangleFace::setNormalIndex(const size_t i, const uint32_t index)
{
	if(i >= 3) return false;
	normal[i] = index;
	return true;
}

inline bool TriangleFace::setTexCoordIndex(const size_t i, const uint32_t index)
{
	if(i >= 3) return false;
	texcoord[i] = index;
	return true;
}

inline vector3& TriangleFace::getVertex(const size_t i)
{ return mesh->vertices[vertex[i]]; 		}

inline vector3& TriangleFace::getNormal(const size_t i)
{ 
	return mesh->normals[normal[i]]; 	
}

inline vector2& TriangleFace::getTexCoord(const size_t i)
{ return mesh->texcoords[texcoord[i]];	}

inline bool TriangleFace::getVertex(const size_t i, vector3& v)
{
	if(i >= 3) return false;
	v = mesh->vertices[vertex[i]];
	return true;
}

inline bool TriangleFace::getNormal(const size_t i, vector3& v)
{
	if(i >= 3) return false;
	v = mesh->normals[normal[i]];
	return true;
}

inline bool TriangleFace::getTexCoord(const size_t i, vector2& v)
{
	if(i >= 3) return false;
	v = mesh->texcoords[texcoord[i]];
	return true;
}

inline bool TriangleFace::setVertex(const size_t i, const vector3& v)
{
	if(i >= 3) return false;
	mesh->vertices[vertex[i]] = v;
	return true;
}

inline bool TriangleFace::setNormal(const size_t i, const vector3& v)
{
	if(i >= 3) return false;
	mesh->normals[normal[i]] = v;
	return true;
}

inline bool TriangleFace::setTexCoord(const size_t i, const vector2& v)
{
	if(i >= 3) return false;
	mesh->texcoords[texcoord[i]] = v;
	return true;
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

inline bool SubMesh::getVertex(const size_t i, vector3& v)
{
	if(i >= vertices.size())
		return false;
	v = vertices[i];
	return true;
}

inline bool SubMesh::setVertex(const size_t i, const vector3& v)
{
	if(i >= vertices.size())
		return false;
	vertices[i] = v;
	return true;
}

inline bool SubMesh::getNormal(const size_t i, vector3& v)
{
	if(i >= normals.size())
		return false;
	v = normals[i];
	return true;
}

inline bool SubMesh::setNormal(const size_t i, const vector3& v)
{
	if(i >= normals.size())
		return false;
	normals[i] = v;
	return true;
}

inline bool SubMesh::getTexCoord(const size_t i, vector2& v)
{
	if(i >= texcoords.size())
		return false;
	v = texcoords[i];
	return true;
}

inline bool SubMesh::setTexCoord(const size_t i, const vector2& v)
{
	if(i >= texcoords.size())
		return false;
	texcoords[i] = v;
	return true;
}

inline bool SubMesh::getFace(const size_t i, TriangleFace& face)
{
	if(i >= faces.size())
		return false;
	face = faces[i];
	return true;
}

inline bool SubMesh::setFace(const size_t i, const TriangleFace& face)
{
	if(i >= faces.size())
		return false;
	faces[i] = face;
	faces[i].setMesh(this);
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