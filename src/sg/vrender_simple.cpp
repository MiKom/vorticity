/* 
 * Copyright © 2009, 2010 Micha³ Siejak
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
#include "vorticity/core/renderer.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/shader.h"

#include "vorticity/sg/attribute.h"
#include "vorticity/sg/node.h"
#include "vorticity/sg/xnode.h"
#include "vorticity/sg/object.h"
#include "vorticity/sg/modifier.h"
#include "vorticity/sg/node_camera.h"
#include "vorticity/sg/vrender_simple.h"

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

