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


#ifndef __VORTICITY_ARRAY_H
#define __VORTICITY_ARRAY_H

namespace Vorticity {

class VAPI OutOfRangeException : public std::runtime_error
{
public:
	OutOfRangeException() : std::runtime_error("Array index out of range!") {}
};

typedef std::vector<AnyAttribute*> AttribArray;
typedef AttribArray::const_iterator AttribIterator;

class VAPI ArrayAttribute : public AnyAttribute
{
protected:
	AttribArray		values;
public:
	ArrayAttribute(const string &name="", bool storable=true);
	ArrayAttribute(const ArrayAttribute &source);
	virtual ~ArrayAttribute();

	virtual ArrayAttribute* clone() const;

	virtual string getType() const
	{ return "array"; }

	virtual bool isArray() const
	{ return true; }

	AnyAttribute&			getAt(const size_t index) const;
	AnyAttribute&			getByName(const string& name) const;
	inline AnyAttribute&	front() const;
	inline AnyAttribute&	back() const;

	bool			setAt(const size_t index, AnyAttribute *attrib);
	inline void		push(AnyAttribute *attrib);
	inline bool		pop(AnyAttribute *attrib);
	inline bool		pop();
	bool			remove(const size_t index);
	bool			remove(const size_t start, const size_t end);
	void			clear();

	AttribIterator  enumValues() const;
	AnyAttribute*	getAt(AttribIterator &it) const;

	size_t			getCount() const
	{ return values.size(); }

	AnyAttribute& operator[](const size_t index) const
	{ return getAt(index); }
	AnyAttribute& operator[](const string &name) const
	{ return getByName(name); }
};

} // Vorticity

#endif