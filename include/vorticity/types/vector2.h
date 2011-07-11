/* 
 * Copyright © 2009, 2010 Micha³ Siejak
 * Copyright © 2010, Mi³osz Kosobucki
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

#ifndef __VECTOR2_H
#define __VECTOR2_H

#define	MATH_VECTOR2

namespace Vorticity {

// Prototypes
class vector3;
class vector4;

/** \brief Universal 2D vector class
 * 
 * This class represents standard dwo-dimensional vector made of two
 * floating-point numbers (IEEE 754).
 **/
class vector2
{
public:
	// constructors
	/**
	 * Creates new vector with values with both components set
	 * as 0.0
	 **/
	vector2(void)				{ x=0.0f;	y=0.0f;		}

	/**
	 * Creates new vector with both x and y set as newVal
	 **/
	vector2(float newVal)		{ x=newVal;	y=newVal;	}
	/**
	 * Creates vector with component set to corresponding parameters
	 **/
	vector2(float nX, float nY)	{ x=nX;		y=nY;		}

	/**
	 * Reads two floating point numbers from memory pointed
	 * by parameter
	 * @param ptr source pointer for data
	 **/
	vector2(const float* ptr )	{ x=ptr[0];	y=ptr[1];	}

	// destructor
	virtual ~vector2(void)		{ }

	// operators:
	// assign (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	//! addition
	vector2 operator + (const vector2 &vec) const
	{ return vector2(x+vec.x, y+vec.y);		}
	//! subtraction
	vector2 operator - (const vector2 &vec) const
	{ return vector2(x-vec.x, y-vec.y);		}
	//! multiplication
	vector2 operator * (const vector2 &vec) const
	{ return vector2(x*vec.x, y*vec.y);		}
	
	/**
	 * Division on each component, if any component of
	 * divisor is 0.0 vector (0.0, 0.0) is returned
	 * @return divided vector
	 **/
	vector2 operator / (const vector2 &vec) const
	{	
		if(vec.x == 0.0f || vec.y == 0.0f) return vector2();
		return vector2(x/vec.x, y/vec.y);
	}

	//! self addition
	void operator += (const vector2 &vec)
	{ x+=vec.x; y+=vec.y; }
	//! self addition (by scalar)
	void operator += (const float &value)
	{ x+=value; y+=value; }
	//! self subtraction
	void operator -= (const vector2 &vec)
	{ x-=vec.x; y-=vec.y; }
	//! self addition (by scalar)
	void operator -= (const float &value)
	{ x-=value; y-=value; }
	//! self multiplication
	void operator *= (const vector2 &vec)
	{ x*=vec.x; y*=vec.y; }
	//! self multiplication (by scalar)
	void operator *= (const float &value)
	{ x*=value; y*=value; }
	/**
	 * Self division, if any component of divisor is 0.0
	 * nothing happens
	 * @param vec divisor
	 **/
	void operator /= (const vector2 &vec)
	{
		if(vec.x == 0.0f || vec.y == 0.0f)
			return;
		x/=vec.x; y/=vec.y;
	}

	/**
	 * Self division by scalar value
	 * @param value scalar value, if 0.0, nothing happens
	 **/
	void operator /= (const float &value)
	{
		if(value == 0.0f)
			return;
		x/=value; y/=value;
	}

	// vector-float ops:
	//! addition (scalar)
	friend vector2 operator + (float value, const vector2 &vec)
	{ return vector2(vec.x+value, vec.y+value);		}
	//! subtraction (scalar)
	friend vector2 operator - (float value, const vector2 &vec)
	{ return vector2(vec.y-value, vec.y-value);		}
	//! multiplication (scalar)
	friend vector2 operator * (float value, const vector2 &vec)
	{ return vector2(vec.x*value, vec.y*value);		}
	//! division (scalar)
	friend vector2 operator / (float value, const vector2 &vec)
	{
		if(value == 0.0f) return vector2();
		return vector2(vec.x/value, vec.y/value);
	}

	//! unary plus
	vector2 operator + (void) const
	{ return *this; }
	//! unary minus
	/**
	 * Each component of vector has its sign shifted
	 **/
	vector2 operator - (void) const
	{ return vector2(-x, -y); }

