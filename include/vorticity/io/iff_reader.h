/* 
 * Copyright � 2010 Micha� Siejak
 * Copyright � 2010 Mi�osz Kosobucki
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

#ifndef __VORTICITY_IFF_READER_H
#define __VORTICITY_IFF_READER_H

namespace Vorticity {

template <class T>
class ChunkHandler
{
public:
	ChunkHandler()
	{
		object = NULL;
		callback = NULL;
	}
	ChunkHandler(T* object, bool (T::*callback)())
	{
		this->object = object;
		this->callback = callback;
	}

	bool __forceinline call()
	{
		return ((object)->*(callback))();
	}

	bool (T::*callback)();
	T* object;
};

template <class T>
class VAPI IFFReader : public FileReader
{
protected:
	static uint32_t ID(const char* fourcc)
	{
		unsigned char buf[4];
		size_t fcclen = strlen(fourcc);
#ifdef _DEBUG
		if(fcclen < 1 || fcclen > sizeof(uint32_t))
			throw std::runtime_error("Invalid FOURCC identifier declared");
#endif
		memset(buf, 0, 4);
		memcpy(buf, fourcc, fcclen);
		return buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
	}
protected:
	std::map<uint32_t, ChunkHandler<T>> handlers;

	bool readChunk()
	{
		uint32_t id, length;
		ChunkHandler<T>* handler;

		file.read((char *) &id, 4);
		file.read((char *) &length, 4);
		
		if(file.eof())
			throw IOException("Unexpected end of file found", path, IOBase::ReadError);
		if(file.fail())
			throw IOException("Read error", path, IOBase::ReadError);

		handler = getChunkHandler(id);
		if(handler == NULL)
		{
			file.seekg(length, std::ios_base::cur);
			if(!file.good())
				throw IOException("Read error", path, IOBase::ReadError);
			return false;
		}

		uint32_t pos = (uint32_t)file.tellg();
		if(!handler->call())
		{
			file.seekg(pos + length, std::ios_base::beg);
			if(!file.good())
				throw IOException("Read error", path, IOBase::ReadError);
			return false;
		}
		if(file.tellg() % 2 == 1)
			file.seekg(1, std::ios_base::cur);
		return true;
	}
	inline void readNextChunk()
	{
		while(!readChunk());
	}
private:
	virtual void read()
	{
		while(!isStreamEof())
		{
			readChunk();
		}
		postload();
	}
protected:
	IFFReader(const std::string& path) : FileReader(path)
	{ }

	bool registerChunkHandler(const uint32_t id, const ChunkHandler<T>& handler)
	{
		auto location = handlers.find(id);
		if(location != handlers.end())
			return false;
		handlers[id] = handler;
		return true;
	}
	ChunkHandler<T>* getChunkHandler(const uint32_t id)
	{
		auto location = handlers.find(id);
		if(location == handlers.end())
			return NULL;
		return &location->second;
	}

	virtual void postload()
	{ }
};

} // Vorticity

#endif