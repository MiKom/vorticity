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
#include "vorticity/sg/proxy.h"

using namespace Vorticity;

Proxy::Proxy(const Vorticity::string &name, XNode *parent) : Modifier(name, parent)
{
}

Proxy::~Proxy()
{
}

bool Proxy::isType(const string& type)
{
	if(proxyMap.find(type) == proxyMap.end())
		return false;
	return true;
}

ProxyNodeIterator Proxy::enumType(const string& type)
{
	ProxyNodeMap::const_iterator it = proxyMap.find(type);
	return ProxyNodeIterator(it->second.begin(), it->second.end());
}

Node* Proxy::getNode(ProxyNodeIterator& it)
{
	if(it.first == it.second)
		return NULL;
	return (*it.first++);
}

bool Proxy::attachNode(Node *node)
{
	ProxyNodeMap::iterator it = proxyMap.find(node->getType());
	if(it == proxyMap.end())
	{
		proxyMap[node->getType()] = NodeArray();
		it = proxyMap.find(node->getType());
	}
	
	ProxyNodeIterator n_it(it->second.begin(), it->second.end());
	while(Node *n = getNode(n_it))
	{ 
		if(n->getName() == node->getName())
			return false;
	}
	it->second.push_back(node);
	return true;
}

bool Proxy::detachNode(const string &type, const size_t index)
{
	ProxyNodeMap::iterator it = proxyMap.find(type);
	if(it == proxyMap.end())
		return false;
	if(index >= it->second.size())
		return false;
	it->second.erase(it->second.begin()+index);
	return true;
}

bool Proxy::detachNode(const string &type, const string &name)
{
	ProxyNodeMap::iterator it = proxyMap.find(type);
	if(it == proxyMap.end())
		return false;
	ProxyNodeIterator n_it(it->second.begin(), it->second.end());
	while(Node *n = getNode(n_it))
	{
		if(n->getName() == name)
		{
			it->second.erase(--n_it.first);
			return true;
		}
	}
	return false;
}

bool Proxy::detachNode(Node *node)
{ return detachNode(node->getType(), node->getName()); }

bool Proxy::clearType(const string& type)
{
	ProxyNodeMap::iterator it = proxyMap.find(type);
	if(it == proxyMap.end())
		return false;
	it->second.clear();
	return true;
}

bool Proxy::clearAll()
{
	proxyMap.clear();
	return true;
}

void Proxy::evaluate()
{
	for(ProxyNodeMap::const_iterator it = proxyMap.begin();
		it != proxyMap.end(); it++)
	{
		for(NodeArray::const_iterator n_it = it->second.begin();
			n_it < it->second.end(); n_it++)
		{
			(*n_it)->setEvaluated();
			(*n_it)->evaluate();
		}
	}
}