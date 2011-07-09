#ifndef __VORTICITY_CONTROLLER_H
#define __VORTICITY_CONTROLLER_H

namespace Vorticity {

class XNode;

class VAPI Controller : public Node
{
protected:
	Controller(const string& name, XNode *parent=NULL);

public:
	virtual ~Controller();

	virtual bool	getStatus() const = 0;

	virtual string getClass() const
	{ return "controller"; }
};

} // Vorticity

#endif