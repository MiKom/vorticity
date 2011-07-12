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
#include "vorticity/sg/node_camera.h"
#include "vorticity/core/renderer.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/backdrop.h"

using namespace Vorticity;

NodeCamera::NodeCamera(const string &name, XNode *parent) : Object(name, parent)
{
	target = NULL;
	backdrop = NULL;
	useProjectionMatrix = false;

	addAttrib(new Attribute<float>("fov")).setValue(45.0f);
	addAttrib(new Attribute<float>("znear")).setValue(1.0f);
	addAttrib(new Attribute<float>("zfar")).setValue(100.0f);
	addAttrib(new Attribute<matrix4>("projection")).setValue(matrix4().identity());

	evaluate();
}

NodeCamera::~NodeCamera()
{
	delete backdrop;
	delete target;
}

void NodeCamera::evaluate()
{
	getAttrib("fov").getValue(fov);
	getAttrib("znear").getValue(zNear);
	getAttrib("zfar").getValue(zFar);
	if(useProjectionMatrix)
		getAttrib("projection").getValue(customProjection);
}

bool NodeCamera::bind(Resource *resource)
{
	if(resource->getType() != "render_asset")
		return false;
	target = (RenderAsset*)resource;
	return true;
}

bool NodeCamera::setupProjection() const
{
	if(!target)
		return false;

	int w,h;
	Renderer *device = target->getDevice();

	if(useProjectionMatrix)
		device->setProjection(customProjection.cell);
	else
	{
		if(!device->getFramebufferProperties(target->getID(), &w, &h, NULL))
			return false;
		device->setPerspectiveProjection(fov, ((float)w)/((float)h), zNear, zFar);
	}
	device->projectionTransform(worldTM.getInverted().cell);
	return true;
}

bool NodeCamera::beginRendering(ClearMode cmode) const
{
	if(!target) return false;
	if(!target->getDevice()->bindFramebuffer(target->getID()))
		return false;
	target->getDevice()->clear(cmode);
	if(backdrop)
		return backdrop->draw();
	return true;
}

bool NodeCamera::endRendering() const
{
	if(!target) return false;
	target->getDevice()->commitFramebuffer(target->getID());
	target->getDevice()->generateBufferMipmaps(target->getID());
	return true;
}
