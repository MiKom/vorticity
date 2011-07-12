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

#ifndef __VORTICITY_NODE_LIGHT_H
#define __VORTICITY_NODE_LIGHT_H

namespace Vorticity {

class VAPI NodeLight : public Object
{
protected:
	vector3	worldPosition;
	vector3	worldRotation;
public:
	NodeLight(const string& name, XNode *parent=NULL);
	virtual ~NodeLight();

	virtual string getType() const
	{ return "light"; }
	virtual string getShadowType() const
	{ return "none"; }

	virtual void evaluate();
	virtual void setupShader(const int shader, Renderer *device, const int index);
};

} // Vorticity

#endif