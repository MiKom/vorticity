/* 
 * Copyright © 2009, 2010 Michał Siejak
 * Copyright © 2010 Miłosz Kosobucki
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

#ifndef __VORTICITY_TIMER_H
#define __VORTICITY_TIMER_H

namespace Vorticity {

class Core;

template <class T, time_t R>
class Timer
{
protected:
	T 		elapsed;
	time_t 	started;
	bool 	active;
public:
	Timer(const T startTime=T(), bool active=false) : started(0)
	{
		elapsed 	 = startTime;
		this->active = active;
	}
	Timer(bool active) : started(0)
	{
		elapsed      = T();
		this->active = active;
	}

	bool isActive() const 	{ return active; 	}
	void start() 			{ active = true; 	}
	void stop()  			{ active = false; 	}

	const T& getElapsed()	{ return elapsed;	}

	void reset()			
	{ 
		started = 0;
		elapsed = T();
	}

	void update()
	{
		if(started == 0)
			started = getCore()->getApplication()->getTime();
		else
		{
			time_t rawTime = getCore()->getApplication()->getTime();
			elapsed = T(rawTime - started)/T(R);
		}
	}
};

typedef Timer<float, 1000> StdTimer;

} // Vorticity

#endif
