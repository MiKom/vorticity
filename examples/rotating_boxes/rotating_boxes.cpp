#include "config.h"
#include "vorticity/vorticity.h"
#include "rotating_boxes.h"

using namespace Vorticity;

VORTICITY_APPLICATION(Sandbox)

bool Sandbox::onStartup(int argc, char **argv)
{
	setTitle("Vorticity Engine");
	setWidth(1024); setHeight(768);
	setUpdateFrequency(50);
	//setFullscreen(true);

#ifdef _DEBUG
	getConsole()->open();
#endif
	return true;
}

NodeLC3 *lc3;

bool Sandbox::onInitialize()
{
	log() << "Arsen Project [Vorticity Engine]" << std::endl;

	// Resources
	Shader 	*phong_shader, *debug_shader;

	try 
	{
		TextLoader lambertLoader("shaders/lambert.cgfx");
		phong_shader = new Shader(lambertLoader);
		TextLoader debugLoader("shaders/debug.cgfx");
		debug_shader = new Shader(debugLoader);
	} 
	catch (IOException& e) {
		showMessage(e.getMessage());
	}
	catch (DeviceException& e) {
		log() << device->getCompilationLog() << std::endl;
		showMessage(e.what());
	}

	// Render targets
	int frameBuffer = device->createFramebuffer(Vorticity::BufferFloat16, 4);
	device->addBufferTarget(frameBuffer, Vorticity::ColorBuffer, NoFilter);

	font = getFontManager()->createFont("Tahoma");

	// Scene graph
	root = new Group("root");
	NodeCamera &camera = gnew <NodeCamera>("cam1", root);
	camera.bind(new RenderAsset(frameBuffer));
	camera["zfar"] = 1000.0f;
	camera["position"] = vec3(0.0f, 0.0f, 20.0f);
	camera.updateTransformation();

	NodeShader &phong = gnew <NodeShader>("phong", root);
	phong.bind(phong_shader);
	phong.connectTo(root);

	// Default material
	NodeMaterial &material = gnew <NodeMaterial>("material1", root);
	material["specular"] = vec3(1.0f, 1.0f, 1.0f);
	material.connectTo(root);

	NodeDummy &dum1 = gnew <NodeDummy>("dum1", root);
	dum1["position"] = vec3(0.0f, 0.0f, 0.0f);
	dum1.updateTransformation();

	NodeDummy &dum2 = gnew <NodeDummy>("dum2", dum1);
	dum2["position"] = vec3(5.0f, 0.0f, 0.0f);
	dum2["scale"] = vec3(0.5f, 0.5f, 0.5f);
	dum2.updateTransformation();

	// Default light
	NodeLight &light1 = gnew <NodeLight>("light1", root);
	light1["diffuse"] = vec3(1.0f, 1.0f, 1.0f);
	light1["specular"] = vec3(1.0f, 1.0f, 1.0f);
	light1["position"] = vec3(0.0f, 0.0f, 1.0f);

	//Proxy object for light
	Proxy &proxy = gnew <Proxy>("lproxy", root);
	proxy.attachNode(&light1);
	proxy.connectTo(root);

	// Rotation
	NodeLC3& rot_lc = gnew <NodeLC3>("rot_lc", root);
	rot_lc.setLoop(true);
	rot_lc.addKey(0.0f, vec3(0.0f, 0.0f, 0.0f));
	rot_lc.addKey(5.0f, vec3(360.0f, 360.0f, 360.0f));
	rot_lc.connectTo(&dum1, "rotation");
	lc3 = &rot_lc;

	ppfx = new PostProcessSimple(device);
	TextLoader shaderLoader("shaders/simple.cgfx");
	ppfx->addShader("blit", new Shader(shaderLoader));
	ppfx->addSource("color", &camera);
	ppfx->validate();

	getCore()->getTimer()->start();
	showWindow(true);
	return true;
}

void Sandbox::onShutdown()
{
	delete root;
	getFontManager()->destroyFont(font);
	getConsole()->close();
}

bool Sandbox::onUpdate()
{
	EvaluateVisitor().traverse(root);
	return true;
}

void Sandbox::onDraw()
{
	device->viewport(0, 0, device->getScreenWidth(), device->getScreenHeight());
	device->clear(Vorticity::ClearColorDepth);

	NodeCamera *camera = (NodeCamera*)root->getChild("cam1");
	
	RenderVisitor(camera).traverse(root);

	int frame = ppfx->render();
	device->drawFeedbackBuffer(frame);

	Overlay* overlay = device->getOverlay();
	overlay->begin(8.0f, 6.0f);
	overlay->bg(0.2f, 0.2f, 0.2f).usebg(true).margins(0.2f, 0.2f).bgalpha(0.5f).text(font, 0.1f, 0.2f, 0.2f, "Vorticity");
	overlay->end();
}
