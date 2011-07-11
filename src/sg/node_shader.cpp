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
#include "vorticity/resources/resource.h"
#include "vorticity/resources/shader.h"

#include "vorticity/sg/attribute.h"
#include "vorticity/sg/node.h"
#include "vorticity/sg/xnode.h"
#include "vorticity/sg/modifier.h"
#include "vorticity/sg/node_shader.h"

using namespace Vorticity;

NodeShader::NodeShader(const string& name, XNode* parent) : Modifier(name, parent)
{
	shader = NULL;
}

NodeShader::~NodeShader()
{
}

bool NodeShader::bind(Resource *shader)
{
	if(shader->getType() != "shader")
		return false;
	if(!shader->getDevice())
		return false;
	if(!shader->getDevice()->exportShaderParameters(shader->getID(), this))
		return false;
	this->shader = (Shader*)shader;
	return true;
}

void NodeShader::evaluate()
{
	if(!shader) return;
	if(!shader->getDevice()) return;

	AttributeIterator it = enumAttributes(0);
	while(AnyAttribute* attr = getAttrib(it))
		shader->getDevice()->setShaderParameter(shader->getID(), attr);
}
