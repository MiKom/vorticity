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

#include "config.h"
#include "vorticity/io/iobase.h"

using namespace Vorticity;

IOBase::IOBase(const std::string& path) : path(path), resource(NULL)
{ }

IOBase::~IOBase()
{
	if(file.is_open())
		file.close();
}

const size_t IOBase::getStreamLength()
{
	std::streamoff pos = file.tellg();
	file.seekg(0, std::ios_base::end);
	std::streamoff length = file.tellg();
	file.seekg(pos, std::ios_base::beg);
	return (size_t)length;
}

IOException::IOException(const std::string& msg, const std::string& path, const int err)
	: std::runtime_error(msg), path(path), error(err)
{ }

const std::string IOException::getMessage() const
{
	std::stringstream stream;
	stream << "[" << path << "] " << what();
	return stream.str();
}

FileReader::FileReader(const std::string& path)
{ 
	this->path = path;
}

void FileReader::readFile()
{
	if(file.is_open())
		file.close();

	file.open(path.c_str(), std::ios_base::in | std::ios_base::binary);
	if(!file.good())
		throw IOException("Could not open file for reading", path, IOBase::NotFound);

	read();
	file.close();
}

FileWriter::FileWriter(const std::string& path)
{ 
	this->path = path;
}

void FileWriter::writeFile()
{
	if(file.is_open())
		file.close();

	file.open(path.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	if(!file.good())
		throw IOException("Could not open file for writing", path, IOBase::WriteError);

	write();
	file.close();
}
