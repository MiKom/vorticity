#ifndef __VORTICITY_KEYFRAME_CTRL_H
#define __VORTICITY_KEYFRAME_CTRL_H

namespace Vorticity {

typedef enum
{
	KC_Running,
	KC_Stopped,
	KC_Paused,
} KeyframeControllerState;

class AbstractKeyframeController : public Controller
{
protected:
	int  			state;
	bool  			loop;
protected:
	AbstractKeyframeController(const string& name, XNode *parent=NULL)
		: Controller(name, parent)
	{
		state	 = KC_Running;
		loop	 = false;
	}
public:
	virtual ~AbstractKeyframeController()
	{ }

	int		getState() const		{ return state;		 			}
	bool	isRunning() const		{ return state == KC_Running; 	}
	bool	isPaused() const		{ return state == KC_Paused;	}
	bool	isStopped() const		{ return state == KC_Stopped;	}

	bool	isLooped() const		{ return loop;					}
	void	setLoop(bool l)			{ loop = l;						}

	virtual void	play()		= 0;
	virtual void	pause()		= 0;
	virtual void	stop()		= 0;
	virtual void	restart()	= 0;

	virtual bool	addKey(const float t, const int &value)		{ return false; }
	virtual bool	addKey(const float t, const float &value)	{ return false; }
	virtual bool	addKey(const float t, const vec2 &value)	{ return false; }
	virtual bool	addKey(const float t, const vec3 &value)	{ return false; }
	virtual bool	addKey(const float t, const vec4 &value)	{ return false; }

	virtual bool	removeKey(const int index) = 0;
	virtual size_t	getKeyCount() const = 0;

	virtual bool	getKey(const int index, float* t, int* value) const		{ return false; }
	virtual bool	getKey(const int index, float* t, float* value) const	{ return false; }
	virtual bool	getKey(const int index, float* t, vec2* value) const	{ return false; }
	virtual bool	getKey(const int index, float* t, vec3* value) const	{ return false; }
	virtual bool	getKey(const int index, float* t, vec4* value) const	{ return false; }

	virtual bool	getKey(const float t, int* index, float* time, int* value)  const		{ return false; }
	virtual bool	getKey(const float t, int* index, float* time, float* value) const		{ return false; }
	virtual bool	getKey(const float t, int* index, float* time, vec2* value) const		{ return false; }
	virtual bool	getKey(const float t, int* index, float* time, vec3* value) const		{ return false; }
	virtual bool	getKey(const float t, int* index, float* time, vec4* value) const		{ return false; }
};

template <class T>
class KeyframeController : public AbstractKeyframeController
{
protected:
	mutable float 	baseTime;
	mutable float	pauseTime;
protected:
	inline float getInternalTime() const
	{
		return (float)getAttrib("factor") * getCore()->getTimer().getElapsed();
	}
protected:
	KeyframeController(const string& name, XNode *parent=NULL)
		: AbstractKeyframeController(name, parent)
	{
		addAttrib(new ArrayAttribute("keys"));
		addAttrib(new ArrayAttribute("values"));
		addAttrib(new Attribute<float>("factor")) = 1.0f;

		baseTime = pauseTime = 0.0f;
	}

	virtual float getTime() const
	{	
		if(state == KC_Paused)
			return pauseTime - baseTime;
		else if(state == KC_Stopped)
			return 0.0f;

		float tf = getInternalTime();
		if(loop)
		{
			ArrayAttribute &keys = (ArrayAttribute&)getAttrib("keys");
			if(keys.getCount() > 0)
			{
				if((float)keys.back() < (tf - baseTime))
					baseTime = tf;
			}
		}
		return tf - baseTime;
	}
public:
	virtual ~KeyframeController()
	{ }

	virtual size_t getKeyCount() const
	{ return ((ArrayAttribute&)getAttrib("keys")).getCount(); }

	virtual bool addKey(const float t, const T &value)
	{
		((ArrayAttribute&)getAttrib("keys")).push(new Attribute<float>("", t));
		((ArrayAttribute&)getAttrib("values")).push(new Attribute<T>("", value));
		return true;
	}

	virtual bool removeKey(const int index)
	{
		if(!((ArrayAttribute&)getAttrib("keys")).remove(index))
			return false;
		if(!((ArrayAttribute&)getAttrib("values")).remove(index))
			return false;
		return true;
	}

	virtual bool getKey(const int index, float* t, T* value) const
	{
		ArrayAttribute &keys = (ArrayAttribute&)getAttrib("keys");
		ArrayAttribute &values = (ArrayAttribute&)getAttrib("values");
		if(keys.getCount() == 0)
			return false;

		int cl_index = index;
		if(index < 0) cl_index = 0;
		else if(keys.getCount() <= (size_t)index) cl_index = keys.getCount()-1;

		if(t) *t = keys.getAt(cl_index);
		if(value) *value = values.getAt(cl_index);
		return true;
	}

	virtual bool getKey(const float t, int* index, float* time, T* value) const
	{
		if(!getStatus())
			return false;

		ArrayAttribute &keys = (ArrayAttribute&)getAttrib("keys");
		AttribIterator it = keys.enumValues();

		int tmp_index = 0;
		float tmp_time[2];
		while(AnyAttribute *attr = keys.getAt(it))
		{
			tmp_time[0] = *attr;
			if(tmp_index == 0) tmp_time[1] = tmp_time[0];
			if(tmp_time[0] > t) break;
			tmp_index++;
			tmp_time[1] = tmp_time[0];
		}
		tmp_index = max(tmp_index-1, 0);
		if(index) *index = tmp_index;
		if(time)  *time  = tmp_time[1];
		if(value) *value = ((ArrayAttribute&)getAttrib("values")).getAt(tmp_index);
		return true;
	}

	virtual void play()
	{
		if(state == KC_Running) return;

		float tf = getInternalTime();
		if(state == KC_Paused)
			baseTime = tf - pauseTime + baseTime;
		else if(state == KC_Stopped)
			baseTime = tf;
		state = KC_Running;
	}
	virtual void pause()
	{
		if(state == KC_Paused) return;

		pauseTime = getInternalTime();
		state	  = KC_Paused;
	}

	virtual void stop()
	{
		if(state == KC_Stopped) return;

		baseTime  = 0.0f;
		pauseTime = 0.0f;
		state	  = KC_Stopped;
	}
	virtual void restart()
	{
		stop();
		play();
	}
};

} // Vorticity

#endif