/* 
 * Copyright © 2010 Michał Siejak
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
#include "vorticity/sg/attribute.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/shader.h"
#include "vorticity/resources/backdrop.h"

using namespace Vorticity;

Backdrop::Backdrop(const int target, Shader* shader) 
	: Resource(), shader(shader)
{
	target_w = target_h = 0;
	setID(target);
	setTarget(target);
}

void Backdrop::setTarget(const int target)
{
	this->target = target;
	device->getTextureProperties(target, &target_w, &target_h);
}

bool Backdrop::draw()
{
	if(!device->bindTexture(target, 0))
		return false;

	float savedProjection[16];
	std::memcpy(savedProjection, device->getProjection(), 16*sizeof(float));
	device->setOrthoProjection(-1.0f, 1.0f, -1.0f, 1.0f);

	if(shader)
	{
		int shaderID = shader->getID();
		device->setShaderMatrix(shaderID, ModelviewMatrix, "V_MODELVIEW");
		device->setShaderMatrix(shaderID, ProjectionMatrix, "V_PROJECTION");
		device->setShaderMatrix(shaderID, ModelviewProjMatrix, "V_MVP");
		device->setShaderMatrix(shaderID, NormalMatrix, "V_MVIT");

		device->setShaderParameterBySemantic(shaderID, &Attribute<int>("V_WIDTH", target_w));
		device->setShaderParameterBySemantic(shaderID, &Attribute<int>("V_HEIGHT", target_h));
		device->setShaderSamplerBySemantic(shaderID, "V_COLOR0", target);

		ShaderPass pass = device->firstShaderPass(shader->getID());
		do {
			device->drawQuad(-1.0f, 1.0f, 1.0f, -1.0f);
		} while(pass = device->nextShaderPass(shader->getID(), pass));
	}
	else
		device->drawQuad(-1.0f, 1.0f, 1.0f, -1.0f);

	device->projectionTransform(savedProjection);
	device->bindTexture(0, 0);
	return true;
}
