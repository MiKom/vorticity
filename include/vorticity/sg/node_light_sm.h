#ifndef __NODE_LIGHTSM_H
#define __NODE_LIGHTSM_H

namespace Vorticity {

class NodeCamera;
class VAPI NodeLightSM : public NodeLight
{
protected:
	int fbo;
	NodeCamera* shadowCamera;
public:
	NodeLightSM(const string& name, XNode *parent=NULL);
	virtual ~NodeLightSM();

	virtual bool createShadowmap(const int w, const int h,
		const float fov, const float zfar);

	virtual void render(Renderer* device);
	virtual void setupShader(const int shader, Renderer *device, const int index);

	virtual string getShadowType() const
	{ return "shadowmap"; }

};

} // Vorticity

#endif