#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "modifier.h"

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
