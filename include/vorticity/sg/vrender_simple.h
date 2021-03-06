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

#ifndef __VORTICITY_VRENDERSIMPLE_H
#define __VORTICITY_VRENDERSIMPLE_H

namespace Vorticity {

class Renderer;
class Node;
class NodeCamera;
class NodeShader;

class VAPI SimpleRenderVisitor : public AbstractVisitor
{
protected:
	Renderer *device;
	Shader *shader;
	NodeCamera *camera;
protected:
	void renderSubtree(Node *root);
public:
	SimpleRenderVisitor(NodeCamera *cam, Shader *shader);
	virtual void traverse(Node *root);

	NodeCamera* getCamera() const
	{ return camera; }
	Shader* getShader() const
	{ return shader; }
};

} // Vorticity

#endif