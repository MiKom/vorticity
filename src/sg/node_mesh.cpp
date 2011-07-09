#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "object.h"
#include "modifier.h"
#include "node_mesh.h"
#include "node_material.h"
#include "resources/resource.h"
#include "resources/material.h"
#include "resources/mesh.h"
#include "resources/submesh.h"
#include "core/renderer.h"

using namespace Vorticity;

NodeMesh::NodeMesh(const string &name, XNode *parent) : Object(name, parent)
{
	mesh = NULL;
}

NodeMesh::~NodeMesh()
{
}

bool NodeMesh::bind(Resource *res)
{
	if(mesh != NULL)
		return false;
	if(res->getType() != "mesh")
		return false;

	mesh = (Mesh*)res;
	std::map<std::string, NodeMaterial*> matcache;
	for(size_t i=0; i<mesh->getMaterialCount(); i++)
	{
		Material* material = mesh->getMaterial(i);
		NodeMaterial *node_mat = new NodeMaterial(material->getName(), this);
		if(node_mat->bind(material))
			matcache[material->getName()] = node_mat;
	}
	for(size_t i=0; i<mesh->getSubMeshCount(); i++)
	{
		SubMesh *subMesh = mesh->getSubMesh(i);
		NodeSubMesh *node_subMesh = new NodeSubMesh(subMesh->getName(), this);
		if(!node_subMesh->bind(subMesh))
		{
			delete node_subMesh;
			return false;
		}

		if(!subMesh->getMaterial())
			continue;
		auto mat_subMesh = matcache.find(subMesh->getMaterial()->getName());
		if(mat_subMesh != matcache.end())
			mat_subMesh->second->connectTo(node_subMesh);
	}
	return true;
}

void NodeMesh::render(Renderer *device)
{ }

NodeSubMesh::NodeSubMesh(const string& name, NodeMesh *parent) : NodeMesh(name, (XNode*)parent)
{
	subMesh = NULL;
}

NodeSubMesh::~NodeSubMesh()
{ }

bool NodeSubMesh::bind(Resource *res)
{
	if(res->getType() != "submesh")
		return false;
	if(!res->getDevice())
		return false;

	if(!((SubMesh*)res)->hasRenderBuffers())
	{
		if(!((SubMesh*)res)->createRenderBuffers())
			return false;
	}

	subMesh = (SubMesh*)res;
	return true;
}

void NodeSubMesh::render(Renderer *device)
{
	if(!subMesh) 
		return;
	if(subMesh->getID() == 0)
		return;
	
	device->drawVertexBuffer(subMesh->getID(), 0, subMesh->getRenderElementCount());
}