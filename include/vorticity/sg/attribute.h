#ifndef __VORTICITY_ATTRIBUTE_H
#define __VORTICITY_ATTRIBUTE_H

namespace Vorticity {

class VAPI BadTypeException : public std::runtime_error
{
public:
	BadTypeException() : std::runtime_error("Generic type mismatch!") {}
	BadTypeException(const string &str) : std::runtime_error(str) {}
};

class VAPI NotFoundException : public std::runtime_error
{
public:
	NotFoundException() : std::runtime_error("Attribute not found!") {}
	NotFoundException(const string& name)
		: std::runtime_error("Attribute not found: " + name) { }
};

class ArrayAttribute;
class CompoundAttribute;

typedef enum
{
	X = 1,
	Y = 2,
	Z = 3,
	W = 4,
	R = 1,
	G = 2,
	B = 3,
	A = 4,
} AttribComponent;

class VAPI AnyAttribute : public Entity
{
protected:
	mutable bool		dirtyFlag;
	string				name;
	ArrayAttribute*		parent;
protected:
	AnyAttribute();
	AnyAttribute(const string &name, bool storable=true);
	
public:
	virtual ~AnyAttribute();
	virtual AnyAttribute* clone() const = 0;

	virtual bool	store(std::fstream &file);
	virtual bool	load(std::fstream &file);

	virtual bool	getValue(int &v) const		{ return false; }
	virtual bool	setValue(const int &v)		{ return false; }
	virtual bool	getValue(float &v) const	{ return false; }
	virtual bool	setValue(const float &v)	{ return false; }
	virtual bool	getValue(vector2 &v) const	{ return false; }
	virtual bool	setValue(const vector2 &v)	{ return false; }
	virtual bool	getValue(vector3 &v) const	{ return false; }
	virtual bool	setValue(const vector3 &v)	{ return false; }
	virtual bool	getValue(vector4 &v) const	{ return false; }
	virtual bool	setValue(const vector4 &v)	{ return false; }
	virtual bool	getValue(matrix4 &v) const	{ return false; }
	virtual bool	setValue(const matrix4 &v)	{ return false; }
	virtual bool	getValue(string &v) const	{ return false; }
	virtual bool	setValue(const string &v)	{ return false; }

	virtual AnyAttribute& operator = (const int &v)
	{ throw BadTypeException("Type mismatch: int");			}
	virtual AnyAttribute& operator = (const float &v)
	{ throw BadTypeException("Type mismatch: float");		}
	virtual AnyAttribute& operator = (const vector2& v)
	{ throw BadTypeException("Type mismatch: vector2");		}
	virtual AnyAttribute& operator = (const vector3& v)
	{ throw BadTypeException("Type mismatch: vector3");		}
	virtual AnyAttribute& operator = (const vector4& v)
	{ throw BadTypeException("Type mismatch: vector4");		}
	virtual AnyAttribute& operator = (const matrix4& v)
	{ throw BadTypeException("Type mismatch: matrix4");		}
	virtual AnyAttribute& operator = (const string& v)
	{ throw BadTypeException("Type mismatch: string");		}

	virtual operator int&		() const	{ throw BadTypeException("Type mismatch: int");		}
	virtual operator float&		() const	{ throw BadTypeException("Type mismatch: float");	}
	virtual operator vector2&	() const	{ throw BadTypeException("Type mismatch: vector2"); }
	virtual operator vector3&	() const	{ throw BadTypeException("Type mismatch: vector3"); }
	virtual operator vector4&	() const	{ throw BadTypeException("Type mismatch: vector4"); }
	virtual operator matrix4&	() const	{ throw BadTypeException("Type mismatch: matrix4"); }
	virtual operator string&	() const	{ throw BadTypeException("Type mismatch: string");	}

	virtual string getClass() const
	{ return "attribute"; }

	virtual string getType() const
	{ return "any"; }

	const string& getName() const
	{ return name; }

	virtual bool isArray() const
	{ return false; }
	virtual bool isNull() const
	{ return false; }
	virtual bool isCompound() const
	{ return false; }

	virtual void		setParent(const ArrayAttribute *parent);
	ArrayAttribute*		getParent() const;

	virtual bool isDirty() const		{ return dirtyFlag;			}
	virtual void setDirty()				{ dirtyFlag = true;			}
	virtual void setClean()				{ dirtyFlag = false;		}
};

class NullAttribute : public AnyAttribute
{
public:
	static NullAttribute Null;
public:
	NullAttribute() : AnyAttribute("", false) { }
	virtual ~NullAttribute()				  { }

	static NullAttribute* create()
	{ return new NullAttribute(); }
	virtual NullAttribute* clone() const
	{ return new NullAttribute(); }

	virtual bool isNull() const
	{ return true; }
	virtual string getType() const
	{ return "null"; }
};

// generic attribute
template <class T>
class Attribute : public AnyAttribute
{
protected:
	T value;
public:
	Attribute(const string &name="", bool storable=true) : AnyAttribute(name, storable)
	{ value = T(); }
	Attribute(const string &name, const T &v, bool storable=true) : AnyAttribute(name, storable)
	{ value = v; }
	virtual ~Attribute() { }

	virtual inline string getType() const;

	static Attribute<T>* create(const string &name="", bool storable=true)
	{ return new Attribute<T>(name, storable); }
	static Attribute<T>* create(const string &name, const T &v, bool storable=true)
	{ return new Attribute<T>(name, v, storable); }

	virtual Attribute<T>* clone() const
	{ return new Attribute<T>(*this); }

	bool getValue(T &v) const
	{
		v = value;
		return true;
	}
	bool setValue(const T &v)
	{
		value		= v;
		dirtyFlag	= true;
		return true;
	}

	operator T& () const
	{
		dirtyFlag = true;
		return (T&)(value);
	}

	Attribute<T>& operator = (const T& v)
	{ 
		value = v;
		dirtyFlag = true;
		return *this;
	}

	virtual inline bool store(std::fstream &file);
	virtual inline bool load(std::fstream &file);
};

template <class T> inline string Attribute<T>::getType() const
{ return T::name2(); }

template < > inline string Attribute<int>::getType() const
{ return "int"; }

template < > inline string Attribute<float>::getType() const
{ return "float"; }

template <class T> inline bool Attribute<T>::load(std::fstream &file)
{
	AnyAttribute::load(file);
	file >> value;
	return file.good();
}

template < > inline bool Attribute<int>::load(std::fstream &file)
{
	AnyAttribute::load(file);
	file >> value;
	return file.good();
}

template < > inline bool Attribute<float>::load(std::fstream &file)
{
	AnyAttribute::load(file);
	file << value;
	return file.good();
}

template <class T> inline bool Attribute<T>::store(std::fstream &file)
{
	AnyAttribute::store(file);
	file << value;
	return file.good();
}

template < > inline bool Attribute<int>::store(std::fstream &file)
{
	AnyAttribute::store(file);
	file << value;
	return file.good();
}

template < > inline bool Attribute<float>::store(std::fstream &file)
{
	AnyAttribute::store(file);
	file << value;
	return file.good();
}

} // Vorticity

#include "array.h"
#include "reference.h"

#endif