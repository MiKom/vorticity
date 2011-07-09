#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "object.h"
#include "node_camera.h"
#include "core/renderer.h"
#include "resources/resource.h"
#include "resources/backdrop.h"

using namespace Vorticity;

NodeCamera::NodeCamera(const string &name, XNode *parent) : Object(name, parent)
{
	target = NULL;
	backdrop = NULL;
	useProjectionMatrix = false;

	addAttrib(new Attribute<float>("fov")).setValue(45.0f);
	addAttrib(new Attribute<float>("znear")).setValue(1.0f);
	addAttrib(new Attribute<float>("zfar")).setValue(100.0f);
	addAttrib(new Attribute<matrix4>("projection")).setValue(matrix4().identity());

	evaluate();
}

NodeCamera::~NodeCamera()
{
	delete backdrop;
	delete target;
}

void NodeCamera::evaluate()
{
	getAttrib("fov").getValue(fov);
	getAttrib("znear").getValue(zNear);
	getAttrib("zfar").getValue(zFar);
	if(useProjectionMatrix)
		getAttrib("projection").getValue(customProjection);
}

bool NodeCamera::bind(Resource *resource)
{
	if(resource->getType() != "render_asset")
		return false;
	target = (RenderAsset*)resource;
	return true;
}

bool NodeCamera::setupProjection() const
{
	if(!target)
		return false;

	int w,h;
	Renderer *device = target->getDevice();

	if(useProjectionMatrix)
		device->setProjection(customProjection.cell);
	else
	{
		if(!device->getFramebufferProperties(target->getID(), &w, &h, NULL))
			return false;
		device->setPerspectiveProjection(fov, ((float)w)/((float)h), zNear, zFar);
	}
	device->projectionTransform(worldTM.getInverted().cell);
	return true;
}

bool NodeCamera::beginRendering(ClearMode cmode) const
{
	if(!target) return false;
	if(!target->getDevice()->bindFramebuffer(target->getID()))
		return false;
	target->getDevice()->clear(cmode);
	if(backdrop)
		return backdrop->draw();
	return true;
}

bool NodeCamera::endRendering() const
{
	if(!target) return false;
	target->getDevice()->commitFramebuffer(target->getID());
	target->getDevice()->generateBufferMipmaps(target->getID());
	return true;
}
