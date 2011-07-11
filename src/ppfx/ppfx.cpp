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
#include "vorticity/sg/node_camera.h"
#include "vorticity/sg/node_ppfx.h"
#include "vorticity/ppfx/ppfx.h"

using namespace Vorticity;

PostProcess::PostProcess(Renderer *device) : Entity(), node(NULL)
{ 
	if(!device)
		this->device = getCore()->getDevice();
	else
		this->device = device;
}

PostProcess::~PostProcess()
{ }

bool PostProcess::addSource(const string &name, NodeCamera *cam)
{
	if(!acceptSource(name))
		return false;
	if(getSource(name))
		return false;
	sources[name] = cam;
	return true;
}

bool PostProcess::removeSource(const string &name)
{
	FrameSourceMap::const_iterator it = sources.find(name);
	if(it == sources.end())
		return false;
	sources.erase(it);
	return true;
}

NodeCamera* PostProcess::getSource(const string &name) const
{
	FrameSourceMap::const_iterator it = sources.find(name);
	if(it == sources.end())
		return NULL;
	return it->second;
}

bool PostProcess::addShader(const string& name, Shader *shader)
{
	if(!acceptShader(name))
		return false;
	if(getShader(name))
		return false;
	shaders[name] = shader;
	return true;
}

bool PostProcess::removeShader(const string& name)
{
	FrameShaderMap::const_iterator it = shaders.find(name);
	if(it == shaders.end())
		return false;
	shaders.erase(it);
	return true;
}

Shader* PostProcess::getShader(const string& name) const
{
	FrameShaderMap::const_iterator it = shaders.find(name);
	if(it == shaders.end())
		return NULL;
	return it->second;
}

NodePostProcess& PostProcess::createNode(const string& name, XNode *parent)
{
	NodePostProcess& ppnode = gnew<NodePostProcess>(name, parent);
	FrameShaderMap::const_iterator it = shaders.begin();
	for(;it!=shaders.end(); it++)
	{
		int id = it->second->getID();
		if(id == 0)
			continue;
		device->exportShaderParameters(id, &ppnode);
	}
	node = &ppnode;
	return ppnode;
}

NodeCamera* PostProcess::getCheckSource(const string& name) const
{
	NodeCamera *cam = getSource(name);
	if(!cam) return NULL;
	if(!cam->getTargetBuffer())
		return NULL;
	if(cam->getTargetBuffer()->getID() == 0)
		return NULL;
	return cam;
}

Shader* PostProcess::getCheckShader(const string& name) const
{
	Shader *shader = getShader(name);
	if(!shader)
		return NULL;
	if(shader->getID() == 0)
		return NULL;
	return shader;
}

void PostProcess::setShaderParameters(Shader *shader)
{
	int sh_id = shader->getID();
	device->modelTransform(NULL);

	if(node)
	{
		AttributeIterator it = node->enumAttributes(0);
		while(AnyAttribute *attr = node->getAttrib(it))
			device->setShaderParameter(sh_id, attr);
	}

	device->setShaderMatrix(sh_id, ModelviewMatrix, "V_MODELVIEW");
	device->setShaderMatrix(sh_id, ProjectionMatrix, "V_PROJECTION");
	device->setShaderMatrix(sh_id, ModelviewProjMatrix, "V_MVP");
}

bool PostProcess::validateBuffer(int &buffer, FramebufferFormat format, const NodeCamera *src)
{
	int w,h;
	if(buffer) device->destroyFeedbackBuffer(buffer);
	device->getFramebufferProperties(src->getTargetBuffer()->getID(), &w, &h, NULL);
	buffer = device->createFeedbackBuffer(format, w, h, Bilinear);
	if(!buffer)
		return false;
	return true;
}

bool PostProcess::validateBuffer(int &buffer, FramebufferFormat format, const int w, const int h)
{
	if(buffer) device->destroyFeedbackBuffer(buffer);
	buffer = device->createFeedbackBuffer(format, w, h, Bilinear);
	if(!buffer)
		return false;
	return true;
}

int PostProcess::beginBuffer(const int buffer, const int shader)
{
	int w,h;
	device->getFeedbackBufferProperties(buffer, &w, &h, NULL);
	device->setShaderParameterBySemantic(shader,
		&Attribute<int>("V_WIDTH", w));
	device->setShaderParameterBySemantic(shader,
		&Attribute<int>("V_HEIGHT", h));
	return device->bindFeedbackBuffer(buffer);
}

int PostProcess::swapBuffer(const int buffer, const int shader)
{
	int w,h;
	device->getFeedbackBufferProperties(buffer, &w, &h, NULL);
	device->setShaderParameterBySemantic(shader,
		&Attribute<int>("V_WIDTH", w));
	device->setShaderParameterBySemantic(shader,
		&Attribute<int>("V_HEIGHT", h));
	return device->swapFeedbackBuffer(buffer);
}

int PostProcess::swapBuffer(const int buffer)
{ return device->swapFeedbackBuffer(buffer); }

void PostProcess::endBuffer()
{ device->bindFeedbackBuffer(0); }

bool PostProcess::setSampler(const int shader, const string &name, const int texture)
{ return device->setShaderSamplerBySemantic(shader, name, texture); }