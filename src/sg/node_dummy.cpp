#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "object.h"
#include "node_dummy.h"

#include <gl/glut.h>
#pragma comment(lib, "glut32.lib")

using namespace Vorticity;

NodeDummy::NodeDummy(const string& name, XNode* parent) : Object(name, parent)
{
	addAttrib(new Attribute<float>("radius")).setValue(1.0f);
}

void NodeDummy::evaluate()
{
}

void NodeDummy::render(Renderer *device)
{
	float radius;
	getAttrib("radius").getValue(radius);
	device->drawBox(radius, radius, radius);
}