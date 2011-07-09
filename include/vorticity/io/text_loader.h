#ifndef __VORTICITY_TEXTFILE_LOADER_H
#define __VORTICITY_TEXTFILE_LOADER_H

namespace Vorticity {

class VAPI TextLoader : public FileReader
{
private:
	virtual void read();
public:
	TextLoader(const std::string& path);
	virtual ~TextLoader();
};

} // Vorticity

#endif