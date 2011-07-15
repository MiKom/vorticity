/* 
 * Copyright © 2010 Michał Siejak
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

#ifndef __VORTICITY_IOBASE_H
#define __VORTICITY_IOBASE_H

namespace Vorticity {

class VAPI IOException : public std::runtime_error
{
protected:
	int error;
	std::string path;
public:
	IOException(const std::string& msg, const std::string& path, const int err);
#ifndef _MSC_VER
	~IOException() throw();
#endif
	
	const int getStatus() const 		{ return error; }
	const std::string& getPath() const 	{ return path; 	}

	const std::string getMessage() const;
};

class Resource;
class VAPI IOBase
{
protected:
	std::fstream 	file;
	std::string 	path;
	Resource*		resource;
protected:
	IOBase(const std::string& path="");
private:
	//preventing copying
	IOBase(const IOBase&);
	IOBase& operator=(const IOBase&);
public:
	virtual ~IOBase();

	inline bool 	isStreamOpen() const;
	inline bool 	isStreamEof();
	const size_t 	getStreamLength();

	const std::string& getPath() const		{ return path; 		}
	std::fstream& getRawStream()			{ return file; 		}
	Resource* 	getResource() const			{ return resource;	}
	void		setResource(Resource* res)	{ resource = res;	}

	enum
	{
		OK,
		NotFound,
		ReadError,
		WriteError,
		Unknown,
	};
};

class VAPI FileReader : public virtual IOBase
{
private:
	virtual void read() = 0;
protected:
	FileReader(const std::string& path);
public:
	void readFile();
};

class VAPI FileWriter : public virtual IOBase
{
private:
	virtual void write() = 0;
protected:
	FileWriter(const std::string& path);
public:
	void writeFile();
};

} // Vorticity

#endif
