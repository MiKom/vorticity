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