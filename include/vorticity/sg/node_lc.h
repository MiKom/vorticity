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

#ifndef __VORTICITY_NODE_LC_H
#define __VORTICITY_NODE_LC_H

namespace Vorticity {

template <class T>
class NodeLC : public KeyframeController<T>
{
public:
	NodeLC(const string& name, XNode *parent=NULL) : KeyframeController<T>(name, parent)
	{ }
	virtual ~NodeLC()
	{ }

	virtual string getType() const
	{ return "lc"; }
	virtual bool isTypeCompatible(const string& type) const
	{ return type == Attribute<T>("").getType(); }

	virtual bool getStatus() const
	{	
		ArrayAttribute &keys = (ArrayAttribute&)this->getAttrib("keys");
		ArrayAttribute &values = (ArrayAttribute&)this->getAttrib("values");

		size_t ikeys	= keys.getCount();
		size_t ivalues	= values.getCount();

		if(ikeys == ivalues && ikeys >= 2)
			return true;
		return false;
	}

	virtual void evaluate()
	{
		int index;
		float time[2];
		T value[2];

		//if(!getStatus()) return;

		float frameTime = this->getTime();
		if(!getKey(frameTime, &index, &time[0], &value[0]))
			return;

		ArrayAttribute& keys = (ArrayAttribute&)this->getAttrib("keys");
		if((size_t)index >= keys.getCount()-1)
		{
			Node::writeOutput<T>(value[0]);
			return;
		}
		
		getKey(index+1, &time[1], &value[1]);
		// Calculate t value
		float t = (time[1] - frameTime) / (time[1] - time[0]);
		// Linear interpolation between value[0] and value[1]
		T out_value = Math::lerp(t, value[0], value[1]);
		Node::writeOutput<T>(out_value);
	}
};

typedef NodeLC<float>	NodeLC1;
typedef NodeLC<vec2>	NodeLC2;
typedef NodeLC<vec3>	NodeLC3;
typedef NodeLC<vec4>	NodeLC4;

} // Vorticity

#endif
