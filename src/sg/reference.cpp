#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "reference.h"

using namespace Vorticity;

Reference::Reference(Node *node)
{
	this->node		= node;
	this->attrib	= NULL;
	this->index		= 0;
	this->hint		= NOINDEX;
}

Reference::Reference(Node *node, AnyAttribute* attrib, const unsigned int index)
{
	this->node	 = node;
	this->attrib = attrib;
	this->index  = index;
	this->hint	 = NOINDEX;

	if(attrib->getType() == "float2")
		hint = VECTOR2;
	else if(attrib->getType() == "float3")
		hint = VECTOR3;
	else if(attrib->getType() == "float4")
		hint = VECTOR4;
	else if(attrib->getType() == "matrix4")
		hint = MATRIX4;
}

template < > bool Reference::setValue<int>(const int& value) const
{ return setValue<float>((const float&)value); }

template < > bool Reference::setValue<float>(const float& value) const
{ 
	if(!attrib) return false;

	if(index == 0)
		return attrib->setValue(value);
	if(hint == VECTOR2 && index <= 2) {
		vector2 v; attrib->getValue(v);
		v.cell[index-1] = value;
		return attrib->setValue(v);
	}
	else if(hint == VECTOR3 && index <= 3) {
		vector3 v; attrib->getValue(v);
		v.cell[index-1] = value;
		return attrib->setValue(v);
	}
	else if(hint == VECTOR4 && index <= 4) {
		vector4 v; attrib->getValue(v);
		v.cell[index-1] = value;
		return attrib->setValue(v);
	}
	else if(hint == MATRIX4 && index <= 16) {
		matrix4 v; attrib->getValue(v);
		v.cell[index-1] = value;
		return attrib->setValue(v);
	}
	else return false;
}

template < > bool Reference::getValue<int>(int& value) const
{ return getValue<float>((float&)value); }

template < > bool Reference::getValue<float>(float& value) const
{
	if(!attrib) return false;

	if(index == 0)
		return attrib->getValue(value);
	if(hint == VECTOR2 && index <= 2) {
		vector2 v; attrib->getValue(v);
		value = v.cell[index-1];
		return true;
	}
	else if(hint == VECTOR3 && index <= 3) {
		vector3 v; attrib->getValue(v);
		value = v.cell[index-1];
		return true;
	}
	else if(hint == VECTOR4 && index <= 4) {
		vector4 v; attrib->getValue(v);
		value = v.cell[index-1];
		return true;
	}
	else if(hint == MATRIX4 && index <= 16) {
		matrix4 v; attrib->getValue(v);
		value = v.cell[index-1];
		return true;
	}
	else return false;
}
