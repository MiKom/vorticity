#ifndef __VORTICITY_WIN32_H
#define __VORTICITY_WIN32_H

#define PlatformBase	Vorticity::Win32_Application

#define VORTICITY_APPLICATION(app_class) \
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int) { \
	return Vorticity::Win32_Application::main(hInstance, lpCmdLine, new app_class); }

namespace Vorticity {

class VAPI Win32_Application : public Application
{
public:
	static const int MaxArgv = 100;
	static const int ThreadSleep = 50;
private:
	bool		keys[256];
	bool		mouseButtons[3];
	int			mousePosition[2];
protected:
	HWND		hWindow;
	HINSTANCE	hInstance; 
	HDC			hDC;
	HGLRC		hRC;
	int			clientWidth, clientHeight;
	time_t		timeResolution;

	volatile bool	 threadTerminate;
	CRITICAL_SECTION threadDrawLock;
protected:
	static DWORD WINAPI updateProc(LPVOID lpParameter);
	static LRESULT CALLBACK staticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	Win32_Application();
	virtual ~Win32_Application();

	bool	initialize();
	void	shutdown();

	bool	createWindow(const int style=0, HWND hParent=NULL, WNDPROC proc=NULL, void *extra=NULL);
	bool	initGraphics(const int bpp, const int depth, const int stencil);
	void	destroyGraphics();
	void	destroyWindow();

	time_t	getTimeResolution() const
	{ return timeResolution; }

	time_t	getTime() const;

	HWND	getHWND() const	{ return hWindow;	}
	HDC		getHDC()  const	{ return hDC;		}

	int		getClientWidth() const
	{ return clientWidth;	}
	int		getClientHeight() const
	{ return clientHeight;	}

	void	getMousePosition(int& x, int& y);
	bool	mouseButtonPressed(const int btn);
	bool	keyPressed(const char k);

	int		run();
	void	showWindow(bool s);
	void	showMessage(const std::string& msg);
	void	showMessage(const std::string& title, const std::string& msg);
	void	die(const std::string& msg="", int retcode=1);
	void	die(const std::string& title, const std::string& msg, int retcode=1);

	static int	main(HINSTANCE hInstance, LPSTR lpCmdLine, Application *theApp);
};

}

#endif