#include "config.h"
#include "core/entity.h"
#include "attribute.h"
#include "array.h"

using namespace Vorticity;

// TODO: Co z dirtyFlag?

ArrayAttribute::ArrayAttribute(const string &name, bool storable) : AnyAttribute(name, storable)
{ }

ArrayAttribute::ArrayAttribute(const Vorticity::ArrayAttribute &source)
{
	for(AttribArray::const_iterator it=source.values.begin();
		it<source.values.end(); it++)
	{
		push((*it)->clone());
	}
	dirtyFlag = true;
}

ArrayAttribute::~ArrayAttribute()
{ clear(); }

ArrayAttribute* ArrayAttribute::clone() const
{ return NULL; }

AnyAttribute& ArrayAttribute::getAt(const size_t index) const
{
	if(index >= values.size())
		return NullAttribute::Null;
	return *values.at(index);
}

AnyAttribute& ArrayAttribute::getByName(const string &name) const
{
	for(AttribArray::const_iterator it=values.begin(); it<values.end(); it++)
	{
		if((*it)->getName() == name)
			return **it;
	}
	return NullAttribute::Null;
}

inline AnyAttribute& ArrayAttribute::front() const
{
	if(values.size() == 0)
		return NullAttribute::Null;
	return *values.front();
}

inline AnyAttribute& ArrayAttribute::back() const
{ 
	if(values.size() == 0)
		return NullAttribute::Null;
	return *values.back();
}

bool ArrayAttribute::setAt(const size_t index, Vorticity::AnyAttribute *attrib)
{
	if(index >= values.size())
		return false;
	values[index] = attrib;
	return true;
}

inline void ArrayAttribute::push(Vorticity::AnyAttribute *attrib)
{ values.push_back(attrib); }

inline bool ArrayAttribute::pop(Vorticity::AnyAttribute *attrib)
{
	if(values.size() == 0)
		return false;
	attrib = values.back();
	values.pop_back();
	return true;
}

inline bool ArrayAttribute::pop()
{
	if(values.size() == 0)
		return false;
	delete values.back();
	values.pop_back();
	return true;
}

bool ArrayAttribute::remove(const size_t index)
{
	if(index >= values.size())
		return false;
	AttribArray::iterator it = values.begin()+index;
	delete *it;
	values.erase(it);
	return true;
}

bool ArrayAttribute::remove(const size_t start, const size_t end)
{
	if(start >= values.size() || end >= values.size())
		return false;

	AttribArray::iterator ibeg = values.begin()+start;
	AttribArray::iterator iend = values.end();
	if(end > 0)
		iend = values.begin()+end;

	for(AttribArray::iterator it=ibeg; it<iend; it++)
		delete *it;
	values.erase(ibeg, iend);
	return true;
}

void ArrayAttribute::clear()
{
	for(AttribArray::iterator it=values.begin(); it<values.end(); it++)
		delete *it;
	values.clear();
}

AttribIterator ArrayAttribute::enumValues() const
{ return values.begin(); }

AnyAttribute* ArrayAttribute::getAt(AttribIterator &it) const
{
	if(it == values.end())
		return NULL;
	return (*it++);
}