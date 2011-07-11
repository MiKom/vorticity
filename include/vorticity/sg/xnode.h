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

#ifndef __VORTICITY_XNODE_H
#define __VORTICITY_XNODE_H

namespace Vorticity {

class ModifierStackElement;
class AttribReference;
class Modifier;

typedef std::list<ModifierStackElement>	ModifierStack;

class VAPI ModifierStackElement
{
protected:
	Modifier* modifier;
	int priority;
public:
	ModifierStackElement(Modifier* modifier, const int priority)
	{
		this->modifier = modifier;
		this->priority = priority;
	}

	bool operator < (const ModifierStackElement& e) const
	{ return priority > e.priority; }
	bool operator > (const ModifierStackElement& e) const
	{ return priority < e.priority; }

	Modifier& getModifier() const
	{ return *modifier; }

	int getPriority() const
	{ return priority; }
};

class VAPI XNode : public Node
{
private:
	bool			destructFlag;
protected:
	NodeArray		children;
	ModifierStack	modstack;

	XNode(const string &name, XNode *parent=NULL);
public:
	virtual ~XNode();

	bool	addChild(Node* node);
	Node*	getChild(const size_t index) const;
	Node*	getChild(const string &name) const;
	bool	removeChild(const size_t index);
	bool	removeChild(const string &name);

	virtual bool hasChildren() const
	{ return children.size() > 0; }
	virtual bool isExtended() const
	{ return true; }

	inline NodeIterator enumChildren(const size_t index) const;
	inline Node* getChild(NodeIterator &it) const;

	bool		addModifier(Modifier* mod, const int priority);
	bool		removeModifier(const string &type);
	Modifier*	getModifier(const size_t index) const;
	Modifier*	getModifier(const string &type) const;

	size_t	getChildCount() const
	{ return children.size(); }

	size_t	getModifierCount() const
	{ return modstack.size(); }

	virtual void	clearEvaluated();
	virtual string	getClass() const	{ return "xnode"; }
};

class VAPI Group : public XNode
{
public:
	Group(const string& name, XNode *parent=NULL) : XNode(name, parent) {}
	virtual ~Group() {}

	virtual string getType() const
	{ return "group"; }
};

} // Vorticity

#endif