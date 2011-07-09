#ifndef __VORTICITY_NODE_LIGHT_H
#define __VORTICITY_NODE_LIGHT_H

namespace Vorticity {

class VAPI NodeLight : public Object
{
protected:
	vector3	worldPosition;
	vector3	worldRotation;
public:
	NodeLight(const string& name, XNode *parent=NULL);
	virtual ~NodeLight();

	virtual string getType() const
	{ return "light"; }
	virtual string getShadowType() const
	{ return "none"; }

	virtual void evaluate();
	virtual void setupShader(const int shader, Renderer *device, const int index);
};

} // Vorticity

#endif