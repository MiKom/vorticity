/* 
 * Copyright © 2009 Michał Siejak
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

#ifndef __STRING_H
#define __STRING_H

#include <string>

namespace Vorticity {

class string : public std::string
{
public:
	string()						: std::string()		{ }
	string(const string& str)		: std::string(str)	{ }
	string(const char *s, size_t n) : std::string(s, n) { }
	string(const char *s)			: std::string(s)	{ }
	string(size_t n, char c)		: std::string(n, c) { }
	string(const std::string& s)	: std::string(s)	{ }

	string(const string& str, size_t pos, size_t n=npos)						 : std::string(str, pos, n) { }
	template<class InputIterator> string(InputIterator begin, InputIterator end) : std::string(begin, end)	{ }

	string to_s() const
	{
		char*	buffer = new char[size()+10];
		string	result;
		sprintf(buffer, "string: %s", c_str());
		result = string(buffer);
		delete[] buffer;
		return result;
	}
	static string name()
	{ return "string"; }
	static string name2()
	{ return "string"; }

	/**
	 * Reads from fstream until null-byte is reached
	 * @param file stream to read from
	 **/
	virtual bool read(std::fstream &file)
	{
		clear();
		char c;
		do { file.get(c); if(c) *this += c; } while(c != 0);
		return true;
	}

	/**
	 * Writes the c-style string to file stream 
	 * terminated with null byte.
	 * @param file file stream to be written to
	 **/
	virtual bool write(std::fstream &file)
	{
		file << *this;
		file.put(0);
		return true;
	}
};

} // Vorticity

#endif