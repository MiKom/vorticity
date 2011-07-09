#ifndef __VORTICITY_NODE_SC_H
#define __VORTICITY_NODE_SC_H

namespace Vorticity {

template <class T>
class NodeSC : public KeyframeController<T>
{
public:
	NodeSC(const string& name, XNode *parent=NULL) : KeyframeController(name, parent)
	{ }
	virtual ~NodeSC()
	{ }

	virtual string getType() const
	{ return "sc"; }
	virtual bool isTypeCompatible(const string& type) const
	{ return type == Attribute<T>("").getType(); }

	virtual bool getStatus() const
	{	
		ArrayAttribute &keys = (ArrayAttribute&)getAttrib("keys");
		ArrayAttribute &values = (ArrayAttribute&)getAttrib("values");

		size_t ikeys	= keys.getCount();
		size_t ivalues	= values.getCount();

		if(ikeys == ivalues && ikeys >= 4)
			return true;
		return false;
	}

	virtual void evaluate()
	{
		int index;
		float time[4];
		T value[4];

		//if(!getStatus()) return;
		float frameTime = getTime();
		int count = getKeyCount();
		if(!getKey(frameTime, &index, &time[1], &value[1]))
			return;
		if(index >= count-1)
		{
			writeOutput<T>(value[1]);
			return;
		}

		getKey(index-1, &time[0], &value[0]);
		getKey(index+1, &time[2], &value[2]);
		getKey(index+2, &time[3], &value[3]);

		float t = (time[2] - frameTime) / (time[2] - time[1]);
		T out_value = Math::catmullRom(1.0f - t, value[0], value[1], value[2], value[3]);
		writeOutput<T>(out_value);
	}
};

typedef NodeSC<vec2>	NodeSC2;
typedef NodeSC<vec3>	NodeSC3;
typedef NodeSC<vec4>	NodeSC4;

} // Vorticity

#endif