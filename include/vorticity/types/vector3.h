/* 
 * Copyright © 2009, 2010 Michał Siejak
 * Copyright © 2010, Miłosz Kosobucki
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

#ifndef __VECTOR3_H
#define __VECTOR3_H

#define	MATH_VECTOR3

namespace Vorticity {

// prototypes
class vector2;
class vector4;

/** \brief Universal 3D vector class
 * 
 * This class represents standard three-dimensional vector made of three
 * floating-point numbers (IEEE 754).
 **/
class vector3
{
public:
	// constructors
	/**
	 * Creates new vector with values with both components set
	 * as \a 0.0
	 **/
	vector3(void)							{ x=0.0f;	y=0.0f;		z=0.0f;		}
	/**
	 * Creates vector with all component set to \a newVal
	 */
	vector3(float newVal)					{ x=newVal;	y=newVal;	z=newVal;	}
	/**
	 * Sets first two components to corresponding parameters values, third 
	 * component is set to \a 0.0
	 */
	vector3(float nX, float nY)				{ x=nX;		y=nY;		z=0.0f;		}
	/**
	 * Sets first two components to values from parameter vector, third value
	 * is set to \a 0.0.
	 **/
	vector3(const vector2 &vec)				{ x=vec.x;	y=vec.y;	z=0.0f;		}
	/**
	 * Creates vector with component set to corresponding parameters
	 **/
	vector3(float nX, float nY, float nZ)	{ x=nX;		y=nY;		z=nZ;		}
	vector3(const vector2 &vec, float nZ)	{ x=vec.x;	y=vec.y;	z=nZ;		}
	vector3(const float* ptr)				{ x=ptr[0];	y=ptr[1];	z=ptr[2];	}

	// destructor
	virtual ~vector3(void)		{ }

