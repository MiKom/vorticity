#ifndef __VORTICITY_RESOURCE_H
#define __VORTICITY_RESOURCE_H

namespace Vorticity {

class VAPI Resource : public Entity
{
protected:
	Renderer* device;
	int		  id;

protected:
	Resource()
	{
		device	= getCore()->getDevice();
		id		= 0;
	}
public:
	virtual string getClass() const
	{ return "resource"; }

	void setID(const int id)
	{ this->id = id; }
	int getID() const
	{ return id; }

	void setDevice(Renderer* device)
	{ this->device = device; }
	Renderer* getDevice() const
	{ return device; }
};

class VAPI TextResource : public Resource
{
protected:
	char* text;
protected:
	TextResource() : Resource(), text(NULL)
	{ }
	TextResource(char* text) : Resource(), text(text)
	{ }
public:
	virtual ~TextResource()
	{
		delete text;
	}

public:
	char* getText()
	{ 
		return text;
	}

	void setText(const char* buffer)
	{
		delete text;
		text = new char[std::strlen(buffer)+1];
		std::strcpy(text, buffer);
	}

	const size_t getTextLength() const
	{
		if(text == NULL)
			return 0;
		return std::strlen(text);
	}
};

class VAPI RenderAsset : public Resource
{
public:
	RenderAsset(const int id) : Resource()
	{ 
		setID(id);
	}

	virtual string getType() const
	{ return "render_asset"; }
};

} // Vorticity

#endif