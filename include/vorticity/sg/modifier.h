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