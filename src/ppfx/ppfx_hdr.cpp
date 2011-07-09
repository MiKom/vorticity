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
#include "ppfx_hdr.h"

using namespace Vorticity;

PostProcessHDR::PostProcessHDR(Renderer *device) : PostProcess(device),
				fb(0), fb_downsampled(0), w(0), h(0),
				srcColor(NULL), blurShader(NULL), extractShader(NULL), tonemapShader(NULL)
{ }

PostProcessHDR::~PostProcessHDR()
{ }

bool PostProcessHDR::acceptShader(const string& name)
{
	if(name == "blur" || name == "extract" || name == "tonemap")
		return true;
	return false;
}

bool PostProcessHDR::acceptSource(const string& name)
{ return name == "color"; }

bool PostProcessHDR::validate()
{
	srcColor = getCheckSource("color");
	blurShader = getCheckShader("blur");
	extractShader = getCheckShader("extract");
	tonemapShader = getCheckShader("tonemap");

	if(!srcColor || !blurShader || !extractShader || !tonemapShader)
		return false;

	device->getFramebufferProperties(srcColor->getTargetBuffer()->getID(),
		&w, &h, NULL);
	if(!validateBuffer(fb, Vorticity::BufferRGBA, srcColor))
		return false;
	if(!validateBuffer(fb_downsampled, Vorticity::BufferFloat16, w/2, h/2))
		return false;
	return true;
}

int PostProcessHDR::render()
{
	int color0	= srcColor->getTargetBuffer()->getID();
	int blur	= blurShader->getID();
	int extract	= extractShader->getID();
	int tonemap = tonemapShader->getID();
	int pass	= 0;

	device->setOrthoProjection(0.0f, 1.0f, 0.0f, 1.0f);
	setShaderParameters(blurShader);
	setShaderParameters(extractShader);
	setShaderParameters(tonemapShader);

	device->setShaderParameterBySemantic(extractShader->getID(),
		&Attribute<int>("V_COLOR0_WIDTH", w));
	device->setShaderParameterBySemantic(extractShader->getID(),
		&Attribute<int>("V_COLOR0_HEIGHT", h));

	ShaderPass p;
	int target;

	// Pass 1: Extract bright parts and downsample to 1/4 size
	setSampler(extract, "V_COLOR0", device->getBufferTarget(color0, ColorBuffer));
	p = device->firstShaderPass(extract);
	beginBuffer(fb_downsampled, extract);
	device->drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
	device->resetShaderPass(p);
	
	// Pass 2: Multipass Gaussian blur of downsampled image
	p = NULL;
	while(p = device->nextShaderPass(blur, p, false))
	{
		target = swapBuffer(fb_downsampled, blur);
		setSampler(blur, "V_COLOR0", target);
		device->setShaderPass(p);
		device->drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
	}
	target = swapBuffer(fb_downsampled);

	// Pass 3: Combine downsampled image with original and do tonemapping
	// to convert from floating point colorspace back to RGBA.
	setSampler(tonemap, "V_COLOR0", device->getBufferTarget(color0, ColorBuffer));
	setSampler(tonemap, "V_COLOR1", target);
	p = device->firstShaderPass(tonemap);
	beginBuffer(fb, tonemap);
	device->drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
	device->resetShaderPass(p);
	endBuffer();
	return fb;
}