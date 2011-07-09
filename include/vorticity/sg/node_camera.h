#ifndef __VORTICITY_NODE_CAMERA_H
#define __VORTICITY_NODE_CAMERA_H

namespace Vorticity {

class RenderAsset;
class Backdrop;
class VAPI NodeCamera : public Object
{
protected:
	RenderAsset* target;
	Backdrop* backdrop;

	float 	fov, zNear, zFar;
	bool 	useProjectionMatrix;
	matrix4 customProjection;
public:
	NodeCamera(const string& name, XNode *parent=NULL);
	virtual ~NodeCamera();

	virtual string getType() const
	{ return "camera"; }
	RenderAsset* getTargetBuffer() const
	{ return target; }

	virtual void evaluate();
	virtual bool bind(Resource* resource);

	void 		setBackdrop(Backdrop* backdrop)	{ this->backdrop = backdrop; 	}
	Backdrop* 	getBackdrop() const				{ return backdrop;				}
	bool		isUsingCustomProjection() const	{ return useProjectionMatrix;	}
	void		useCustomProjection(bool val)	{ useProjectionMatrix = val;	}

	const float getFOV() const
	{ return fov; }
	const float getZNear() const
	{ return zNear; }
	const float getZFar() const
	{ return zFar; }

	virtual bool	setupProjection() const;
	virtual bool	beginRendering(ClearMode cmode) const;
	virtual bool	endRendering() const;
};

} // Vorticity

#endif