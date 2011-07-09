#include "config.h"
#include "core/renderer.h"
#include "resources/resource.h"
#include "resources/shader.h"
#include "sg/attribute.h"
#include "sg/node.h"
#include "sg/xnode.h"
#include "sg/object.h"
#include "sg/node_ppfx.h"
#include "sg/node_camera.h"
#include "ppfx.h"
#include "ppfx_simple.h"

using namespace Vorticity;

PostProcessSimple::PostProcessSimple(Renderer *device) : PostProcess(device), fb(0)
{ }

PostProcessSimple::~PostProcessSimple()
{
	if(fb && device)
		device->destroyFeedbackBuffer(fb);
}

bool PostProcessSimple::acceptShader(const string& name)
{ return name == "blit"; }

bool PostProcessSimple::acceptSource(const string& name)
{ return name == "color"; }

int PostProcessSimple::render()
{
	int color = srcColor->getTargetBuffer()->getID();
	int shader = blitShader->getID();
	int pass   = 0;

	device->setOrthoProjection(0.0f, 1.0f, 0.0f, 1.0f);
	setShaderParameters(blitShader);

	// First pass
	setSampler(shader, "V_COLOR0", device->getBufferTarget(color, ColorBuffer));
	ShaderPass p = device->firstShaderPass(shader);
	beginBuffer(fb, shader);
	do
	{
		// Next Pass
		if(pass++ > 0) swapBuffer(fb);
		device->drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
		setSampler(shader, "V_COLOR0", device->getFeedbackBufferTarget(fb));
	} while(p = device->nextShaderPass(p));
	endBuffer();
	return fb;
}

bool PostProcessSimple::validate()
{
	srcColor = getCheckSource("color");
	blitShader = getCheckShader("blit");

	if(!srcColor || !blitShader)
		return false;

	return validateBuffer(fb, BufferRGBA, srcColor);
}