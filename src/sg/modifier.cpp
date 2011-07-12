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

Modifier::Modifier(const string &name, Vorticity::XNode* parent) : Node(name, parent)
{
	priority = 0;
}

Modifier::~Modifier()
{
}

bool Modifier::canConnectTo(Vorticity::Node *pnode, const string &attrib, const int index) const
{
	if(!pnode->isExtended())
		return false;
	if(((XNode*)pnode)->getModifier(getType()) != NULL)
		return false;
	return true;
}

bool Modifier::isConnectedTo(Vorticity::Node *pnode, const string&, const int) const
{
	if(pnode->getClass() != "xnode")
		return false;
	return (((XNode*)pnode)->getModifier(getType()) != NULL);
}

bool Modifier::connectTo(Vorticity::Node *pnode, const string&, const int)
{
	if(!canConnectTo(pnode))
		return false;

	pnode->addToAffectedBy(this);
	outputNodes.push_back(Reference(pnode));
	return ((XNode*)pnode)->addModifier(this, priority);
}

bool Modifier::connectTo(Vorticity::Node *pnode, const int new_priority)
{
	if(!canConnectTo(pnode))
		return false;

	pnode->addToAffectedBy(this);
	outputNodes.push_back(Reference(pnode));
	return ((XNode*)pnode)->addModifier(this, new_priority);
}

bool Modifier::disconnect(const size_t index)
{
	if(index >= outputNodes.size())
		return false;
	ReferenceArray::iterator it = outputNodes.begin()+index;
	Node* affectedNode = (*it).getNode();
	outputNodes.erase(it);
	if(!affectedNode->removeFromAffectedBy(this))
		return false;
	return ((XNode*)affectedNode)->removeModifier(getType());
}

bool Modifier::disconnect(const Vorticity::string &name)
{
	ReferenceArray::iterator it = outputNodes.begin();
	Node *pnode;
	for(;it<outputNodes.end(); it++)
	{
		pnode = (*it).getNode();
		if(pnode->getName() == name)
		{
			outputNodes.erase(it);
			if(!pnode->removeFromAffectedBy(this))
				return false;
			return ((XNode*)pnode)->removeModifier(getType());
		}
	}
	return false;
}

void Modifier::disconnectAll()
{
	ReferenceArray::iterator it=outputNodes.begin();
	for(;it<outputNodes.end(); it++)
	{
		Node *pnode = (*it).getNode();
		pnode->removeFromAffectedBy(this);
		((XNode*)pnode)->removeModifier(getType());
	}
	outputNodes.clear();
}
