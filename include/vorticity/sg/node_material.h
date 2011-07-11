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

#ifndef __VORTICITY_NODE_MATERIAL_H
#define __VORTICITY_NODE_MATERIAL_H

namespace Vorticity {

class Material;
class VAPI NodeMaterial : public Modifier
{
public:
	NodeMaterial(const string& name, XNode *parent=NULL);
	virtual ~NodeMaterial();

	virtual string getType() const
	{ return "material"; }

	virtual void setSampler(const string& name, const int texture);
	virtual int	 getSampler(const string& name) const;
	virtual void setupShader(const int shader, Renderer *device);

	virtual void evaluate();
	virtual bool bind(Resource *res);
protected:
	std::map<string, int> samplers;
};

} // Vorticity

#endif