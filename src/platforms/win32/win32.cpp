#include "config.h"
#include "vorticity/render/opengl.h"
#include "vorticity/overlay/overlay.h"
#include "render/gloverlay.h"
#include "console.h"
#include "fontmanager.h"
#include <mmsystem.h>

using namespace Vorticity;

Win32_Application::Win32_Application() : Application(), clientWidth(0), clientHeight(0)
{
	std::memset(keys, 0, sizeof(keys));
	std::memset(mouseButtons, 0, sizeof(mouseButtons));
	std::memset(mousePosition, 0, sizeof(mousePosition));

	hInstance = GetModuleHandle(NULL);

	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	timeResolution = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);

	setConsole(new Win32_Console());
	getCore()->setApplication(this);
}

Win32_Application::~Win32_Application()
{
}

bool Win32_Application::initialize()
{
	if(!createWindow())
		throw std::runtime_error("Failed to create application's window.");
	if(!initGraphics(32, 24, 0))
		throw std::runtime_error("Failed initializing graphics API.");
	return true;
}

void Win32_Application::shutdown()
{
	onShutdown();
	destroyGraphics();
	destroyWindow();
}

LRESULT CALLBACK Win32_Application::staticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Win32_Application *thisApp;
	if(uMsg == WM_CREATE)
	{
		thisApp = (Win32_Application*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)thisApp);
	}
	else
		thisApp = (Win32_Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if(thisApp == NULL)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	return thisApp->windowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Win32_Application::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		if(device)
		{
			device->setScreenWidth(LOWORD(lParam));
			device->setScreenHeight(HIWORD(lParam));
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		keys[LOWORD(wParam)] = true;
		break;
	case WM_KEYDOWN:
		keys[LOWORD(wParam)] = false;
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		mousePosition[0] = LOWORD(lParam);
		mousePosition[1] = HIWORD(lParam);
		mouseButtons[MouseLeft]    = wParam&MK_LBUTTON?true:false;
		mouseButtons[MouseMiddle]  = wParam&MK_MBUTTON?true:false;
		mouseButtons[MouseRight]   = wParam&MK_RBUTTON?true:false;
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

bool Win32_Application::createWindow(const int style, HWND hParent, WNDPROC proc, void *extra)
{
	WNDCLASSEX	wndClass;
	DWORD dwStyle = style;
	int windowPosX, windowPosY;

	if(fullscreen)
		dwStyle = WS_POPUP;
	else if(dwStyle == 0)
		dwStyle = WS_SYSMENU;

	memset(&wndClass, 0, sizeof(WNDCLASSEX));
	wndClass.cbSize			= sizeof(WNDCLASSEX);
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance		= hInstance;
	wndClass.lpszClassName	= VORTICITY_NAME;
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;

	if(!proc)
		wndClass.lpfnWndProc = staticWindowProc;
	else
		wndClass.lpfnWndProc = proc;

	if(!RegisterClassEx(&wndClass))
		return false;

	if(fullscreen)
	{
		RECT desktopRect;
		GetWindowRect(GetDesktopWindow(), &desktopRect);
		width  = desktopRect.right;
		height = desktopRect.bottom;
		windowPosX = 0;
		windowPosY = 0;
	}
	else
	{
		windowPosX = 50;
		windowPosY = 50;
	}

	hWindow = CreateWindowEx(0, VORTICITY_NAME, windowTitle.c_str(), dwStyle,
		windowPosX, windowPosY,
		width, height, hParent, NULL, hInstance, extra?extra:this);
	if(hWindow == NULL)
	{
		UnregisterClass(VORTICITY_NAME,	hInstance);
		return false;
	}

	RECT rect;
	GetClientRect(hWindow, &rect);
	clientWidth	 = rect.right;
	clientHeight = rect.bottom;
	return true;
}

void Win32_Application::showWindow(bool s)
{
	if(s)
	{
		ShowWindow(hWindow, SW_SHOW);
		UpdateWindow(hWindow);
	}
	else
		ShowWindow(hWindow, SW_HIDE);
}

void Win32_Application::destroyWindow()
{
	ShowWindow(hWindow, SW_HIDE);
	DestroyWindow(hWindow);
	UnregisterClass(VORTICITY_NAME, hInstance);
}

bool Win32_Application::initGraphics(const int bpp, const int depth, const int stencil)
{
	PIXELFORMATDESCRIPTOR	pfd;
	int	pixelFormat;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.cColorBits	= bpp;
	pfd.cDepthBits	= depth;
	pfd.cStencilBits= stencil;

	hDC = GetDC(hWindow);
	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if(!SetPixelFormat(hDC, pixelFormat, &pfd))
		return false;

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	setDevice(new OpenGL());
	setFontManager(new Win32_FontManager(hDC));

	device->setScreenWidth(clientWidth);
	device->setScreenHeight(clientHeight);
	device->setOverlay(new OverlayOpenGL(fontManager));

	// ATI sucks big time
	if(strstr(device->getVendorString(), "ATI Technologies") != NULL)
		threaded = false;
	return true;
}

void Win32_Application::destroyGraphics()
{
	delete device;
	device = NULL;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWindow, hDC);
}

DWORD Win32_Application::updateProc(LPVOID lpParameter)
{
	Win32_Application* static_this = (Win32_Application*)lpParameter;
	time_t timerSleep = 1000 / static_this->updateFrequency;

	bool updateStatus = true;
	time_t dwTimeBegin, dwTimeElapsed;
	do
	{	
		dwTimeBegin = static_this->getTime();
		EnterCriticalSection(&static_this->threadDrawLock);
		updateStatus = static_this->onUpdate();
		LeaveCriticalSection(&static_this->threadDrawLock);
		dwTimeElapsed = static_this->getTime() - dwTimeBegin;

		if(timerSleep > dwTimeElapsed)
		{
			timeBeginPeriod((UINT)static_this->getTimeResolution());
			Sleep((DWORD)(timerSleep - dwTimeElapsed));
			timeEndPeriod((UINT)static_this->getTimeResolution());
		}
	} while(static_this->threaded && (!static_this->threadTerminate && updateStatus));
	if(static_this->threaded)
		PostQuitMessage(0);
	return 0;
}

int Win32_Application::run()
{
	MSG	msg;
	HANDLE hUpdateThread;

	if(threaded)
	{
		hUpdateThread = CreateThread(NULL, 0,
			Win32_Application::updateProc, this, CREATE_SUSPENDED, NULL);
		if(hUpdateThread == (HANDLE)-1)
			return 1;

		threadTerminate = false;
		InitializeCriticalSection(&threadDrawLock);
		ResumeThread(hUpdateThread);
		Sleep(Win32_Application::ThreadSleep);
	}
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if(threaded)
			EnterCriticalSection(&threadDrawLock);
		else
			updateProc(this);

		onDraw();
		getCore()->getTimer().update();
		if(threaded)
			LeaveCriticalSection(&threadDrawLock);

		SwapBuffers(hDC);
	}
	if(threaded)
	{
		threadTerminate = true;
		WaitForSingleObject(hUpdateThread, -1);
		DeleteCriticalSection(&threadDrawLock);
	}
	return msg.wParam;
}

