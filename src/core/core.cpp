#include "config.h"
#include "vorticity/core/timer.h"
#include "vorticity/resources/resource.h"
#include "vorticity/resources/rset.h"

using namespace Vorticity;

// This is just wrong to put it here.
const float Math::Pi		= 3.1415927f;
const float Math::Pi180		= 0.017453f;
const float Math::Epsilon	= 0.001f;

Core* Vorticity::getCore()
{
	static Core theCore;
	return &theCore;
}

inline std::ostream& Vorticity::log()
{
	return getCore()->getLog();
}

Core::Core() : vlog(NULL), theApp(NULL), device(NULL)
{
	defaultSet = new ResourceSet();
	currentSet = defaultSet;
}

Core::~Core()
{
	delete defaultSet;
	delete vlog;
}

StdTimer& Core::getTimer()
{
	return timer;
}

std::ostream& Core::getLog()
{
	if(theApp == NULL)
		throw std::runtime_error("No console driver initialized");
	if(vlog == NULL)
		vlog = new std::ostream(theApp->getConsole());
	return *vlog;
}