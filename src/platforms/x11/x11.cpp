#include "config.cpp"
#include <vorticity/platforms/x11/x11.h>

#include <X11/Xlib.h>

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
	//TODO: implement
	return true;
}

void X11_Application::shutdown()
{
	//TODO: implement
}

bool X11_Application::initializeGraphics()
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


}

int X11_Application::main(int argc, char **argv, Application *theApp)
{
	int retcode = 0;

	return retcode;
}


void X11_Application::showWindow(bool s)
{
	//TODO: implement
}

int X11_Application::run()
{
	//TODO: implement
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
