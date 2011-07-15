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
#include "vorticity/sg/node_light.h"

using namespace Vorticity;

NodeLight::NodeLight(const string& name, XNode *parent) : Object(name, parent)
{
	addAttrib(new Attribute<vec3>("ambient"));
	addAttrib(new Attribute<vec3>("diffuse")) = vec3(0.5f, 0.5f, 0.5f);
	addAttrib(new Attribute<vec3>("specular"));
}

NodeLight::~NodeLight()
{
}

void NodeLight::evaluate()
{
	vec3 localPosition = getAttrib("position");
	vec3 localRotation = getAttrib("rotation");

	vec4 outPosition = getWorldTM() * vector4(localPosition, 1.0f);
	vec4 outRotation = getWorldInvTranspose() * vector4(localRotation, 1.0f);

	worldPosition = vec3(outPosition.x, outPosition.y, outPosition.z);
	worldRotation = vec3(outRotation.x, outRotation.y, outRotation.z);
}

void NodeLight::setupShader(const int shader, Renderer *device, const int index)
{
        //INFO: This cast of last parameter to Attribute constructor is to make the cast for exact type
        // that Attribute constructor expects i.e. const T&
        device->setShaderParameter(shader,
                &Attribute<vec3>("LightAmbient", static_cast<const vec3&>(getAttrib("ambient"))), index);
	device->setShaderParameter(shader,
                &Attribute<vec3>("LightDiffuse", static_cast<const vec3&> (getAttrib("diffuse"))), index);
	device->setShaderParameter(shader,
                &Attribute<vec3>("LightSpecular", static_cast<const vec3&> (getAttrib("specular"))), index);
	device->setShaderParameter(shader,
		&Attribute<vec3>("LightPosition", worldPosition), index);
}
