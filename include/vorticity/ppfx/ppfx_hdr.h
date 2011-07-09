#ifndef __VORTICITY_PPFX_HDR_H
#define __VORTICITY_PPFX_HDR_H

namespace Vorticity {

class VAPI PostProcessHDR : public PostProcess
{
protected:
	NodeCamera *srcColor;
	Shader *blurShader;
	Shader *extractShader;
	Shader *tonemapShader;
	int fb, fb_downsampled;
	int w,h;
protected:
	virtual bool acceptSource(const string& name);
	virtual bool acceptShader(const string& name);
public:
	PostProcessHDR(Renderer *device=NULL);
	virtual ~PostProcessHDR();

	virtual string getType() const
	{ return "hdr"; }

	virtual int render();
	virtual bool validate();
};

} // Vorticity

#endif