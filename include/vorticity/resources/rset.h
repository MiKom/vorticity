#ifndef __VORTICITY_RSET_H
#define __VORTICITY_RSET_H

#include "io/iobase.h"

namespace Vorticity {

class Resource;
class ResourceSet
{
protected:
	std::map<std::string, Resource*> resources;
public:
	ResourceSet() { }

	inline bool		hasResource(const std::string& path) const;
	inline void		clear();

	Resource* 		getResource(const std::string& path);
	bool			removeResource(const std::string& path);
	bool			addResource(const std::string& path, Resource* res);

	template <class T>
	T* loadResource(FileReader& reader)
	{
		T* resource;
		resource = dynamic_cast<T*>(getResource(reader.getPath()));
		if(resource == nullptr)
		{
			resource = new T(reader);
			addResource(reader.getPath(), resource);
		}
		return resource;
	}
};

} // Vorticity

#endif