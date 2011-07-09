#include "config.h"
#include "resource.h"
#include "material.h"
#include "image.h"
#include "core/renderer.h"
#include "io/iobase.h"
#include "mesh.h"
#include "submesh.h"

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
