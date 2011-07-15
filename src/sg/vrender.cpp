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
#include "vorticity/core/renderer.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/shader.h"
#include "vorticity/resources/material.h"

#include "vorticity/sg/attribute.h"
#include "vorticity/sg/node.h"
#include "vorticity/sg/xnode.h"
#include "vorticity/sg/object.h"
#include "vorticity/sg/modifier.h"
#include "vorticity/sg/proxy.h"
#include "vorticity/sg/node_shader.h"
#include "vorticity/sg/node_camera.h"
#include "vorticity/sg/node_material.h"
#include "vorticity/sg/vrender.h"

using namespace Vorticity;

const string RenderVisitor::NullGroupName = "null";

ModifierCache::ModifierCache()
{ }

bool ModifierCache::isModifier(const string& type) const
{
	for(std::vector<Modifier*>::const_iterator it=modifiers.begin();
		it != modifiers.end(); it++)
		if((*it)->getType() == type)
			return true;
	return false;
}

Modifier* ModifierCache::getModifier(const string& type) const
{
	for(std::vector<Modifier*>::const_iterator it=modifiers.begin();
		it != modifiers.end(); it++)
		if((*it)->getType() == type)
			return (*it);
	return NULL;
}

Modifier* ModifierCache::updateModifier(const string& type, XNode *node)
{
	size_t i=0;
	Modifier *mod = node->getModifier(type);

	for(std::vector<Modifier*>::iterator it=modifiers.begin();
		it != modifiers.end(); it++)
	{
		if((*it)->getType() == type)
		{
			if(mod)
			{
				modifiers.at(i) = mod;
				return mod;
			}
			else
				return (*it);
		}
		i++;
	}
	if(mod)
	{
		modifiers.push_back(mod);
		return mod;
	}
	return NULL;
}

RenderVisitor::RenderVisitor(NodeCamera* cam) : AbstractVisitor()
{
	cached		 = false;
	this->camera = cam;
	if(!cam->getTargetBuffer())
		this->device = NULL;
	else
		this->device = cam->getTargetBuffer()->getDevice();
}

void RenderVisitor::createNodeCache(XNode *node, ModifierCache &cache)
{
	cache.updateModifier("material", node);
	cache.updateModifier("proxy", node);
	cache.setNode(node);
}

// WARNING: See adnotation in header file.
void RenderVisitor::createRenderGroups(XNode *node, ModifierCache cache)
{
	if(!node->isEnabled())
		return;

	if(!node->isHidden())
	{
		string gname(NullGroupName);
		Modifier* modShader = cache.updateModifier("shader", node);
		if(modShader)
			gname = modShader->getName();
		
		std::map<string, ModifierCacheArray>::iterator it = group.find(gname);
		createNodeCache(node, cache);

		if(it == group.end())
		{
			ModifierCacheArray	modarray;
			modarray.push_back(cache);
			group[gname] = modarray;
			shaders.push_back(modShader);
		}
		else
			it->second.push_back(cache);
	}

        NodeConstIterator node_it = node->enumChildren(0);
	while(Node* n = node->getChild(node_it))
	{
		if(n->isExtended())
		{
			cache.setNode((XNode*)n);
			createRenderGroups((XNode*)n, cache);
		}
	}
}

void RenderVisitor::setupShaderParameters(const ModifierCache &cache, const int shader)
{
	XNode *node = cache.getNode();
	matrix4	nodeTM(true);
	if(node->getClass() == "object")
		nodeTM = ((Object*)node)->getWorldTM();
	
	const matrix4* cameraMatrix = &camera->getWorldTM();
	Attribute<vector3> eyePos("V_EYEPOS");
	eyePos.setValue(vector3(cameraMatrix->cell[12], cameraMatrix->cell[13], cameraMatrix->cell[14]));

	device->modelTransform(nodeTM.cell);
	device->setShaderMatrix(shader, ModelviewMatrix, "V_MODELVIEW");
	device->setShaderMatrix(shader, ProjectionMatrix, "V_PROJECTION");
	device->setShaderMatrix(shader, ModelviewProjMatrix, "V_MVP");
	device->setShaderMatrix(shader, NormalMatrix, "V_MVIT");
	device->setShaderParameterBySemantic(shader, &eyePos);

	Node *material = cache.getModifier("material");
	if(material)
		material->setupShader(shader, device);
	Proxy *proxy = (Proxy*)cache.getModifier("proxy");
	if(proxy && proxy->isType("light"))
	{
		Attribute<int> maxLights("V_MAXLIGHTS");
		int lightIndex = 0;

		ProxyNodeIterator lightNodes = proxy->enumType("light");
		while(Node *light = proxy->getNode(lightNodes))
			light->setupShader(shader, device, lightIndex++);
		maxLights = lightIndex;
		device->setShaderParameterBySemantic(shader, &maxLights);
	}
}

void RenderVisitor::traverseRenderGroups()
{
	if(group.size() == 0) return;
	Shader *currentShader;
	ShaderPass currentPass;

	size_t shader_index = 0;
	for(std::map<string, ModifierCacheArray>::const_iterator it=group.begin();
		it != group.end(); it++)
	{
		NodeShader *nodeShader = (NodeShader*)shaders.at(shader_index++);	
		if(nodeShader)
			currentShader = nodeShader->getShader();
		else continue; // Don't cha render without da shader!

		int shader = currentShader->getID();
		currentPass = device->firstShaderPass(shader, false);
		do
		{
			for(ModifierCacheArray::const_iterator mod_it = it->second.begin();
				mod_it < it->second.end(); mod_it++)
			{
				XNode* node = (*mod_it).getNode();
				setupShaderParameters((*mod_it), shader);
				device->setShaderPass(currentPass);
				node->render(device);
			}
		} while(currentPass = device->nextShaderPass(currentPass, false));
	}
}

void RenderVisitor::traverse(Node *root)
{
	if(!device) return;

	if(!cached)
	{
		group.clear();
		shaders.clear();
		createRenderGroups((XNode*)root, ModifierCache());
		cached = true;
	}

	if(!camera->beginRendering(ClearAll))
		return;
	camera->setupProjection();
	traverseRenderGroups();
	camera->endRendering();
}
