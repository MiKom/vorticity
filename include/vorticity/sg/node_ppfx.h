#ifndef __VORTICITY_NODE_PPFX_H
#define __VORTICITY_NODE_PPFX_H

namespace Vorticity {

class NodePostProcess : public Node
{
public:
	NodePostProcess(const string& name, XNode *parent=NULL) : Node(name, parent)
	{ }
	virtual ~NodePostProcess()
	{ }

	virtual string getType() const
	{ return "ppfx"; }
};

} // Vorticity

#endif