/* 
 * Copyright © 2009, 2010 Micha³ Siejak
 *
 * All rights reserved.
 * 
 * This file is part of Vorticity.
 *
 * Vorticity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vorticity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vorticity.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __VORTICITY_CORE_H
#define __VORTICITY_CORE_H

namespace Vorticity {

class ResourceSet;

template <class T, time_t R>
class Timer;
typedef Timer<float, 1000> StdTimer;

class VAPI Core
{
private:
	Application *theApp;
	ResourceSet *currentSet;
	ResourceSet *defaultSet;
	Renderer	*device;

	std::ostream* 	vlog;
	StdTimer* 		timer;
public:
	Core();
	virtual ~Core();

	ResourceSet*	getDefaultResourceSet()
	{ return defaultSet; }
	ResourceSet* 	getCurrentResourceSet()
	{ return currentSet; }

	void setCurrentResourceSet(ResourceSet* set)
	{ currentSet = set;	 }

	StdTimer* getTimer();
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
