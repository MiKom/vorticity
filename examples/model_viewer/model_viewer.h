#pragma once

namespace Vorticity {

class ModelViewer : public PlatformBase
{
protected:
	XNode *root;
	PostProcess *ppfx;
	int pixelBuffer;
	string mModelPath;

public:
	bool	onStartup(int argc, char **argv);
	bool	onInitialize();
	void	onShutdown();
	bool	onUpdate();
	void	onDraw();
};

} // Vorticity
