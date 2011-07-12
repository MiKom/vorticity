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
#include "vorticity/sg/modifier.h"
#include "vorticity/sg/node_material.h"

#include "vorticity/core/renderer.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/material.h"
#include "vorticity/resources/image.h"

using namespace Vorticity;

NodeMaterial::NodeMaterial(const string& name, XNode *parent) : Modifier(name, parent)
{
	addAttrib(new Attribute<vector3>("diffuse"))	= vec3(0.5f, 0.5f, 0.5f);
	addAttrib(new Attribute<float>("glossiness"))	= 1.0f;
	addAttrib(new Attribute<float>("opacity"))		= 1.0f;

	addAttrib(new Attribute<float>("specular_level"));
	addAttrib(new Attribute<vector3>("specular"));
	addAttrib(new Attribute<vector3>("ambient"));
	addAttrib(new Attribute<vector3>("emissive"));
}

NodeMaterial::~NodeMaterial()
{
}

void NodeMaterial::evaluate()
{ }

bool NodeMaterial::bind(Resource *res)
{
	if(res->getType() != "material")
		return false;
	Material *material = (Material*)res;

	getAttrib("diffuse")		= material->getDiffuse();
	getAttrib("specular")		= material->getSpecular();
	getAttrib("ambient")		= material->getAmbient();
	getAttrib("emissive")		= material->getEmissive();
	getAttrib("glossiness")		= material->getGlossiness();
	getAttrib("opacity")		= material->getOpacity();
	getAttrib("specular_level") = material->getSpecularLevel();

	Image* textureImage;
	size_t textureIndex = 0;
	char samplerName[256];
	while(textureImage = material->getTextureChannel(textureIndex))
	{
		sprintf_s<256>(samplerName, "Sampler%d", textureIndex++);
		setSampler(samplerName, textureImage->getID());
	}
	return true;
}

void NodeMaterial::setSampler(const string& name, const int texture)
{
	if(texture == 0)
	{
		auto location = samplers.find(name);
		if(location != samplers.end())
			samplers.erase(location);
	}
	else
		samplers[name] = texture;
}

int NodeMaterial::getSampler(const string& name) const
{
	auto location = samplers.find(name);
	if(location == samplers.end())
		return 0;
	return location->second;
}

void NodeMaterial::setupShader(const int shader, Renderer *device)
{
	device->setShaderParameter(shader,
		&Attribute<vector3>("MaterialDiffuse", getAttrib("diffuse")));
	device->setShaderParameter(shader,
		&Attribute<vector3>("MaterialAmbient", getAttrib("ambient")));
	device->setShaderParameter(shader,
		&Attribute<vector3>("MaterialSpecular", getAttrib("specular")));
	device->setShaderParameter(shader,
		&Attribute<float>("MaterialGloss", getAttrib("glossiness")));
	device->setShaderParameter(shader,
		&Attribute<float>("MaterialSpecLevel", getAttrib("specular_level")));
	device->setShaderParameter(shader,
		&Attribute<vector3>("MaterialEmissive", getAttrib("emissive")));

	device->setShaderParameterBySemantic(shader, &Attribute<int>("V_MAXSAMPLERS", (int)samplers.size()));
	for(auto i=samplers.begin(); i != samplers.end(); i++)
		device->setShaderSampler(shader, i->first, i->second);
}