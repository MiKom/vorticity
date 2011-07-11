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


#ifndef __VORTICITY_MODIFIER_H
#define __VORTICITY_MODIFIER_H

namespace Vorticity {

class VAPI Modifier : public Node
{
protected:
	int priority;
protected:
	Modifier(const string &name, XNode* parent=NULL);
public:
	virtual ~Modifier();

	virtual bool	canConnectTo(Node *pnode, const string &attrib="", const int index=0) const;
	virtual bool	isConnectedTo(Node *pnode, const string &attrib="", const int index=0) const;

	virtual bool	connectTo(Node* pnode, const string &attrib="", const int index=0);
	virtual bool	connectTo(Node* pnode, const int new_priority);
	virtual bool	disconnect(const size_t index);
	virtual bool	disconnect(const string &name);
	virtual void	disconnectAll();

	virtual void	setupShader(const int shader, Renderer *device)
	{ }

	void	setPriority(const int p)
	{ priority = p; }
	int		getPriority() const
	{ return priority; }
};

} // Vorticity

#endif