	//! comparsion
	/**
	 * Compares vectors by checking if difference at every component is
	 * less then pre-defined \a Math::Epsilon value
	 * @return true if vectors differ by at most Math::Epsilon at each component
	 * false otherwise
	 **/
	bool operator == (const vector2 &vec) const
	{
		return	(((x-Math::Epsilon)<=vec.x) && ((x+Math::Epsilon)>= vec.x)) &&
				(((y-Math::Epsilon)<=vec.y) && ((y+Math::Epsilon)>= vec.y));
	}

	/**
	 * Compares vectors by checking if difference at every component is
	 * less then pre-defined \a Math::Epsilon value
	 * @return true if vectors differ by more than \a Math::Epsilon at at least 
	 * one component false otherwise
	 **/
	bool operator != (const vector2 &vec) const
	{ return !(*this == vec);	}


	// functions:
	//! loads the vector with zeroes, previous values are destroyed
	vector2& zeros()
	{ 
		x = y = 0.0f;
		return *this;
	}
	//! loads the vector with ones, previous values are destroyed
	vector2& ones()
	{
		x = y = 1.0f;
		return *this;
	}

	//! returns length of the vector
	/**
	 * Returns length of vector computed as \f$sqrt{x^2 + y^2}$
	 **/
	float	length() const
	{ return sqrtf(Math::SQR(x)+Math::SQR(y));	}
	//! returns squared length of the vector
	float	sqLength() const
	{ return (Math::SQR(x) + Math::SQR(y));		}
	//! returns inverted length of the vector
	float	invLength() const
	{ return Math::INV(length());			}

	//! normalize the vector
	vector2& normalize()
	{
		float	len = length();
		if(len==0.0f || len==1.0f)	return *this;
		x /= len;
		y /= len;
		return *this;
	}
	//! return normalized vector
	vector2	getNormalized() const
	{
		vector2	result = *this;
		result.normalize();
		return result;
	}

	//! dot product
	float	dot(const vector2 &vec) const
	{ return (x*vec.x+y*vec.y); }

	//! linear interpolation
	/**
	 * Returns linear interpolation between calling object and
	 * argument at \a factor fraction. 
	 * @parameter factor fraction of interpolation, values outside [0,1] will
	 * give values on the line going through vectors but beyond the line segment
	 * between the pints
	 **/
	vector2	lerp(const vector2 &vec, const float factor) const
	{ return (*this)*(1.0f-factor) + vec*factor; }

	// io

	/**
	 * Reads two IEEE 754 floating point numbers (x,y) from file stream
	 * @param file stream to read from
	 **/
	bool read(std::fstream &file)
	{
		file.read((char*)cell, sizeof(cell));
		return true;
	}

	/**
	 * Reads two IEEE 754 floating point numbers (x,y) from standard
	 * C file pointer.
	 * @param file pointer to read from
	 **/
	bool read(FILE *file)
	{
		fread(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Writes two IEEE 754 floating point numbers (x,y) to file stream
	 * @param file file stream to be written to
	 **/
	bool write(std::fstream &file)
	{
		file.write((char*)cell, sizeof(cell));
		return true;
	}

	/**
	 * Writes two IEEE 754 floating point numbers (x,y) to standard
	 * C file pointer
	 * @param file file pointer to be written to
	 **/
	bool write(FILE *file)
	{
		fwrite(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Reads string representation of two IEEE 754 floating point numbers
	 * (x,y) from the stream.
	 * @param os target output stream
	 */
	friend std::istream& operator>>( std::istream& is, vector2& v){
		is >> v.x >> v.y;
		return is;
	}

	/**
	 * Writes string representation of two IEEE 754 floating point numbers (x,y)
	 * to the stream.
	 * @param os target output stream
	 */
	friend std::ostream& operator<<( std::ostream& os, const vector2& v){
		os << v.x << v.y;
		return os;
	}

	string to_s() const
	{
		char buffer[50];
		sprintf(buffer, "vec2: %f %f", x, y);
		return string(buffer);
	}
	static string name()
	{ return "vector2"; }
	static string name2()
	{ return "float2"; }

	// data
	union
	{
		struct	{ float x; float y; };
		float	cell[2];
	};
};

typedef vector2	float2;
typedef vector2	vec2;

} // Vorticity

#endif 