/* 
 * Copyright © 2009, 2010 Micha³ Siejak
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

#ifndef __VORTICITY_APPLICATION_H
#define __VORTICITY_APPLICATION_H

namespace Vorticity {

class Console : public std::streambuf 
{
protected:
	static const std::string defaultTitle;
public:
	virtual bool open(const std::string& title="") = 0;
	virtual void close() = 0;
};

class FontManager;
class VAPI Application
{
protected:
	Renderer*	 device;
	Console*	 console;
	FontManager* fontManager;

	bool		threaded;
	bool		fullscreen;

	string		windowTitle;
	int			width, height;
	int			updateFrequency;
protected:
	void	setDevice(Renderer *dev)		{ device = dev;  	}
	void	setConsole(Console *con)		{ console = con; 	}
	void	setFontManager(FontManager *fm) { fontManager = fm; }
public:
	Application();
	virtual ~Application();

	virtual bool	initialize() = 0;
	virtual void	shutdown() = 0;

	virtual bool	onStartup(int argc, char** argv) { return false; }
	virtual bool	onInitialize()		{ return false; }
	virtual bool	onUpdate()			{ return false; }
	virtual void	onDraw()			{ }
	virtual void	onShutdown()		{ }
	
	virtual int		run()				= 0;
	virtual void	showWindow(bool s)	= 0;
	virtual void	showMessage(const std::string& msg) = 0;
	virtual void	showMessage(const std::string& title, const std::string& msg) = 0;
	virtual void	die(const std::string& msg="", int retcode=1) = 0;
	virtual void	die(const std::string& title, const std::string& msg, int retcode=1) = 0;

	virtual time_t	getTimeResolution() const = 0;
	virtual time_t	getTime() const		= 0;
	virtual int getClientWidth() const	= 0;
	virtual int getClientHeight() const	= 0;

	virtual void getMousePosition(int& x, int& y) = 0;
	virtual bool mouseButtonPressed(const int btn) = 0;
	virtual bool keyPressed(const char k) = 0;

	int		getWidth() const			{ return width;			}
	int		getHeight() const			{ return height;		}
	void	setWidth(const int w)		{ width		= w;		}
	void	setHeight(const int h)		{ height	= h;		}
	void	setFullscreen(bool fs)		{ fullscreen= fs;		}
	bool	isFullscreen() const		{ return fullscreen;	}

	bool	isThreaded() const			{ return threaded;		}

	int		getUpdateFrequency() const		{ return updateFrequency;	}
	void	setUpdateFrequency(const int u)	{ updateFrequency = u;	 	} 

	void	setTitle(const string t)		{ windowTitle = t;			}
	string	getTitle() const				{ return windowTitle;		}

	Renderer* 		getDevice()				{ return device;			}
	Console*  		getConsole()			{ return console;			}
	FontManager*	getFontManager()		{ return fontManager;		}
	
	enum
	{
		MouseLeft,
		MouseMiddle,
		MouseRight,

		KeyEnter	= 0x0D,
		KeySpace 	= 0x20,
		KeyEscape 	= 0x1B,
		KeyShift	= 0x10,
		KeyControl  = 0x11,
		KeyUp 		= 0x26,
		KeyDown 	= 0x28,
		KeyLeft 	= 0x25,
		KeyRight 	= 0x27,
	};
};

} // Vorticity

#endif