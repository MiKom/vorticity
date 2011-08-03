#ifndef __VORTICITY_SANDBOX_H
#define __VORTICITY_SANDBOX_H

namespace Vorticity {

class Sandbox : public PlatformBase
{
protected:
	XNode *root;
	PostProcess *ppfx;
	Font* font;
	int pixelBuffer;

public:
	bool	onStartup(int argc, char **argv);
	bool	onInitialize();
	void	onShutdown();
	bool	onUpdate();
	void	onDraw();
};

} // Vorticity

#endif