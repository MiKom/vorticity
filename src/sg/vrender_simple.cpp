#include "config.h"
#include "core/renderer.h"
#include "resources/resource.h"
#include "resources/shader.h"

#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "object.h"
#include "modifier.h"
#include "node_camera.h"
#include "vrender_simple.h"

using namespace Vorticity;

SimpleRenderVisitor::SimpleRenderVisitor(NodeCamera *cam, Shader *shader) : AbstractVisitor()
{
	this->camera = cam;
	this->shader = shader;

	if(!cam->getTargetBuffer())
		this->device = NULL;
	else
		this->device = cam->getTargetBuffer()->getDevice();
}

void SimpleRenderVisitor::renderSubtree(Node *root)
{
	if(!root->isEnabled())
		return;

	if(!root->isHidden())
	{
		const int shaderID = shader->getID();

		matrix4 nodeTM(true);
		if(root->getClass() == "object")
			nodeTM = ((Object*)root)->getWorldTM();

		device->modelTransform(nodeTM.cell);
		device->setShaderMatrix(shaderID, ModelviewMatrix, "V_MODELVIEW");
		device->setShaderMatrix(shaderID, ModelviewProjMatrix, "V_MVP");
		device->setShaderMatrix(shaderID, NormalMatrix, "V_MVIT");

		root->render(device);
	}
	if(root->hasChildren())
	{
		NodeIterator it = ((XNode*)root)->enumChildren(0);
		while(Node* n = ((XNode*)root)->getChild(it))
			renderSubtree(n);
	}
}

void SimpleRenderVisitor::traverse(Node *root)
{
	if(!device) return;
	if(!camera->beginRendering(ClearAll))
		return;

	const int shaderID = shader->getID();
	const matrix4* cameraMatrix = &camera->getWorldTM();
	Attribute<vector3> eyePos("V_EYEPOS");
	eyePos.setValue(vector3(cameraMatrix->cell[12], cameraMatrix->cell[13], cameraMatrix->cell[14]));

	camera->setupProjection();
	device->setShaderMatrix(shaderID, ProjectionMatrix, "V_PROJECTION");
	device->setShaderParameterBySemantic(shaderID, &eyePos);

	ShaderPass currentPass = device->firstShaderPass(shaderID);
	do { renderSubtree(root); }
	while(currentPass = device->nextShaderPass(currentPass));
	camera->endRendering();
}

