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

#ifndef __VECTOR4_H
#define __VECTOR4_H

#define	MATH_VECTOR4

namespace Vorticity {

// prototypes
class vector2;
class vector3;

/** \brief Universal 3D vector class
 * 
 * This class represents standard four-dimensional vector made of four
 * floating-point numbers (IEEE 754).
 **/
class vector4
{
public:
	// constructors
	/**
	 * Creates vector with all components set to \a 0.0
	 **/
	vector4(void)									{ x=0.0f;	y=0.0f;		z=0.0f;		w=0.0f;		}
	
	/**
	 * Creates vector with all components set to newVal
	 * @param newVal value to be assinged to all components
	 **/
	vector4(float newVal)							{ x=newVal;	y=newVal;	z=newVal;	w=newVal;	}
	
	/**
	 * Creates vector with first two components set as: \n
	 * x = nX, y = nY \n
	 * All other components are set to \a 0.0
	 **/
	vector4(float nX, float nY)						{ x=nX;		y=nY;		z=0.0f;		w=0.0f;		}
	
	/**
	 * Creates vector with first two components copied from parameter \n
	 * x = vec.x, y = vec.y \n
	 * All other components are set to 0.0
	 **/
	vector4(const vector2 &vec)					 	{ x=vec.x;	y=vec.y;	z=0.0f;		w=0.0f;		}
	
	/**
	 * Creates vector with first three components set as: \n
	 * x = nX, y = nY, z = nZ \n
	 * Last component is set to 0.0
	 **/
	vector4(float nX, float nY, float nZ)			{ x=nX;		y=nY;		z=nZ;		w=0.0f;		}
	
	/**
	 * Creates vector with first two components copied from \a vec and third set to \a nZ \n
	 * x = vec.x, y = vec.y, z = vec.z \n
	 * Last component is set to 0.0
	 **/
	vector4(const vector2 &vec, float nZ)			{ x=vec.x;	y=vec.y;	z=nZ;		w=0.0f;		}
	
	/**
	 * Creates constructor that copies first three components from parameter, fourth one is
	 * set to \a 0.0
	 **/
	vector4(const vector3 &vec)						{ x=vec.x;	y=vec.y;	z=vec.z;	w=0.0f;		}
	
	/**
	 * Creates vector with components set to corresponding parameters
	 **/
	vector4(float nX, float nY, float nZ, float nW)	{ x=nX;		y=nY;		z=nZ;		w=nW;		}
	
	/**
	 * Creates vector with first three components copied from \a vec and last
	 * component set to \a nW
	 * @param vec provides data to first three components
	 * @param nW value for the fourth component
	 **/
	vector4(const vector3 &vec, float nW)			{ x=vec.x;	y=vec.y;	z=vec.z;	w=nW;		}

	/**
	 * Creates vector from two 2D vectors.
	 * @param v1 2D vector that provides data to \a x and \a y components
	 * @param v2 2D vector that provides data to \a z and \a w components
	 **/
	vector4(const vector2 &v1, const vector2 &v2)	{ x=v1.x;	y=v1.y;		z=v2.x;		w=v2.y;		}

	/**
	 * Creates vector by reading four floating point 
	 * numbers from memory pointed by parameter
	 * @param ptr source pointer for data
	 **/
	vector4(const float* ptr)						{ x=ptr[0];	y=ptr[1];	z=ptr[2];	w=ptr[3];	}

	// destructor
	virtual ~vector4(void)		{ }

	// operators:
	// assign (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	//! addition
	vector4 operator + (const vector4 &vec) const
	{ return vector4(x+vec.x, y+vec.y, z+vec.z, w+vec.w);		}
	//! subtraction
	vector4 operator - (const vector4 &vec) const
	{ return vector4(x-vec.x, y-vec.y, y-vec.z, w-vec.w);		}
	//! multiplication
	vector4 operator * (const vector4 &vec) const
	{ return vector4(x*vec.x, y*vec.y, z*vec.z, w*vec.w);		}
	//! division
	vector4 operator / (const vector4 &vec) const
	{	
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f || vec.w == 0.0f) return vector4();
		return vector4(x/vec.x, y/vec.y, z/vec.z, w/vec.w);
	}

