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