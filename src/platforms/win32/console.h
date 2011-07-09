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