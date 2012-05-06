#include <vector>
#include <ostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include "avrtexture.h"

using namespace AVR;

void AVR::write(std::ostream& os, const AVRTexture& texture)
{
	os.write("TEXT", 4);
	auto sizePos = os.tellp();
	os.seekp(4, std::ios::cur);
	auto startPos = os.tellp();

	os.write(texture.mPath.c_str(), texture.mPath.length() + 1);

	auto endPos = os.tellp();
	int32_t size = endPos - startPos;
	os.seekp(sizePos);
	os.write((char*) &size, 4);
	os.seekp(endPos);
}
