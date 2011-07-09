#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "object.h"
#include "node_light.h"

using namespace Vorticity;

NodeLight::NodeLight(const string& name, XNode *parent) : Object(name, parent)
{
	addAttrib(new Attribute<vec3>("ambient"));
	addAttrib(new Attribute<vec3>("diffuse")) = vec3(0.5f, 0.5f, 0.5f);
	addAttrib(new Attribute<vec3>("specular"));
}

NodeLight::~NodeLight()
{
}

void NodeLight::evaluate()
{
	vec3 localPosition = getAttrib("position");
	vec3 localRotation = getAttrib("rotation");

	vec4 outPosition = getWorldTM() * vector4(localPosition, 1.0f);
	vec4 outRotation = getWorldInvTranspose() * vector4(localRotation, 1.0f);

	worldPosition = vec3(outPosition.x, outPosition.y, outPosition.z);
	worldRotation = vec3(outRotation.x, outRotation.y, outRotation.z);
}

void NodeLight::setupShader(const int shader, Renderer *device, const int index)
{
	device->setShaderParameter(shader,
		&Attribute<vec3>("LightAmbient", getAttrib("ambient")), index);
	device->setShaderParameter(shader,
		&Attribute<vec3>("LightDiffuse", getAttrib("diffuse")), index);
	device->setShaderParameter(shader,
		&Attribute<vec3>("LightSpecular", getAttrib("specular")), index);
	device->setShaderParameter(shader,
		&Attribute<vec3>("LightPosition", worldPosition), index);
}