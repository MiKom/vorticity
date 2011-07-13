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


#ifndef __VORTICITY_NODE_H
#define __VORTICITY_NODE_H

namespace Vorticity {

class Node;
class XNode;
class Reference;
class Renderer;
class Resource;

typedef std::map<string, AnyAttribute*>	AttributeMap;
typedef std::vector<Node*>				NodeArray;
typedef NodeArray::const_iterator		NodeIterator;
typedef AttributeMap::const_iterator	AttributeIterator;
typedef std::map<string, Node*>			NodeMap;
typedef std::vector<Reference>			ReferenceArray;
typedef ReferenceArray::const_iterator	ReferenceIterator;
typedef std::list<AnyAttribute*>		AttributeList;

#include "reference.h"

class VAPI Node : public Entity
{
	friend class Modifier;
	friend class XNode;
protected:
	XNode*			parent;
	string			name;
	bool			evaluateFlag;
	bool			enabled;
	bool			hidden;
	AttributeMap	attributes;
	NodeArray		affectedBy;
	ReferenceArray	outputNodes;
protected:
	Node(const string &name, XNode* parent=NULL);

	inline void		setParent(XNode *parent);
	inline void		addToAffectedBy(Node *pnode);
	bool			removeFromAffectedBy(Node *pnode);
	bool			removeFromConnectedTo(Node *pnode);
	virtual bool	isTypeCompatible(const string &type) const
	{ return false; }

	template <class T>
	bool writeOutput(const T& value, bool updateTransform=true) const
	{
                for(auto it=outputNodes.begin();
			it < outputNodes.end(); it++)
		{
			if(!(*it).setValue<T>(value))
				return false;
			if(updateTransform)
			{
				if((*it).getNode()->getClass() == "object")
					((Object*)(*it).getNode())->updateTransformation();
			}
		}
		return true;
	}
public:
	virtual ~Node();

	const string&	getName() const;
	void			setName(const string &name);
	XNode*			getParent() const;
	bool			changeParent(XNode* pnew);

	AnyAttribute&	addAttrib(AnyAttribute* attrib, bool replace=false);
	AnyAttribute&	getAttrib(const string& name) const;
	AnyAttribute&	getAttrib(const int index) const;
	bool			removeAttrib(const string &name);
	bool			removeAttrib(const int index);
	bool			isAffectedBy(Node *pnode) const;

	inline NodeIterator enumAffectedBy(const size_t index) const;
	inline Node* getAffectedBy(NodeIterator &it) const;

	inline AttributeIterator enumAttributes(const size_t index) const;
	inline AnyAttribute* getAttrib(AttributeIterator &it) const;

	virtual bool	canConnectTo(Node *pnode, const string &attrib="", const int index=0) const;
	virtual bool	isConnectedTo(Node *pnode, const string &attrib="", const int index=0) const;

	virtual bool	connectTo(Node* pnode, const string &attrib="", const int index=0);
	virtual bool	disconnect(const size_t index);
	virtual bool	disconnect(const string &name);
	virtual void	disconnectAll();

	virtual bool	hasChildren() const
	{ return false; }
	virtual bool	isExtended() const
	{ return false; }
	
	//! turn subgraph evaluation on and off
	/**
	 * Set whether the subgraph of this node should be evaluated during
	 * scene graph evaluation. Subgraph is also not rendered.
	 * \param e true to enable subgraph evaluation false to disable
	 **/
	void	setEnabled(bool e) 	{ enabled = e; 		}

	//! turn node rendering on and off
	/**
	 * Set whether this node should be rendered. Its children however
	 * may still be rendered if are not hidden.
	 * It may however still be evaluated.
	 * \param h true to hide the node false to show
	 **/
	void	setHidden(bool h) 	{ hidden  = h; 		}
	bool	isHidden() const	{ return hidden; 	}
	bool	isEnabled() const	{ return enabled; 	}

	void	hideSubgraph(bool flag);

	AnyAttribute& operator[](const string &name) const
	{ 
		AnyAttribute &attr = getAttrib(name);
		if(attr.isNull())
			throw NotFoundException(name);
		return attr;
	}

	size_t attribCount() const
	{ return attributes.size(); }

	bool hasOutput() const
	{ return (outputNodes.size() > 0); }

	bool isEvaluated() const
	{ return evaluateFlag; }

	virtual void clearEvaluated()
	{ evaluateFlag = false; }
	virtual void setEvaluated()
	{ evaluateFlag = true; }

	virtual string getClass() const	{ return "node"; }

	// "graph-new" implementation
        template <class T> inline friend T& gnew(const string& name, XNode& parent)
	{ return (T&)*new T(name, &parent); }
        template <class T> inline friend T& gnew(const string& name, XNode* parent)
	{ return (T&)*new T(name, parent);	}
        template <class T> inline friend T& gnew(const string& name)
	{ return (T&)*new T(name, NULL);	}

	virtual void setupShader(const int shader, Renderer *device)
	{ }
	virtual void setupShader(const int shader, Renderer *device, const int index)
	{ }

	virtual bool bind(Resource *res)			{ return false; }
	virtual void evaluate()						{ }
	virtual void render(Renderer *device)		{ }
};

class VAPI AbstractVisitor
{
protected:
	AbstractVisitor() { }
public:
	virtual void traverse(Node* root) = 0;
};

} // Vorticity

#endif
