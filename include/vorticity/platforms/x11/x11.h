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

#define PlatformBase Vorticity::X11_Application

#define VORTICITY_APPLICATION(app_clas) \
	int main(int argc, char **argv) { \
	return Vorticity::X11_Application::main(argc, argv, new app_class); }

namespace Vorticity
{

class VAPI X11_Application : public Application
{
public:
	bool	initialize();
	void	shutdown();

	int	run();
	void	showWindow();
	void	showMessage(const std::string& msg);
	void	showMessage(const std::string& title, const std::string& msg);
	void	die(const std::string& msg="", int retcode=1);
	void	die(const std::string& title, const std::string& msg, int retcode=1);

	time_t	getTimeResolution() const;
	time_t	getTime() const;
	int	getClientWidth() const;
	int	getClientHeight() const;

	void	getMousePosition(int& x, int& y);
	bool	mouseButtonPressed(const int btn);
	bool	keyPressed(const char k);

	int	main(int argc, char **argv);
};
} // Vorticity

#endif //
