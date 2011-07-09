#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "modifier.h"
#include "node_shader.h"
#include "resources/resource.h"
#include "resources/shader.h"

using namespace Vorticity;

NodeShader::NodeShader(const string& name, XNode* parent) : Modifier(name, parent)
{
	shader = NULL;
}

NodeShader::~NodeShader()
{
}

bool NodeShader::bind(Resource *shader)
{
	if(shader->getType() != "shader")
		return false;
	if(!shader->getDevice())
		return false;
	if(!shader->getDevice()->exportShaderParameters(shader->getID(), this))
		return false;
	this->shader = (Shader*)shader;
	return true;
}

void NodeShader::evaluate()
{
	if(!shader) return;
	if(!shader->getDevice()) return;

	AttributeIterator it = enumAttributes(0);
	while(AnyAttribute* attr = getAttrib(it))
		shader->getDevice()->setShaderParameter(shader->getID(), attr);
}
