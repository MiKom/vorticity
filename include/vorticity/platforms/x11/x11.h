/*
 * Copyright © 2012 Miłosz Kosobucki
 *
 * All rights reserved.
 *
 * This file is part of Vorticity.
 *
 * Vorticity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vorticity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vorticity.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __VORTICITY_X11_H
#define __VORTICITY_X11_H

#include <mutex>
#include <chrono>

#define PlatformBase Vorticity::X11_Application

#define VORTICITY_APPLICATION(app_class) \
	int main(int argc, char **argv) { \
	return Vorticity::X11_Application::main(argc, argv, new app_class); }

namespace Vorticity
{

class VAPI X11_Application : public Application
{
public:
	static const int ThreadSleep = 50;
protected:
	Display		*mDisplay;
	Window		mWindow;
	GLXContext	mGLXCtx;
	volatile bool 	mThreadTerminate;
	std::mutex	mMutex;

	/**
	  This variable keeps moment in which program started. It's set in
	  initialize()
	 */
	std::chrono::monotonic_clock::time_point mProgramStart;
protected:
	bool initGraphics();
	void destroyGraphics();

	/**
	  Looks through all the configs provided, and reurns index of the one
	  with most samples and sample buffers.
	  \param dpy X display to use
	  \param fb array of frame buffer configs
	  \param count number of items in fb array
	  \return index to fb array pointing to best config
	 */
	static size_t findBestFBConfig(Display* display,
				       GLXFBConfig *fbc,
				       int fbcount);
	static void initializeGLX();
	void updateProc();
public:
	X11_Application();
	virtual ~X11_Application();	

	bool	initialize();
	void	shutdown();

	int	run();
	void	showWindow(bool s);
	void	showMessage(const std::string& msg);
	void	showMessage(const std::string& title, const std::string& msg);
	void	die(const std::string& msg="", int retcode=1);
	void	die(const std::string& title, const std::string& msg, int retcode=1);

	time_t	getTimeResolution() const {
		//TODO:: implement
	}
	time_t	getTime() const;
	int	getClientWidth() const {
		//TODO: implement
	}
	int	getClientHeight() const {
		//TODO: implement
	}

	void	getMousePosition(int& x, int& y);
	bool	mouseButtonPressed(const int btn);
	bool	keyPressed(const char k);

	static int main(int argc, char **argv, Application *theApp);
};
} // Vorticity

#endif //
