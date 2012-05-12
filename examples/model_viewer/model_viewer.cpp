#include "config.h"
#include <vorticity/vorticity.h>
#include "model_viewer.h"

#include <sstream>
#include <string>
using namespace Vorticity;

VORTICITY_APPLICATION(ModelViewer)

bool ModelViewer::onStartup(int argc, char **argv)
{
	setTitle("Vorticity Engine");
	setWidth(1024); setHeight(768);
	setUpdateFrequency(50);
	//setFullscreen(true);
#ifdef _DEBUG
	getConsole()->open();
#endif
	if(argc != 2) {
		std::ostringstream os;
		os << std::endl << "Usage:" << std::endl << argv[0] << " [model file]"
		   << std::endl;
		showMessage(os.str());
		return false;
	} else {
		mModelPath = string(argv[1]);
	}
	return true;
}

NodeLC3 *lc3;

bool ModelViewer::onInitialize()
{
	log() << "Model Viewer [Vorticity Engine]" << std::endl;
	
	device->clearColor(0.1f, 0.2f, 0.3f, 1.0f);

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

	// Scene graph
	root = new Group("root");
	NodeCamera &camera = gnew <NodeCamera>("cam1", root);
	camera.bind(new RenderAsset(frameBuffer));
	camera["zfar"] = 1000.0f;
	camera["position"] = vec3(0.0f, 3.0f, 20.0f);
	camera.updateTransformation();

	NodeShader &phong = gnew <NodeShader>("phong", root);
	phong.bind(phong_shader);
	phong.connectTo(root);

	// Default material
	NodeMaterial &material = gnew <NodeMaterial>("material1", root);
	material["specular"] = vec3(1.0f, 1.0f, 1.0f);
	material.connectTo(root);
	
	StdMeshLoader meshLoader(mModelPath);
	Mesh* mesh = new Mesh(meshLoader);
	NodeMesh &nodeMesh = gnew <NodeMesh>("logo", root);
	nodeMesh["position"] = vec3(0.0f, 0.0f, 0.0f);
	nodeMesh.bind(mesh);
	

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
	rot_lc.addKey(5.0f, vec3(0.0f, 360.0f, 0.0f));
	rot_lc.connectTo(&nodeMesh, "rotation");
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

void ModelViewer::onShutdown()
{
	delete root;
	getConsole()->close();
}

bool ModelViewer::onUpdate()
{
	EvaluateVisitor().traverse(root);
	return true;
}

void ModelViewer::onDraw()
{
	device->viewport(0, 0, device->getScreenWidth(), device->getScreenHeight());
	device->clear(Vorticity::ClearColorDepth);

	NodeCamera *camera = (NodeCamera*)root->getChild("cam1");
	
	RenderVisitor(camera).traverse(root);

	int frame = ppfx->render();
	device->drawFeedbackBuffer(frame);
}
