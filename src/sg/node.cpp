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

using namespace Vorticity;

Node::Node(const string &name, Vorticity::XNode *parent) : Entity(),
        enabled(true), hidden(false), parent(NULL)
{
	this->name			= const_cast<string&>(name);
	this->evaluateFlag	= false;

	if(parent)
		parent->addChild(this);
}

Node::~Node()
{
	if(parent)
		parent->removeChild(name);
	disconnectAll();
	for(auto it=affectedBy.begin(); it<affectedBy.end(); it++)
		(*it)->removeFromConnectedTo(this);
	for(AttributeMap::iterator it=attributes.begin(); it!=attributes.end(); it++)
		delete it->second;
}

inline const string& Node::getName() const
{ return name; }

inline void Node::setName(const string &name)
{ this->name = const_cast<string&>(name); }

inline XNode* Node::getParent() const
{ return parent; }

inline void Node::setParent(Vorticity::XNode *parent)
{ this->parent = parent; }

AnyAttribute& Node::addAttrib(Vorticity::AnyAttribute *attrib, bool replace)
{
	if(!attrib) return NullAttribute::Null;
	if(!replace)
	{
		if(attributes.find(attrib->getName()) != attributes.end())
			return NullAttribute::Null;
	}

	attributes[attrib->getName()] = attrib;
	return *attrib;
}

AnyAttribute& Node::getAttrib(const string &name) const
{
	AttributeMap::const_iterator loc = attributes.find(name);
	if(loc == attributes.end())
		return NullAttribute::Null;
	return *(loc->second);
}

bool Node::removeAttrib(const string &name)
{
	AttributeMap::iterator loc = attributes.find(name);
	if(loc == attributes.end())
		return false;
	attributes.erase(loc);
	return true;
}

bool Node::canConnectTo(Vorticity::Node *pnode, const string &attrib, const int index) const
{
	AnyAttribute& nodeAttrib = pnode->getAttrib(attrib);
	if(nodeAttrib.getType() == "null")
		return false;

	if(index == 0)
		return isTypeCompatible(nodeAttrib.getType());
	else
	{
		string type = nodeAttrib.getType();
		if(type == "float2" || type == "float3" || type == "float4" || type == "matrix4")
			return isTypeCompatible("float");
		else return false;
	}
}

bool Node::isConnectedTo(Vorticity::Node *pnode, const string &attrib, const int index) const
{
	for(ReferenceArray::const_iterator it=outputNodes.begin();
		it<outputNodes.end(); it++)
	{
		if((*it).getNode() == pnode)
		{
			if((*it).getAttrib()->getName() == attrib
				&& (*it).getIndex() == index)
				return true;
		}
	}
	return false;
}

bool Node::isAffectedBy(Vorticity::Node *pnode) const
{
	for(NodeArray::const_iterator it=affectedBy.begin();
		it<affectedBy.end(); it++)
	{
		if((*it) == pnode)
			return true;
	}
	return false;
}

inline void Node::addToAffectedBy(Vorticity::Node *pnode)
{ affectedBy.push_back(pnode); }

bool Node::removeFromAffectedBy(Vorticity::Node *pnode)
{
	for(NodeArray::iterator it=affectedBy.begin();
		it<affectedBy.end(); it++)
	{
		if((*it) == pnode)
		{
			affectedBy.erase(it);
			return true;
		}
	}
	return false;
}

bool Node::removeFromConnectedTo(Vorticity::Node* pnode)
{
	for(auto it=outputNodes.begin(); it<outputNodes.end(); it++)
	{
		if(it->getNode()->getName() == pnode->getName())
		{
			outputNodes.erase(it);
			return true;
		}
	}
	return false;
}

bool Node::connectTo(Vorticity::Node* pnode, const string &attrib, const int index)
{
	if(isConnectedTo(pnode, attrib, index))
		return false;
	if(!canConnectTo(pnode, attrib, index))
		return false;

	AnyAttribute& pattrib = pnode->getAttrib(attrib);
	pnode->addToAffectedBy(this);
	outputNodes.push_back(Reference(pnode, &pattrib, index));
	return true;
}

bool Node::disconnect(const size_t index)
{
	if(index >= outputNodes.size())
		return false;
	ReferenceArray::iterator it = outputNodes.begin()+index;
	Node* affectedNode = (*it).getNode();
	outputNodes.erase(it);
	return affectedNode->removeFromAffectedBy(this);
}

bool Node::disconnect(const string &name)
{
	ReferenceArray::iterator it = outputNodes.begin();
	bool found = false;
	for(;it<outputNodes.end(); it++)
	{
		if((*it).getAttrib()->getName() == name)
		{
			found = true;
			break;
		}
	}
	if(!found)
		return false;

	Node* affectedNode = (*it).getNode();
	outputNodes.erase(it);
	return affectedNode->removeFromAffectedBy(this);
}

void Node::disconnectAll()
{
	for(auto it=outputNodes.begin();it<outputNodes.end(); it++)
		(*it).getNode()->removeFromAffectedBy(this);
	outputNodes.clear();
}

NodeConstIterator Node::enumAffectedBy(const size_t index) const
{ return affectedBy.begin() + index; }

Node* Node::getAffectedBy(NodeConstIterator &it) const
{
	if(it == affectedBy.end())
		return NULL;
	return (*it++);
}

AttributeIterator Node::enumAttributes(const size_t index) const
{
	AttributeIterator it = attributes.begin();
	for(size_t i=0; i<index; i++)
		it++;
	return it;
}

AnyAttribute* Node::getAttrib(AttributeIterator &it) const
{
	AnyAttribute* result;
	if(it == attributes.end())
		return NULL;
	result = it->second;
	it++;
	return result;
}

void Node::hideSubgraph(bool flag)
{
	hidden = flag;
	if(hasChildren())
	{
                NodeConstIterator it = ((XNode*)this)->enumChildren(0);
		while(Node* n = ((XNode*)this)->getChild(it))
			n->hideSubgraph(flag);
	}
}

bool Node::changeParent(XNode* pnew)
{
	if(parent)
		parent->removeChild(name);
	return pnew->addChild(this);
}
