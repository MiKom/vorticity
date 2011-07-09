#ifndef __VORTICITY_IMAGE_LOADER_H
#define __VORTICITY_IMAGE_LOADER_H

namespace Vorticity {

class VAPI ImageLoader : public FileReader
{
private:
	virtual void read();
	static bool devilInit;
public:
	ImageLoader(const std::string& path);
	virtual ~ImageLoader();
};

} // Vorticity

#endif