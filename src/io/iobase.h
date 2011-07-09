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