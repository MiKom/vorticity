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

#ifndef __VORTICITY_OBJECT_H
#define __VORTICITY_OBJECT_H

namespace Vorticity {

class VAPI Object : public XNode
{
protected:
	matrix4	worldTM;
	matrix4	localTM;
protected:
	Object(const string& name, XNode *parent=NULL);

public:
	virtual ~Object();

	const matrix4& getWorldTM() const
	{ return worldTM; }
	const matrix4& getLocalTM() const
	{ return localTM; }
	matrix4 getWorldInvTranspose() const
	{ return worldTM.getInverted().getTransposed(); }
	matrix4 getLocalInvTranspose() const
	{ return localTM.getInverted().getTransposed(); }

	virtual string getClass() const
	{ return "object"; }
	
	virtual void updateTransformation(bool propagate=true);
};

/**
 * Empty object that doesn't render anything. Can be use as base for
 * transformations (through parent-child relationship).
 **/
class VAPI ObjectNull : public Object
{
public:
	ObjectNull(const string& name, XNode *parent=NULL) : Object(name, parent)
	{}

	string getType() const
	{ return "null"; }
};

} // Vorticity

#endif