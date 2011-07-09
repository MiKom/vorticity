#ifndef __VORTICITY_PPFX_SIMPLE_H
#define __VORTICITY_PPFX_SIMPLE_H

namespace Vorticity {

class VAPI PostProcessSimple : public PostProcess
{
protected:
	NodeCamera *srcColor;
	Shader *blitShader;
	int fb;
protected:
	virtual bool acceptSource(const string& name);
	virtual bool acceptShader(const string& name);
public:
	PostProcessSimple(Renderer *device=NULL);
	virtual ~PostProcessSimple();

	virtual string getType() const
	{ return "simple"; }

	virtual int		render();
	virtual bool	validate();
};

} // Vorticity

#endif