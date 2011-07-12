/* 
 * Copyright © 2009, 2010 Michał Siejak
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
#include "vorticity/sg/attribute.h"
#include "vorticity/sg/node.h"
#include "vorticity/sg/xnode.h"
#include "vorticity/sg/object.h"
#include "vorticity/sg/modifier.h"
#include "vorticity/sg/node_mesh.h"
#include "vorticity/sg/node_material.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/material.h"
#include "vorticity/resources/mesh.h"
#include "vorticity/resources/submesh.h"
#include "vorticity/core/renderer.h"

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