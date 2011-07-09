#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "object.h"
#include "node_light.h"
#include "node_light_sm.h"
#include "node_camera.h"
#include "resources/resource.h"

using namespace Vorticity;

NodeLightSM::NodeLightSM(const string& name, XNode *parent) : NodeLight(name, parent)
{
	fbo = 0;
}

NodeLightSM::~NodeLightSM()
{
	if(fbo)
		getCore()->getDevice()->destroyFramebuffer(fbo);
}

bool NodeLightSM::createShadowmap(const int w, const int h,
								  const float fov, const float zfar)
{
	Renderer *dev = getCore()->getDevice();

	if(fbo && !dev->destroyFramebuffer(fbo))
		return false;

	fbo = dev->createFramebuffer(Vorticity::BufferRGBA, w, h, 0);
	dev->addBufferTarget(fbo, Vorticity::ColorBuffer, NoFilter);
	dev->addBufferTarget(fbo, Vorticity::DepthBuffer, NoFilter);

	string cam_name = "__cam_";
	cam_name.append(name);

	if(shadowCamera)
		delete shadowCamera;
	shadowCamera = new NodeCamera(cam_name, this);
	shadowCamera->bind(new RenderAsset(fbo));
	shadowCamera->getAttrib("fov").setValue(fov);
	shadowCamera->getAttrib("znear").setValue(1.0f);
	shadowCamera->getAttrib("zfar").setValue(zfar);

	return true;
}

void NodeLightSM::setupShader(const int shader, Renderer* device, const int index)
{
	NodeLight::setupShader(shader, device, index);
}

void NodeLightSM::render(Renderer* device)
{
	
}