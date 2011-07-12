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

#ifndef __VORTICITY_PROXY_H
#define __VORTICITY_PROXY_H

namespace Vorticity {

typedef std::map<string, NodeArray>	ProxyNodeMap;
typedef std::pair<NodeIterator, NodeIterator> ProxyNodeIterator;

class VAPI Proxy : public Modifier
{
protected:
	ProxyNodeMap	proxyMap;
public:
	Proxy(const string& name, XNode *parent=NULL);
	virtual ~Proxy();

	virtual bool	attachNode(Node *node);
	virtual bool	detachNode(Node *node);
	virtual bool	detachNode(const string& type, const string& name);
	virtual bool	detachNode(const string& type, const size_t index);

	virtual bool	isType(const string& type);
	virtual Node*	getNode(ProxyNodeIterator& it);

	virtual ProxyNodeIterator enumType(const string& type);

	virtual bool	clearType(const string& type);
	virtual bool	clearAll();
	virtual void	evaluate();

	virtual string getType() const
	{ return "proxy"; }
};

} // Vorticity

#endif