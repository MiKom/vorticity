#include "config.cpp"
#include "vorticity/platforms/x11/x11.h"
#include "vorticity/render/opengl.h"

#include <mutex>
#include <thread>

using namespace Vorticity;

X11_Application::X11_Application() : Application()
{
	getCore()->setApplication(this);
}

X11_Application::~X11_Application()
{
}

bool X11_Application::initialize()
{
	if(!initGraphics()) {
		throw std::runtime_error("Failed to create GLX window");
	}
	return true;
}

void X11_Application::shutdown()
{
	onShutdown();
	destroyGraphics();
}

bool X11_Application::initGraphics()
{
	mDisplay = XOpenDisplay(NULL);
	static int visual_attribs[] = {
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		//GLX_SAMPLE_BUFFERS  , 1,
		//GLX_SAMPLES         , 4,
		None
	};

	int glx_minor, glx_major;

	if (!glXQueryVersion( mDisplay, &glx_major, &glx_minor ) ||
	    ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
	{
		throw std::runtime_error( "Invalid GLX version" );
	}

	int fbcount;
	GLXFBConfig *fbc = glXChooseFBConfig(mDisplay, DefaultScreen(mDisplay),
					     visual_attribs, &fbcount);

	size_t bestFBConfigIdx = findBestFBConfig(mDisplay, fbc, fbcount);
	GLXFBConfig bestFbc = fbc[bestFBConfigIdx];
	XFree(fbc);

	XVisualInfo *vi = glXGetVisualFromFBConfig(mDisplay, bestFbc);
	XSetWindowAttributes wAttr;
	Colormap cMap;

	wAttr.colormap = XCreateColormap(mDisplay,
					 RootWindow(mDisplay, vi->screen),
					 vi->visual, AllocNone);
	wAttr.background_pixmap = None;
	wAttr.border_pixel = 0;
	wAttr.event_mask = StructureNotifyMask;

	//TODO: put initial windows position somewhere else.
	mWindow = XCreateWindow(mDisplay, RootWindow(mDisplay, vi->screen),
				50, 50, width, height, 0, vi->depth, InputOutput,
				vi->visual,
				CWBorderPixel | CWColormap | CWEventMask,
				&wAttr);
	XFree(vi);
	if(!mWindow) {
		return false;
	}
	XStoreName(mDisplay, mWindow, windowTitle.c_str());

	int contextAttribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
#ifdef OPENGL_COMPATIBILITY_RENDERER
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
#else
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB
#endif
	};
	mGLXCtx = glXCreateContextAttribsARB(mDisplay, bestFbc, 0, True,
					     contextAttribs);
	if(!mGLXCtx) {
		return false;
	}

	setDevice(new OpenGL());

	//TODO: uncomment after implementing respective classes
	//setFintManager(new X11_FontManager());
	//device->setScreenWidth();
	//device->setScreenHeight();
	//device->setOverlay(new OverlayOpenGL(fontManager));

	return true;
}

void X11_Application::destroyGraphics()
{
	delete device;
	device = NULL;

	glXMakeCurrent(mDisplay, 0, 0);
	glXDestroyContext(mDisplay, mGLXCtx);
	XUnmapWindow(mDisplay, mWindow);
	XDestroyWindow(mDisplay, mWindow);
	XCloseDisplay(mDisplay);
}

size_t
X11_Application::findBestFBConfig(
		Display* display,
		GLXFBConfig *fbc,
		int fbcount)
{
	int bestFbc = -1, worstFbc = -1, bestNumSamp = -1, worstNumSamp = 999;
	int i;
	for ( i = 0; i < fbcount; i++ ) {
		XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );
		if ( vi ){
			int sampBuf, samples;
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &sampBuf );
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );

			if ( bestFbc < 0 || sampBuf && samples > bestNumSamp ) {
				bestFbc = i, bestNumSamp = samples;
			}
			if ( worstFbc < 0 || !sampBuf || samples < worstNumSamp ) {
				worstFbc = i, worstNumSamp = samples;
			}
		}
		XFree( vi );
	}
	return bestFbc;
}

int X11_Application::main(int argc, char **argv, Application *theApp)
{
	int retcode = 0;
	//TODO: implement
	return retcode;
}


//TODO: Don't know if it should work that way
void X11_Application::showWindow(bool s)
{
	if(s) {
		XMapWindow(mDisplay, mWindow);
		glXMakeCurrent(mDisplay, mWindow, mGLXCtx);
	} else {
		glXMakeCurrent(mDisplay, 0, 0);
		XUnmapWindow(mDisplay, mWindow);
	}
}

void X11_Application::updateProc()
{
	//TODO: implement
	return;
}
int X11_Application::run()
{
	std::thread updateThread;
	if(threaded) {
		updateThread = std::thread(&X11_Application::updateProc, this);
	}
	while(true) {
		if(threaded) {
			mMutex.lock();
		} else {
			updateProc();
		}
		onDraw();
		getCore()->getTimer()->update();
		if(threaded) {
			mMutex.unlock();
		}
		glXSwapBuffers(mDisplay, mWindow);
	}
	if(threaded){
		mThreadTerminate = true;
		updateThread.join();
	}
	return 0;
}

time_t X11_Application::getTime() const
{
	//TODO: implement
	return 0;
}

void X11_Application::showMessage(const std::string &msg)
{
	//TODO: implement
}

void X11_Application::showMessage(const std::string& title, const std::string& msg)
{
	//TODO: implement
}

void X11_Application::die(const std::string &msg, int retcode)
{
	if(msg.length() > 0) {
		showMessage(msg);
	}
	exit(retcode);
}

void X11_Application::die(const std::string& title, const std::string& msg, int retcode)
{
	showMessage(title, msg);
	exit(retcode);
}

bool X11_Application::keyPressed(const char k)
{
	//TODO: implement
	return false;
}

bool X11_Application::mouseButtonPressed(const int btn)
{
	//TODO: implement
	return false;
}

void X11_Application::getMousePosition(int &x, int &y)
{
	//TODO: implement
	x = 0;
	y = 0;
}
