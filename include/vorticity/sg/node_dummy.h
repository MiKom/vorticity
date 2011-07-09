#ifndef __VORTICITY_NODE_DUMMY_H
#define __VORTICITY_NODE_DUMMY_H

namespace Vorticity {

class VAPI NodeDummy : public Object
{
public:
	NodeDummy(const string& name, XNode *parent=NULL);

	virtual string getType() const
	{ return "dummy"; }

	virtual void evaluate();
	virtual void render(Renderer *device);
};

} //Vorticity

#endif