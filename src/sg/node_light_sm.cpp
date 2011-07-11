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
#include "vorticity/sg/attribute.h"
#include "vorticity/sg/node.h"
#include "vorticity/sg/xnode.h"
#include "vorticity/sg/object.h"
#include "vorticity/sg/node_light.h"
#include "vorticity/sg/node_light_sm.h"
#include "vorticity/sg/node_camera.h"
#include "vorticity/resources/resource.h"

using namespace Vorticity;

NodeLightSM::NodeLightSM(const string& name, XNode *parent) : NodeLight(name, parent)
{
	fbo = 0;
}

NodeLightSM::~NodeLightSM()
{
	if(fbo)
		getCore()->getDevice()->destroyFramebuffer(fbo);
}

bool NodeLightSM::createShadowmap(const int w, const int h,
								  const float fov, const float zfar)
{
	Renderer *dev = getCore()->getDevice();

	if(fbo && !dev->destroyFramebuffer(fbo))
		return false;

	fbo = dev->createFramebuffer(Vorticity::BufferRGBA, w, h, 0);
	dev->addBufferTarget(fbo, Vorticity::ColorBuffer, NoFilter);
	dev->addBufferTarget(fbo, Vorticity::DepthBuffer, NoFilter);

	string cam_name = "__cam_";
	cam_name.append(name);

	if(shadowCamera)
		delete shadowCamera;
	shadowCamera = new NodeCamera(cam_name, this);
	shadowCamera->bind(new RenderAsset(fbo));
	shadowCamera->getAttrib("fov").setValue(fov);
	shadowCamera->getAttrib("znear").setValue(1.0f);
	shadowCamera->getAttrib("zfar").setValue(zfar);

	return true;
}

void NodeLightSM::setupShader(const int shader, Renderer* device, const int index)
{
	NodeLight::setupShader(shader, device, index);
}

void NodeLightSM::render(Renderer* device)
{
	
}