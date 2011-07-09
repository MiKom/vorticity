#ifndef __VORTICITY_OBJECT_H
#define __VORTICITY_OBJECT_H

namespace Vorticity {

class VAPI Object : public XNode
{
protected:
	matrix4	worldTM;
	matrix4	localTM;
protected:
	Object(const string& name, XNode *parent=NULL);

public:
	virtual ~Object();

	const matrix4& getWorldTM() const
	{ return worldTM; }
	const matrix4& getLocalTM() const
	{ return localTM; }
	matrix4 getWorldInvTranspose() const
	{ return worldTM.getInverted().getTransposed(); }
	matrix4 getLocalInvTranspose() const
	{ return localTM.getInverted().getTransposed(); }

	virtual string getClass() const
	{ return "object"; }
	
	virtual void updateTransformation(bool propagate=true);
};

/**
 * Empty object that doesn't render anything. Can be use as base for
 * transformations (through parent-child relationship).
 **/
class VAPI ObjectNull : public Object
{
public:
	ObjectNull(const string& name, XNode *parent=NULL) : Object(name, parent)
	{}

	string getType() const
	{ return "null"; }
};

} // Vorticity

#endif