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

#include "config.h"
#include "vorticity/platforms/win32/win32.h"
#include "console.h"

using namespace Vorticity;

Win32_Console::Win32_Console(const size_t bufsize) : Console(),
	hStdOutput(NULL), buffer(new char[bufsize]), bufsize(bufsize)
{ 
	setg(0, 0, 0);
	setp(buffer, buffer + bufsize - 1);
}

Win32_Console::~Win32_Console()
{
	close();
	delete[] buffer;
}

bool Win32_Console::open(const std::string& title)
{
	if(AllocConsole() == FALSE)
		return false;
	hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if(title.length() > 0)
		SetWindowText(GetConsoleWindow(), title.c_str());
	else
		SetWindowText(GetConsoleWindow(), defaultTitle.c_str());
	return true;
}

void Win32_Console::close()
{
	if(hStdOutput != NULL)
	{
		hStdOutput = NULL;
		FreeConsole();
	}
}

int Win32_Console::overflow(int c)
{
	char* ptr_begin = buffer;
	char* ptr_end   = pptr();

	setp(buffer, buffer + bufsize + 1);
	if(c != EOF) *ptr_end++ = c;
	if(hStdOutput == NULL) return 0;

	DWORD dwLen = (DWORD)(ptr_end - ptr_begin);
	DWORD dwWritten;
	if(WriteConsole(hStdOutput, buffer, dwLen, &dwWritten, NULL) == FALSE)
		return EOF;
	if(dwLen != dwWritten)
		return EOF;
	return 0;
}

int Win32_Console::sync()
{
	if(overflow(EOF) == EOF)
		return -1;
	return 0;
}

