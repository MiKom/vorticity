#pragma once

namespace AVR
{

class AVRTexture
{
protected:
	std::string mPath;
public:
	std::string getPath() const { return mPath;}
	void setPath(const std::string& path) { mPath = path; }
	friend void write(std::ostream& os, const AVRTexture& texture);
};

void write(std::ostream& os, const AVRTexture& texture);
} //AVR
