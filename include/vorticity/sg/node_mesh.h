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

#ifndef __VORTICITY_NODE_MESH_H
#define __VORTICITY_NODE_MESH_H

namespace Vorticity {

class Mesh;
class SubMesh;

class VAPI NodeMesh : public Object
{
protected:
	Mesh *mesh;
public:
	NodeMesh(const string& name, XNode *parent);
	virtual ~NodeMesh();

	virtual string getType() const
	{ return "mesh"; }

	virtual bool bind(Resource *res);
	virtual void render(Renderer *device);
};

class VAPI NodeSubMesh : public NodeMesh
{
protected:
	SubMesh *subMesh;
public:
	NodeSubMesh(const string& name, NodeMesh *parent);
	virtual ~NodeSubMesh();

	virtual string getType() const
	{ return "submesh"; }

	virtual bool bind(Resource *res);
	virtual void render(Renderer *device);
};

} // Vorticity

#endif