	// operators:
	// assign (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	// addition
	vector3 operator + (const vector3 &vec) const
	{ return vector3(x+vec.x, y+vec.y, z+vec.z);		}
	// subtraction
	vector3 operator - (const vector3 &vec) const
	{ return vector3(x-vec.x, y-vec.y, z-vec.z);		}
	// multiplication
	vector3 operator * (const vector3 &vec) const
	{ return vector3(x*vec.x, y*vec.y, z*vec.z);		}
	// division
	vector3 operator / (const vector3 &vec) const
	{	
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f) return vector3();
		return vector3(x/vec.x, y/vec.y, z/vec.z);
	}

	//! self addition
	void operator += (const vector3 &vec)
	{ x+=vec.x; y+=vec.y; z+=vec.z; }
	//! self addition (by scalar)
	void operator += (const float &value)
	{ x+=value; y+=value; z+=value; }
	//! self subtraction
	void operator -= (const vector3 &vec)
	{ x-=vec.x; y-=vec.y; z-=vec.z; }
	//! self subtraction (by scalar)
	void operator -= (const float &value)
	{ x-=value; y-=value; z-=value; }
	// self multiplication
	void operator *= (const vector3 &vec)
	{ x*=vec.x; y*=vec.y; z*=vec.z; }
	//! self multiplication (by scalar)
	void operator *= (const float &value)
	{ x*=value; y*=value; z*=value; }
	
	//! self division
	/**
	 * Self division, if aany component of divisor is 0.0 
	 * nothing happens
	 * @param vec divisor
	 **/
	void operator /= (const vector3 &vec)
	{
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f)
			return;
		x/=vec.x; y/=vec.y; z/=vec.z;
	}
	/**
	 * Self division by scalar value
	 * @param value scalar value, if 0.0, nothing happens
	 **/
	void operator /= (const float &value)
	{
		if(value == 0.0f)
			return;
		x/=value; y/=value; z/=value;
	}

	// vector-float ops:
	//! addition (by scalar)
	friend vector3 operator + (float value, const vector3 &vec)
	{ return vector3(vec.x+value, vec.y+value, vec.z+value);		}
	//! subtraction (by scalar)
	friend vector3 operator - (float value, const vector3 &vec)
	{ return vector3(vec.y-value, vec.y-value, vec.z-value);		}
	//! multiplication (by scalar)
	friend vector3 operator * (float value, const vector3 &vec)
	{ return vector3(vec.x*value, vec.y*value, vec.z*value);		}
	//! division (by scalar)
	friend vector3 operator / (float value, const vector3 &vec)
	{
		if(value == 0.0f) return vector3();
		return vector3(vec.x/value, vec.y/value, vec.z/value);
	}

	//! unary plus
	vector3 operator + (void) const
	{ return *this; }
	//! unary minus
	/**
	 * Each component of vector has its sign shifted
	 **/
	vector3 operator - (void) const
	{ return vector3(-x, -y, -z); }

	//! comparsion
	/**
	 * Compares vectors by checking if difference at every component is
	 * less then pre-defined \a Math::Epsilon value
	 * @return true if vectors differ by at most Math::Epsilon at each component
	 * false otherwise
	 **/
	bool operator == (const vector3 &vec) const
	{
		return	(((x-Math::Epsilon)<=vec.x) && ((x+Math::Epsilon)>= vec.x)) &&
				(((y-Math::Epsilon)<=vec.y) && ((y+Math::Epsilon)>= vec.y)) &&
				(((z-Math::Epsilon)<=vec.z) && ((z+Math::Epsilon)>= vec.z));
	}
	/**
	 * Compares vectors by checking if difference at every component is
	 * less then pre-defined \a Math::Epsilon value
	 * @return true if vectors differ by more than \a Math::Epsilon at at least 
	 * one component false otherwise
	 **/
	bool operator != (const vector3 &vec) const
	{ return !(*this == vec);	}


	// functions:
	//! loads the vector with zeroes, previous values are destroyed
	vector3& zeros()
	{ 
		x = y = z = 0.0f;
		return *this;
	}
	//! loads the vector with ones, previous values are destroyed
	vector3& ones()
	{
		x = y = z = 1.0f;
		return *this;
	}

	//! returns length of the vector
	float	length() const
	{ return sqrtf(Math::SQR(x)+Math::SQR(y)+Math::SQR(z)); }
	//! returns squared length of the vector
	float	sqLength() const
	{ return (Math::SQR(x) + Math::SQR(y) + Math::SQR(z));	}
	//! returns inverted length of the vector
	float	invLength() const
	{ return Math::INV(length());					}

	//! normalize the vector
	vector3& normalize()
	{
		float	len = length();
		if(len==0.0f || len==1.0f)	return *this;
		x /= len;
		y /= len;
		z /= len;
		return *this;
	}
	//! return normalized vector
	vector3	getNormalized() const
	{
		vector3	result = *this;
		result.normalize();
		return result;
	}

	//! dot product
	float	dot(const vector3 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z); }

	//! cross product
	vector3	cross(const vector3 &vec) const
	{ return vector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x); }

	//! linear interpolation
	/**
	 * Returns linear interpolation between calling object and
	 * argument at \a factor fraction. 
	 * @parameter factor fraction of interpolation, values outside [0,1] will
	 * give values on the line going through vectors but beyond the line segment
	 * between the pints
	 **/
	vector3	lerp(const vector3 &vec, const float factor) const
	{ return (*this)*(1.0f-factor) + vec*factor; }

	// io

	/**
	* Reads three IEEE 754 floating point numbers (x,y,z) from file stream
	* @param file stream to read from
	**/
	bool read(std::fstream &file)
	{
		file.read((char*)&cell, sizeof(cell));
		return true;
	}

	/**
	 * Writes three IEEE 754 floating point numbers (x,y,z) to file stream
	 * @param file file stream to be written to
	 **/
	bool write(std::fstream &file)
	{
		file.write((char*)&cell, sizeof(cell));
		return true;
	}

	/**
	 * Reads three IEEE 754 floating point numbers (x,y,z) from standard
	 * C file pointer.
	 * @param file pointer to read from
	 **/
	bool read(FILE *file)
	{
		fread(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Writes three IEEE 754 floating point numbers (x,y,z) to standard
	 * C file pointer
	 * @param file file pointer to be written to
	 **/
	bool write(FILE *file)
	{
		fwrite(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Reads string representation of three IEEE 754 floating point numbers
	 * (x,y,z)
	 * from the stream.
	 * @param os target output stream
	 */
	friend std::istream& operator>>( std::istream& is, vector3& v){
		is >> v.x >> v.y >> v.z;
		return is;
	}

	/**
	* Reads string representation of three IEEE 754 floating point numbers
	* (x,y,z)
	* from the stream.
	* @param os target output stream
	*/
	friend std::ostream& operator<<( std::ostream& os, const vector3& v){
		os << v.x << v.y << v.z;
		return os;
	}

	string to_s() const
	{
		char buffer[50];
		sprintf(buffer, "vec3: %f %f %f", x, y, z);
		return string(buffer);
	}
	static string name()
	{ return "vector3"; }
	static string name2()
	{ return "float3"; }

	// data
	union
	{
		struct	{ float x; float y; float z; };
		struct	{ float r; float g; float b; };
		float	cell[3];
	};
};

typedef vector3	float3;
typedef vector3 vec3;

} // Vorticity

#endif