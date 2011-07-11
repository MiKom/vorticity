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