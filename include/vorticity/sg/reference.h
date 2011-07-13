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

#ifndef __VORTICITY_REFERENCE_H
#define __VORTICITY_REFERENCE_H

namespace Vorticity {

class Node;
class AnyAttribute;

class VAPI Reference
{
private:
	Node*			node;
	AnyAttribute*	attrib;
	unsigned int	index;

	enum AttribHint
	{
		NOINDEX = 0,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		MATRIX4,
	};
	AttribHint hint;
public:
	Reference(Node *node);
	Reference(Node *node, AnyAttribute *attrib, const unsigned int index=0);

	AnyAttribute*	getAttrib() const		{ return attrib; }
	Node*			getNode() const			{ return node;	 }
	unsigned int	getIndex() const		{ return index;  }

	template <class T> 
	bool setValue(const T& value) const
	{
		if(!attrib) return false;
		if(index == 0)
			return attrib->setValue(value);
		else return false;
        }

	template <class T>
	bool getValue(T& value) const
	{
		if(!attrib) return false;
		if(index == 0)
			return attrib->getValue(value);
		else return false;
        }
};
template < > bool Reference::setValue<float>(const float& value) const;
template < > bool Reference::getValue<int>(int& value) const;

template < > bool Reference::getValue<float>(float& value) const;
template < > bool Reference::getValue<int>(int& value) const;

} // Vorticity

#endif
