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
	template < > bool setValue<float>(const float& value) const;
	template < > bool setValue<int>(const int& value) const;

	template <class T>
	bool getValue(T& value) const
	{
		if(!attrib) return false;
		if(index == 0)
			return attrib->getValue(value);
		else return false;
	}
	template < > bool getValue<float>(float& value) const;
	template < > bool getValue<int>(int& value) const;
};

} // Vorticity

#endif