#ifndef __VORTICITY_VRENDERSIMPLE_H
#define __VORTICITY_VRENDERSIMPLE_H

namespace Vorticity {

class Renderer;
class Node;
class NodeCamera;
class NodeShader;

class VAPI SimpleRenderVisitor : public AbstractVisitor
{
protected:
	Renderer *device;
	Shader *shader;
	NodeCamera *camera;
protected:
	void renderSubtree(Node *root);
public:
	SimpleRenderVisitor(NodeCamera *cam, Shader *shader);
	virtual void traverse(Node *root);

	NodeCamera* getCamera() const
	{ return camera; }
	Shader* getShader() const
	{ return shader; }
};

} // Vorticity

#endif