int Win32_Application::main(HINSTANCE hInstance, LPSTR lpCmdLine, Application *theApp)
{
	int	  argc = 1;
	char* argv[Win32_Application::MaxArgv];
	char  moduleName[MAX_PATH], *token;

	GetModuleFileName(NULL, moduleName, MAX_PATH);
	argv[0]= moduleName;
	token  = strtok(lpCmdLine, " ");
	while(token)
	{
		argv[argc++] = token;
		token = strtok(NULL, " ");
	}

	try {
		if(!theApp->onStartup(argc, argv))
			throw std::exception();
		theApp->initialize();
		if(!theApp->onInitialize())
			throw std::exception();
	}
	catch(const std::exception& e)
	{
		theApp->showMessage(e.what());
		delete theApp;
		return 1;
	}

	int retcode = theApp->run();
	theApp->shutdown();
	delete theApp;
	return retcode;
}

time_t Win32_Application::getTime() const
{ 
	timeBeginPeriod((UINT)timeResolution);
	time_t r = (time_t)timeGetTime();
	timeEndPeriod((UINT)timeResolution);
	return r;
}

void Win32_Application::showMessage(const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), VORTICITY_NAME, MB_ICONEXCLAMATION | MB_OK);
}

void Win32_Application::showMessage(const std::string& title, const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_ICONEXCLAMATION | MB_OK);
}

void Win32_Application::die(const std::string& msg, int retcode)
{
	if(msg.length() > 0)
		showMessage(msg);
	ExitProcess((unsigned int)retcode);
}

void Win32_Application::die(const std::string& title, const std::string& msg, int retcode)
{
	showMessage(title, msg);
	ExitProcess((unsigned int)retcode);
}

bool Win32_Application::keyPressed(const char k)
{ return keys[k]; }

bool Win32_Application::mouseButtonPressed(const int btn)
{ return mouseButtons[btn]; }

void Win32_Application::getMousePosition(int &x, int &y)
{
	x = mousePosition[0];
	y = mousePosition[1];
}
