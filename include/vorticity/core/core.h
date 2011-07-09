#ifndef __VORTICITY_CORE_H
#define __VORTICITY_CORE_H

namespace Vorticity {

class ResourceSet;
class VAPI Core
{
private:
	Application *theApp;
	ResourceSet *currentSet;
	ResourceSet *defaultSet;
	Renderer	*device;

	std::ostream* 	vlog;
	StdTimer 		timer;
public:
	Core();
	virtual ~Core();

	ResourceSet*	getDefaultResourceSet()
	{ return defaultSet; }
	ResourceSet* 	getCurrentResourceSet()
	{ return currentSet; }

	void setCurrentResourceSet(ResourceSet* set)
	{ currentSet = set;	 }

	StdTimer& getTimer();
	std::ostream& getLog();

	void	setApplication(Application *app) { theApp = app;  }
	void	setDevice(Renderer *dev)		 { device = dev;  }

	Renderer* 		getDevice() const		 { return device; }
	Application* 	getApplication() const	 { return theApp; }
};

VAPI Core* getCore();
VAPI inline std::ostream& log();

} // Vorticity

#endif