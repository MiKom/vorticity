#include "config.h"
#include "core/renderer.h"
#include "sg/attribute.h"
#include "resource.h"
#include "shader.h"
#include "backdrop.h"

using namespace Vorticity;

Backdrop::Backdrop(const int target, Shader* shader) 
	: Resource(), shader(shader)
{
	target_w = target_h = 0;
	setID(target);
	setTarget(target);
}

void Backdrop::setTarget(const int target)
{
	this->target = target;
	device->getTextureProperties(target, &target_w, &target_h);
}

bool Backdrop::draw()
{
	if(!device->bindTexture(target, 0))
		return false;

	float savedProjection[16];
	std::memcpy(savedProjection, device->getProjection(), 16*sizeof(float));
	device->setOrthoProjection(-1.0f, 1.0f, -1.0f, 1.0f);

	if(shader)
	{
		int shaderID = shader->getID();
		device->setShaderMatrix(shaderID, ModelviewMatrix, "V_MODELVIEW");
		device->setShaderMatrix(shaderID, ProjectionMatrix, "V_PROJECTION");
		device->setShaderMatrix(shaderID, ModelviewProjMatrix, "V_MVP");
		device->setShaderMatrix(shaderID, NormalMatrix, "V_MVIT");

		device->setShaderParameterBySemantic(shaderID, &Attribute<int>("V_WIDTH", target_w));
		device->setShaderParameterBySemantic(shaderID, &Attribute<int>("V_HEIGHT", target_h));
		device->setShaderSamplerBySemantic(shaderID, "V_COLOR0", target);

		ShaderPass pass = device->firstShaderPass(shader->getID());
		do {
			device->drawQuad(-1.0f, 1.0f, 1.0f, -1.0f);
		} while(pass = device->nextShaderPass(shader->getID(), pass));
	}
	else
		device->drawQuad(-1.0f, 1.0f, 1.0f, -1.0f);

	device->projectionTransform(savedProjection);
	device->bindTexture(0, 0);
	return true;
}
