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

#ifndef __VORTICITY_NODE_SC_H
#define __VORTICITY_NODE_SC_H

namespace Vorticity {

template <class T>
class NodeSC : public KeyframeController<T>
{
public:
	NodeSC(const string& name, XNode *parent=NULL) : KeyframeController(name, parent)
	{ }
	virtual ~NodeSC()
	{ }

	virtual string getType() const
	{ return "sc"; }
	virtual bool isTypeCompatible(const string& type) const
	{ return type == Attribute<T>("").getType(); }

	virtual bool getStatus() const
	{	
		ArrayAttribute &keys = (ArrayAttribute&)getAttrib("keys");
		ArrayAttribute &values = (ArrayAttribute&)getAttrib("values");

		size_t ikeys	= keys.getCount();
		size_t ivalues	= values.getCount();

		if(ikeys == ivalues && ikeys >= 4)
			return true;
		return false;
	}

	virtual void evaluate()
	{
		int index;
		float time[4];
		T value[4];

		//if(!getStatus()) return;
		float frameTime = getTime();
		int count = getKeyCount();
		if(!getKey(frameTime, &index, &time[1], &value[1]))
			return;
		if(index >= count-1)
		{
			writeOutput<T>(value[1]);
			return;
		}

		getKey(index-1, &time[0], &value[0]);
		getKey(index+1, &time[2], &value[2]);
		getKey(index+2, &time[3], &value[3]);

		float t = (time[2] - frameTime) / (time[2] - time[1]);
		T out_value = Math::catmullRom(1.0f - t, value[0], value[1], value[2], value[3]);
		writeOutput<T>(out_value);
	}
};

typedef NodeSC<vec2>	NodeSC2;
typedef NodeSC<vec3>	NodeSC3;
typedef NodeSC<vec4>	NodeSC4;

} // Vorticity

#endif