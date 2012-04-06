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

using namespace Vorticity;

Object::Object(const Vorticity::string &name, Vorticity::XNode *parent) : XNode(name, parent)
{
	addAttrib(new Attribute<vec3>("position", true));
	addAttrib(new Attribute<vec3>("rotation", true));
	addAttrib(new Attribute<vec3>("pivot", true));
	addAttrib(new Attribute<vec3>("scale", true)).setValue(vector3().ones());

	addAttrib(new Attribute<matrix4>("base_tm", true)).setValue(matrix4().identity());
	worldTM.identity();
	localTM.identity();
}

Object::~Object()
{ }

void Object::updateTransformation(bool propagate)
{
	Attribute<vec3> p, r, s, pv;
	Attribute<matrix4> tm;

	p 	= *(Attribute<vec3>*)&getAttrib("position");
	r 	= *(Attribute<vec3>*)&getAttrib("rotation");
	s 	= *(Attribute<vec3>*)&getAttrib("scale");
	pv	= *(Attribute<vec3>*)&getAttrib("pivot");
	tm 	= *(Attribute<matrix4>*)&getAttrib("base_tm");
	
	bool recalcTransform = 
		p.isDirty() || r.isDirty() || s.isDirty() || tm.isDirty() || pv.isDirty();
	//recalcTransform = true;
	
	if(recalcTransform)
	{
		if(parent && parent->getClass() == "object")
			worldTM = ((Object*)parent)->getWorldTM();
		else
			worldTM.identity();

		vector3 vr;
		tm.getValue(localTM);
		r.getValue(vr);
		tm.setClean();

		localTM.translate(p);
		localTM.rotate(vector3(vr.x*Math::Pi180, vr.y*Math::Pi180, vr.z*Math::Pi180));
		localTM.translate(pv);

		//FIXME: Gcc is more strict about type matching,
		localTM.scale((const vec3&)s);
		worldTM *= localTM;

		p.setClean(); r.setClean(); s.setClean(); pv.setClean();
	}

	if(propagate && recalcTransform)
	{
		NodeConstIterator it = enumChildren(0);
		while(Node *n = getChild(it))
			if(n->getClass() == "object") ((Object*)n)->updateTransformation(true);
	}
}
