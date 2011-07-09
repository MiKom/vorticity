#include "config.h"
#include "win32.h"

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

