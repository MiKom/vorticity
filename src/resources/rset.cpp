#include "config.h"
#include "resource.h"
#include "rset.h"

using namespace Vorticity;

inline bool ResourceSet::hasResource(const std::string& path) const
{
	return resources.find(path) != resources.end();
}

inline void ResourceSet::clear()
{ resources.clear(); }

Resource* ResourceSet::getResource(const std::string& path)
{
	auto loc = resources.find(path);
	if(loc == resources.end())
		return NULL;
	return loc->second;
}

bool ResourceSet::addResource(const std::string& path, Resource *res)
{
	if(hasResource(path))
		return false;
	resources[path] = res;
	return true;
}

bool ResourceSet::removeResource(const std::string& path)
{
	auto loc = resources.find(path);
	if(loc != resources.end())
	{
		resources.erase(loc);
		return true;
	}
	return false;
}
