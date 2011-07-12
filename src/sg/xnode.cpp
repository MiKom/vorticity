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
#include "vorticity/sg/modifier.h"

using namespace Vorticity;

XNode::XNode(const string &name, Vorticity::XNode *parent) : Node(name, parent)
{
	destructFlag = false;
}

XNode::~XNode()
{
	destructFlag = true;
	for(NodeArray::iterator it=children.begin(); it < children.end(); it++)
		delete *it;
}

bool XNode::addChild(Vorticity::Node *node)
{
	for(NodeArray::iterator it=children.begin();
		it<children.end(); it++)
	{
		if((*it)->getName() == node->getName())
			return false;
	}
	children.push_back(node);
	node->setParent(this);
	return true;
}

Node* XNode::getChild(const size_t index) const
{
	if(index >= children.size())
		return NULL;
	return children.at(index);
}

Node* XNode::getChild(const string &name) const
{
	for(NodeArray::const_iterator it=children.begin();
		it<children.end(); it++)
	{
		if((*it)->getName() == name)
			return *it;
	}
	return NULL;
}

bool XNode::removeChild(const size_t index)
{
	if(index >= children.size())
		return false;
	auto it = children.begin() + index;
	(*it)->setParent(nullptr);
	children.erase(it);
	return true;
}

bool XNode::removeChild(const string &name)
{
	// Prevents circular child deletion!
	if(destructFlag)
		return false;

	for(auto it=children.begin(); it<children.end(); it++)
	{
		if((*it)->getName() == name)
		{
			(*it)->setParent(nullptr);
			children.erase(it);
			return true;
		}
	}
	return false;
}

void XNode::clearEvaluated()
{
	Node::clearEvaluated();
	for(NodeArray::iterator it=children.begin();
		it<children.end(); it++)
		(*it)->clearEvaluated();
}

bool XNode::addModifier(Vorticity::Modifier *mod, const int priority)
{
	int lp = priority;
	if(lp == 0 && modstack.size() > 0)
		lp = modstack.front().getPriority() + 1;

	for(ModifierStack::iterator it=modstack.begin();
		it != modstack.end(); it++)
	{
		if((*it).getModifier().getType() == mod->getType())
			return false;
	}
	modstack.push_back(ModifierStackElement(mod, lp));
	modstack.sort();
	return true;
}

bool XNode::removeModifier(const string &type)
{
	for(ModifierStack::iterator it=modstack.begin();
		it != modstack.end(); it++)
	{
		if((*it).getModifier().getType() == type)
		{
			modstack.erase(it);
			return true;
		}
	}
	return false;
}

Modifier* XNode::getModifier(const size_t index) const
{
	size_t i=0;
	if(index >= modstack.size())
		return NULL;
	for(ModifierStack::const_iterator it=modstack.begin();
		it != modstack.end(); it++)
	{
		if(i++ == index)
			return &(*it).getModifier();
	}
	return NULL;
}

Modifier* XNode::getModifier(const string &type) const
{
	for(ModifierStack::const_iterator it=modstack.begin();
		it != modstack.end(); it++)
	{
		if((*it).getModifier().getType() == type)
			return &(*it).getModifier();
	}
	return NULL;
}

NodeIterator XNode::enumChildren(const size_t index) const
{ return children.begin() + index; }

Node* XNode::getChild(NodeIterator &it) const
{
	if(it == children.end())
		return NULL;
	return (*it++);
}