	//! self addition
	void operator += (const vector4 &vec)
	{ x+=vec.x; y+=vec.y; z+=vec.z; w+=vec.w; }
	//! self addition (scalar)
	void operator += (const float &value)
	{ x+=value; y+=value; z+=value; w+=value; }
	//! self subtraction
	void operator -= (const vector4 &vec)
	{ x-=vec.x; y-=vec.y; z-=vec.z; w-=vec.w; }
	//! self subtraction (scalar)
	void operator -= (const float &value)
	{ x-=value; y-=value; z-=value; w-=value; }
	//! self multiplication
	void operator *= (const vector4 &vec)
	{ x*=vec.x; y*=vec.y; z*=vec.z; w*=vec.w; }
	//! self multiplication (scalar)
	void operator *= (const float &value)
	{ x*=value; y*=value; z*=value; w*=value; }
	
	//! self division
	/**
	 * Self division, if any component of divisor is \a 0.0
	 * nothing happens
	 * @param vec divisor
	 **/
	void operator /= (const vector4 &vec)
	{
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f || vec.w == 0.0f)
			return;
		x/=vec.x; y/=vec.y; z/=vec.z; w/=vec.w;
	}
	
	/**
	 * Self division by scalar value
	 * @param value scalar value, if \a 0.0, nothing happens
	 **/
	void operator /= (const float &value)
	{
		if(value == 0.0f)
			return;
		x/=value; y/=value; z/=value; w/=value;
	}

	// vector-float ops:
	//! addition
	friend vector4 operator + (float value, const vector4 &vec)
	{ return vector4(vec.x+value, vec.y+value, vec.z+value, vec.w+value);	}
	//! subtraction
	friend vector4 operator - (float value, const vector4 &vec)
	{ return vector4(vec.y-value, vec.y-value, vec.z-value, vec.w-value);	}
	//! multiplication
	friend vector4 operator * (float value, const vector4 &vec)
	{ return vector4(vec.x*value, vec.y*value, vec.z*value, vec.w*value);	}
	
	//! division operator
	/**
	 * Division by scalar operator. If scalar is 0.0 vector of
	 * four zeros is returned
	 **/
	friend vector4 operator / (float value, const vector4 &vec)
	{
		if(value == 0.0f) return vector4();
		return vector4(vec.x/value, vec.y/value, vec.z/value, vec.w/value);
	}

	//! unary plus
	vector4 operator + (void) const
	{ return *this; }
	//! unary minus
	/**
	 * Each component of vector has its sign shifted
	 **/
	vector4 operator - (void) const
	{ return vector4(-x, -y, -z, -w); }

	// comparsion
	/**
	 * Compares vectors by checking if difference at every component is
	 * less then pre-defined \a Math::Epsilon value
	 * @return true if vectors differ by at most Math::Epsilon at each component
	 * false otherwise
	 **/
	bool operator == (const vector4 &vec) const
	{
		return	(((x-Math::Epsilon)<=vec.x) && ((x+Math::Epsilon)>= vec.x)) &&
				(((y-Math::Epsilon)<=vec.y) && ((y+Math::Epsilon)>= vec.y)) &&
				(((z-Math::Epsilon)<=vec.z) && ((z+Math::Epsilon)>= vec.z)) &&
				(((w-Math::Epsilon)<=vec.w) && ((w+Math::Epsilon)>= vec.w));
	}

	/**
	 * Compares vectors by checking if difference at every component is
	 * less then pre-defined \a Math::Epsilon value
	 * @return true if vectors differ by more than \a Math::Epsilon at at least 
	 * one component false otherwise
	 **/
	bool operator != (const vector4 &vec) const
	{ return !(*this == vec);	}


	// functions:
	// loads the vector with zeroes, previous values are destroyed
	vector4& zeros()
	{ 
		x = y = z = w = 0.0f;
		return *this;
	}
	// loads the vector with ones, previous values are destroyed
	vector4& ones()
	{
		x = y = z = w = 1.0f;
		return *this;
	}
	
	//! returns length of the vector
	float	length() const
	{ return sqrtf(Math::SQR(x)+Math::SQR(y)+Math::SQR(z)+Math::SQR(w)); }
	//! returns squared length of the vector
	float	sqLength() const
	{ return (Math::SQR(x)+Math::SQR(y)+Math::SQR(z)+Math::SQR(w));		}
	//! returns inverted length of the vector
	float	invLength() const
	{ return Math::INV(length());						}

	//! normalize the vector
	vector4& normalize()
	{
		float	len = length();
		if(len==0.0f || len==1.0f)	return *this;
		x /= len;
		y /= len;
		z /= len;
		w /= len;
		return *this;
	}
	//! return normalized vector
	vector4	getNormalized() const
	{
		vector4	result = *this;
		result.normalize();
		return result;
	}

	//! dot product
	float	dot3(const vector3 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z);			}

	//! dot product
	float	dot3(const vector4 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z);			}

	//! dot product
	float	dot(const vector4 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z+w*vec.w); }

	//! cross product
	vector4	cross(const vector3 &vec) const
	{ return vector4(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - vec.y*x); }

	//! cross product
	vector4	cross(const vector4 &vec) const
	{ return vector4(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - vec.y*x); }

	//! linear interpolation
	/**
	 * Returns linear interpolation between calling object and
	 * argument at \a factor fraction. 
	 * @parameter factor fraction of interpolation, values outside [0,1] will
	 * give values on the line going through vectors but beyond the line segment
	 * between the pints
	 **/
	vector4	lerp(const vector4 &vec, const float factor) const
	{ return (*this)*(1.0f-factor) + vec*factor; }

	// io
	/**
	* Reads four IEEE 754 floating point numbers (x,y,z,w) from file stream
	* @param file stream to read from
	**/
	bool read(std::fstream &file)
	{
		file.read((char*)&cell, sizeof(cell));
		return true;
	}
	/**
	 * Writes four IEEE 754 floating point numbers (x,y,z,w) to file stream
	 * @param file file stream to be written to
	 **/
	bool write(std::fstream &file)
	{
		file.write((char*)&cell, sizeof(cell));
		return true;
	}

	/**
	 * Reads four IEEE 754 floating point numbers (x,y,z,w) from standard
	 * C file pointer.
	 * @param file pointer to read from
	 **/
	bool read(FILE *file)
	{
		fread(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Writes four IEEE 754 floating point numbers (x,y,z,w) to standard
	 * C file pointer
	 * @param file file pointer to be written to
	 **/
	bool write(FILE *file)
	{
		fwrite(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Reads string representation of four IEEE 754 floating point numbers
	 * (x,y,z,w)
	 * from the stream.
	 * @param os target output stream
	 */
	friend std::istream& operator>>( std::istream& is, vector4& v){
		is >> v.x >> v.y >> v.z >> v.w;
		return is;
	}

	/**
	* Reads string representation of four IEEE 754 floating point numbers
	* (x,y,z,w)
	* from the stream.
	* @param os target output stream
	*/
	friend std::ostream& operator<<( std::ostream& os, const vector4& v){
		os << v.x << v.y << v.z << v.w;
		return os;
	}

	string to_s() const
	{
		char buffer[50];
		sprintf(buffer, "vec4: %f %f %f %f", x, y, z, w);
		return string(buffer);
	}
	static string name()
	{ return "vector4"; }
	static string name2()
	{ return "float4"; }

	// data
	union
	{
		struct	{ float x; float y; float z; float w; };
		struct	{ float r; float g; float b; float a; };
		float	cell[4];
	};
};

typedef vector4	float4;
typedef vector4	vec4;

} // Vorticity

#endif