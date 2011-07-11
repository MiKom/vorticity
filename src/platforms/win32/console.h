/* 
 * Copyright © 2010 Micha³ Siejak
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

#ifndef __VORTICITY_WIN32_CONSOLE_H
#define __VORTICITY_WIN32_CONSOLE_H

namespace Vorticity {

class VAPI Win32_Console : public Console
{
public:
	static const size_t _std_bufsize = 1024;
private:
	HANDLE hStdOutput;
	char*  buffer;
	size_t bufsize;
protected:
	virtual int overflow(int c=EOF);
	virtual int sync();
public:
	Win32_Console(const size_t bufsize=_std_bufsize);
	virtual ~Win32_Console();

	bool open(const std::string& title="");
	void close();
};

} // Vorticity

#endif