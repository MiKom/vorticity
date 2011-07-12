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
#include "vorticity/sg/attribute.h"
#include "vorticity/sg/node.h"
#include "vorticity/sg/xnode.h"
#include "vorticity/sg/object.h"
#include "vorticity/sg/node_ppfx.h"
#include "vorticity/sg/node_camera.h"
#include "vorticity/ppfx/ppfx.h"
#include "vorticity/ppfx/ppfx_simple.h"

using namespace Vorticity;

PostProcessSimple::PostProcessSimple(Renderer *device) : PostProcess(device), fb(0)
{ }

PostProcessSimple::~PostProcessSimple()
{
	if(fb && device)
		device->destroyFeedbackBuffer(fb);
}

bool PostProcessSimple::acceptShader(const string& name)
{ return name == "blit"; }

bool PostProcessSimple::acceptSource(const string& name)
{ return name == "color"; }

int PostProcessSimple::render()
{
	int color = srcColor->getTargetBuffer()->getID();
	int shader = blitShader->getID();
	int pass   = 0;

	device->setOrthoProjection(0.0f, 1.0f, 0.0f, 1.0f);
	setShaderParameters(blitShader);

	// First pass
	setSampler(shader, "V_COLOR0", device->getBufferTarget(color, ColorBuffer));
	ShaderPass p = device->firstShaderPass(shader);
	beginBuffer(fb, shader);
	do
	{
		// Next Pass
		if(pass++ > 0) swapBuffer(fb);
		device->drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
		setSampler(shader, "V_COLOR0", device->getFeedbackBufferTarget(fb));
	} while(p = device->nextShaderPass(p));
	endBuffer();
	return fb;
}

bool PostProcessSimple::validate()
{
	srcColor = getCheckSource("color");
	blitShader = getCheckShader("blit");

	if(!srcColor || !blitShader)
		return false;

	return validateBuffer(fb, BufferRGBA, srcColor);
}