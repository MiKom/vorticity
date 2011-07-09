#ifndef __VORTICITY_TIMER_H
#define __VORTICITY_TIMER_H

namespace Vorticity {

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