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