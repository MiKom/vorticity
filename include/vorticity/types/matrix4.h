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

#ifndef __MATRIX4_H
#define __MATRIX4_H

#define	MATH_MATRIX4

namespace Vorticity {

// prototypes
class vector2;
class vector3;
class vector4;

//! 4x4 float matrix
/**
 * Universal 4x4 matrix made of floating point, single precision values.
 * Matrix is kept in column-major format.
 **/
class matrix4
{
public:
	// constructors
	/**
	 * @param i true for identity matrix, false for zeroes
	 **/
	matrix4(bool i=false)			{ if(i) identity(); else memset(cell, 0, 64); }
	/**
	 * Creates new matrix by copying data directly from \a data parameter
	 * @param data memory block with exactly 16 float values
	 **/
	matrix4(float *data)			{ memcpy(cell, data, 64);						}

	/**
	 * Creates new matrix by copying 16 double-precision values from
	 * data pointer \n
	 * \warning Some precision is lost due to double to float conversion
	 * \param data pointer to 16 double values
	 **/
	matrix4(double *data){
		for(size_t i=0; i<16; i++){
			cell[i] = static_cast<float>(data[i]);
		}
	}
	/**
	 * Creates new matrix with all values set to \a value
	 **/
	matrix4(float value)			{ for(int i=0; i<16; i++) { cell[i] = value; }  }
	/**
	 * Creates new matrix out of four \ref vec4 object
	 */
	matrix4(vector4 &row1, vector4 &row2, vector4 &row3, vector4 &row4)
	{	memcpy(&cell[0], row1.cell, 16); memcpy(&cell[4], row2.cell, 16);
		memcpy(&cell[8], row3.cell, 16); memcpy(&cell[12], row4.cell, 16);
	}
	/**
	 * Creates new matrix from four pointers, each pointing to
	 * arrays of four float values.
	 **/
	matrix4(float *row1, float *row2, float *row3, float *row4)
	{	memcpy(&cell[0], row1, 16); memcpy(&cell[4], row2, 16);
		memcpy(&cell[8], row3, 16); memcpy(&cell[12], row4, 16);
	}
	/**
	 * Creates new matrix from values provided by parameters,
	 * column-major.
	 **/
	matrix4(float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8,
			float c9, float c10, float c11, float c12, float c13, float c14, float c15, float c16)
	{
		cell[0]=c1; cell[1]=c2; cell[2]=c3; cell[3]=c4; cell[4]=c5; cell[5]=c6;
		cell[6]=c7; cell[7]=c8; cell[8]=c9; cell[9]=c10; cell[10]=c11; cell[11]=c12;
		cell[12]=c13; cell[13]=c14; cell[14]=c15; cell[15]=c16;
	}

	// destructor
	virtual ~matrix4(void)			{ }

	// operators:
	// assign (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	//! loads zero matrix (resets matrix)
	matrix4& zeroes(void)
	{ 
		memset(cell, 0, 64);
		return *this;
	}
	//! loads identity matrix
	matrix4& identity(void)
	{
		memset(cell, 0, 64);
		cell[0] = 1.0f; cell[5] = 1.0f; cell[10] = 1.0f; cell[15] = 1.0f;
		return *this;
	}

	//! addition
	matrix4 operator + (const matrix4 &m) const
	{ 
		matrix4 result;
		for(int i=0; i<16; i++) { result.cell[i] = cell[i] + m.cell[i]; }
		return result;
	}

	//! subtraction
	matrix4 operator - (const matrix4 &m) const
	{
		matrix4 result;
		for(int i=0; i<16; i++) { result.cell[i] = cell[i] - m.cell[i]; }
		return result;
	}

	// TODO: optimize!
	//! multiplication 
	matrix4 operator * (const matrix4 &m) const
	{
		matrix4 r;

		r.cell[0] = cell[0]*m.cell[0] + cell[1]*m.cell[4] + cell[2]*m.cell[8] + cell[3]*m.cell[12];
		r.cell[1] = cell[0]*m.cell[1] + cell[1]*m.cell[5] + cell[2]*m.cell[9] + cell[3]*m.cell[13];
		r.cell[2] = cell[0]*m.cell[2] + cell[1]*m.cell[6] + cell[2]*m.cell[10] + cell[3]*m.cell[14];
		r.cell[3] = cell[0]*m.cell[3] + cell[1]*m.cell[7] + cell[2]*m.cell[11] + cell[3]*m.cell[15];

		r.cell[4] = cell[4]*m.cell[0] + cell[5]*m.cell[4] + cell[6]*m.cell[8] + cell[7]*m.cell[12];
		r.cell[5] = cell[4]*m.cell[1] + cell[5]*m.cell[5] + cell[6]*m.cell[9] + cell[7]*m.cell[13];
		r.cell[6] = cell[4]*m.cell[2] + cell[5]*m.cell[6] + cell[6]*m.cell[10] + cell[7]*m.cell[14];
		r.cell[7] = cell[4]*m.cell[3] + cell[5]*m.cell[7] + cell[6]*m.cell[11] + cell[7]*m.cell[15];

		r.cell[8] = cell[8]*m.cell[0] + cell[9]*m.cell[4] + cell[10]*m.cell[8] + cell[11]*m.cell[12];
		r.cell[9] = cell[8]*m.cell[1] + cell[9]*m.cell[5] + cell[10]*m.cell[9] + cell[11]*m.cell[13];
		r.cell[10] = cell[8]*m.cell[2] + cell[9]*m.cell[6] + cell[10]*m.cell[10] + cell[11]*m.cell[14];
		r.cell[11] = cell[8]*m.cell[3] + cell[9]*m.cell[7] + cell[10]*m.cell[11] + cell[11]*m.cell[15];

		r.cell[12] = cell[12]*m.cell[0] + cell[13]*m.cell[4] + cell[14]*m.cell[8] + cell[15]*m.cell[12];
		r.cell[13] = cell[12]*m.cell[1] + cell[13]*m.cell[5] + cell[14]*m.cell[9] + cell[15]*m.cell[13];
		r.cell[14] = cell[12]*m.cell[2] + cell[13]*m.cell[6] + cell[14]*m.cell[10] + cell[15]*m.cell[14];
		r.cell[15] = cell[12]*m.cell[3] + cell[13]*m.cell[7] + cell[14]*m.cell[11] + cell[15]*m.cell[15];

		return r;
	}

	//! matrix-vector product
	vector4 operator * (const vector4 &v) const
	{
		vector4	r;

		r.x = cell[0]*v.x + cell[1]*v.y + cell[2]*v.z + cell[3]*v.w;
		r.y = cell[4]*v.x + cell[5]*v.y + cell[6]*v.z + cell[7]*v.w;
		r.z = cell[8]*v.x + cell[9]*v.y + cell[10]*v.z + cell[11]*v.w;
		r.w = cell[12]*v.x + cell[13]*v.y + cell[14]*v.z + cell[15]*v.w;

		return r;
	}

	//! self addition
	void operator += (const matrix4 &m)
	{ *this = m + *this; }
	//! self subtraction
	void operator -= (const matrix4 &m)
	{ *this = m - *this; }
	//! self multiplication
	void operator *= (const matrix4 &m)
	{ *this = m * *this; }

	//! unary plus
	matrix4 operator + (void) const 
	{ return *this; }
	//! unary minus
	/**
	 * Every component of the matrix has its sign switched
	 **/
	matrix4 operator - (void) const
	{
		return matrix4(	-cell[0], -cell[1], -cell[2], -cell[3], -cell[4], -cell[5],
						-cell[6], -cell[7], -cell[8], -cell[9], -cell[10], -cell[11],
						-cell[12], -cell[13], -cell[14], -cell[15]);
	}

	//! swap cell[a] with cell[b]
	inline void swap(const int a, const int b)
	{
		float temp	= cell[a];
		cell[a]		= cell[b];
		cell[b]		= temp;
	}

	//! transposition
	/**
	 * Transposes the matrix
	 * @return reference to self for chaining.
	 **/
	matrix4& transpose(void)
	{
		swap(1,4); swap(2,8); swap(3,12);
		swap(4,1); swap(6,9); swap(7,13);
		swap(8,2); swap(9,6); swap(11,14);
		swap(12,3); swap(13,7); swap(14,11);
		return *this;
	}

	/**
	 * Returns transposed matrix leaving original object
	 * intact.
	 * @return new transposed matrix
	 **/
	matrix4 getTransposed(void) const
	{
		return matrix4(
			cell[0], cell[4], cell[8], cell[12],
			cell[1], cell[5], cell[9], cell[13],
			cell[2], cell[6], cell[10], cell[14],
			cell[3], cell[7], cell[11], cell[15]);
	}

	//! discriminant
	/**
	 * Returns the discriminant of the matrix.
	 **/
	float det(void) const
	{
		return	cell[0] * cell[5] * cell[10] * cell[15] +
				cell[0] * cell[6] * cell[11] * cell[13] +
				cell[0] * cell[7] * cell[9] * cell[14] +

				cell[1] * cell[4] * cell[11] * cell[14] +
				cell[1] * cell[6] * cell[8] * cell[15] +
				cell[1] * cell[7] * cell[10] * cell[12] +

				cell[2] * cell[4] * cell[9] * cell[15] +
				cell[2] * cell[5] * cell[11] * cell[12] +
				cell[2] * cell[7] * cell[8] * cell[13] +

				cell[3] * cell[4] * cell[10] * cell[13] +
				cell[3] * cell[5] * cell[8] * cell[14] +
				cell[3] * cell[6] * cell[9] * cell[12] -

				cell[0] * cell[5] * cell[11] * cell[14] -
				cell[0] * cell[6] * cell[9] * cell[15] -
				cell[0] * cell[7] * cell[10] * cell[13] -

				cell[1] * cell[4] * cell[10] * cell[15] -
				cell[1] * cell[6] * cell[11] * cell[12] -
				cell[1] * cell[7] * cell[8] * cell[14] -

				cell[2] * cell[4] * cell[11] * cell[13] -
				cell[2] * cell[5] * cell[8] * cell[15] - 
				cell[2] * cell[7] * cell[9] * cell[12] -

				cell[3] * cell[4] * cell[9] * cell[14] -
				cell[3] * cell[5] * cell[10] * cell[12] -
				cell[3] * cell[6] * cell[8] * cell[13];
	}

	//! inverse 
	/**
	 * Inverts the matrix.
	 **/
	void inverse(void)
	{ *this = getInverted(); }

	/**
	 * Returns inverted matrix leaving the original object intact
	 * @return new inverted matrix
	 **/
	matrix4	getInverted(void) const
	{
		float detf = 1.0f / det();
		return matrix4(
			detf * (cell[5]*cell[10]*cell[15] + cell[6]*cell[11]*cell[13] + cell[7]*cell[9]*cell[14] - cell[5]*cell[11]*cell[14] - cell[6]*cell[9]*cell[15] - cell[7]*cell[10]*cell[13]),
			detf * (cell[1]*cell[11]*cell[14] + cell[2]*cell[9]*cell[15] + cell[3]*cell[10]*cell[13] - cell[1]*cell[10]*cell[15] - cell[2]*cell[11]*cell[13] - cell[3]*cell[9]*cell[14]),
			detf * (cell[1]*cell[6]*cell[15] + cell[2]*cell[7]*cell[13] + cell[3]*cell[5]*cell[14] - cell[1]*cell[7]*cell[14] - cell[2]*cell[5]*cell[15] - cell[3]*cell[6]*cell[13]),
			detf * (cell[1]*cell[7]*cell[10] + cell[2]*cell[5]*cell[11] + cell[3]*cell[6]*cell[9] - cell[1]*cell[6]*cell[11] - cell[2]*cell[7]*cell[9] - cell[3]*cell[5]*cell[10]),
			detf * (cell[4]*cell[11]*cell[14] + cell[6]*cell[8]*cell[15] + cell[7]*cell[10]*cell[12] - cell[4]*cell[10]*cell[15] - cell[6]*cell[11]*cell[12] - cell[7]*cell[8]*cell[14]),
			detf * (cell[0]*cell[10]*cell[15] + cell[2]*cell[11]*cell[12] + cell[3]*cell[8]*cell[14] - cell[0]*cell[11]*cell[14] - cell[2]*cell[8]*cell[15] - cell[3]*cell[10]*cell[12]),
			detf * (cell[0]*cell[7]*cell[14] + cell[2]*cell[4]*cell[15] + cell[3]*cell[6]*cell[12] - cell[0]*cell[6]*cell[15] - cell[2]*cell[7]*cell[12] - cell[3]*cell[4]*cell[14]),
			detf * (cell[0]*cell[6]*cell[11] + cell[2]*cell[7]*cell[8] + cell[3]*cell[4]*cell[10] - cell[0]*cell[7]*cell[10] - cell[2]*cell[4]*cell[11] - cell[3]*cell[6]*cell[8]),
			detf * (cell[4]*cell[9]*cell[15] + cell[5]*cell[11]*cell[12] + cell[7]*cell[8]*cell[13] - cell[4]*cell[11]*cell[13] - cell[5]*cell[8]*cell[15] - cell[7]*cell[9]*cell[12]),
			detf * (cell[0]*cell[11]*cell[13] + cell[1]*cell[8]*cell[15] + cell[3]*cell[9]*cell[12] - cell[0]*cell[9]*cell[15] - cell[1]*cell[11]*cell[12] - cell[3]*cell[8]*cell[13]),
			detf * (cell[0]*cell[5]*cell[15] + cell[1]*cell[7]*cell[12] + cell[3]*cell[4]*cell[13] - cell[0]*cell[7]*cell[13] - cell[1]*cell[4]*cell[15] - cell[3]*cell[5]*cell[12]),
			detf * (cell[0]*cell[7]*cell[9] + cell[1]*cell[4]*cell[11] + cell[3]*cell[5]*cell[8] - cell[0]*cell[5]*cell[11] - cell[1]*cell[7]*cell[8] - cell[3]*cell[4]*cell[9]),
			detf * (cell[4]*cell[10]*cell[13] + cell[5]*cell[8]*cell[14] + cell[6]*cell[9]*cell[12] - cell[4]*cell[9]*cell[14] - cell[5]*cell[10]*cell[12] - cell[6]*cell[8]*cell[13]),
			detf * (cell[0]*cell[9]*cell[14] + cell[1]*cell[10]*cell[12] + cell[2]*cell[8]*cell[13] - cell[0]*cell[10]*cell[13] - cell[1]*cell[8]*cell[14] - cell[2]*cell[9]*cell[12]),
			detf * (cell[0]*cell[6]*cell[13] + cell[1]*cell[4]*cell[14] + cell[2]*cell[5]*cell[12] - cell[0]*cell[5]*cell[14] - cell[1]*cell[6]*cell[12] - cell[2]*cell[4]*cell[13]),
			detf * (cell[0]*cell[5]*cell[10] + cell[1]*cell[6]*cell[8] + cell[2]*cell[4]*cell[9] - cell[0]*cell[6]*cell[9] - cell[1]*cell[4]*cell[10] - cell[2]*cell[5]*cell[8])
		);
	}

	// transformation operations

	//! translation
	/**
	 * Sets the translation (move) component of the matrix to the
	 * components of the parameter.
	 * Translation components are in 12th, 13th and 14th component of
	 * the matrix.
	 **/
	void setTranslation(const vector3 &vec)
	{ cell[12] = vec.x; cell[13] = vec.y; cell[14] = vec.z; }

	/**
	 * Sets the translation (move) component of the matrix to values
	 * from parameters.
	 * Translation components are in 12th, 13th and 14th component of
	 * the matrix.
	 **/
	void setTranslation(const float x, const float y, const float z)
	{ cell[12] = x; cell[13] = y; cell[14] = z; }

	/**
	 * Applies translation (move) to the matrix by given vector.
	 * @param vector move vector applied to the matrix
	 * @return reference to self for chaining.
	 **/
	matrix4& translate(const vector3 &vec)
	{
		matrix4 t(true);
		t.setTranslation(vec);
		*this *= t;
		return *this;
	}

	/**
	 * Applies translation (move) to the matrix by given parameter values.
	 *
	 * @param x translation on the x axis
	 * @param y translation on the y axis
	 * @param z translation on the z axis
	 * @return reference to self for chaining.
	 **/
	matrix4& translate(const float x, const float y, const float z)
	{
		matrix4 t(true);
		t.setTranslation(x, y, z);
		*this *= t;
		return *this;
	}

	// scale
	/**
	 * Sets the scale factor of the matrix to \a s (on every axe).
	 **/
	void setScale(const float s)
	{ cell[0] = s; cell[5] = s; cell[10] = s; cell[15] = s; }

	/**
	 * Sets the scale factor on every axe to values on corresponding
	 * parameter vector components.
	 */
	void setScale(const vector3 &vec)
	{ cell[0] = vec.x; cell[5] = vec.y; cell[10] = vec.z; cell[15] = 1.0f; }

	/**
	 * Sets the scale factor on each axe to values of corresponding
	 * parameters
	 * @param x scale factor on x axis
	 * @param y scale factor on y axis
	 * @param z scale factor on z axis
	 **/
	void setScale(const float x, const float y, const float z)
	{ cell[0] = x; cell[5] = y; cell[10] = z; cell[15] = 1.0f; }

	/**
	 * Applies scaling by \a s to the matrix (on every axe)
	 **/
	matrix4& scale(const float s)
	{
		matrix4 result(true);
		result.setScale(s);
		*this *= result;
		return *this;
	}

	/**
	 * Applies scaling to the matrix. Each axis is scaled
	 * by factor from corresponding \a vec component
	 * @return reference to self for chaining.
	 **/
	matrix4& scale(const vector3 &vec)
	{
		matrix4 result(true);
		result.setScale(vec);
		*this *= result;
		return *this;
	}

	/**
	 * Applies scale to the matrix on every axe.
	 * @param x scale factor on x axis.
	 * @param y scale factor on y axis.
	 * @param z scale factor on z axis.
	 * @return reference to self for chaining.
	 **/
	matrix4& scale(const float x, const float y, const float z)
	{
		matrix4 result(true);
		result.setScale(x, y, z);
		*this *= result;
		return *this;
	}

	//! rotation (X-axis)
	/**
	 * Sets the rotation on the X axis to value given in parameter
	 * @param x degrees of rotation to be applied, in \b radians
	 **/
	void setRotationX(const float x)
	{
		cell[5]  = cosf(x); 
		cell[6]  = -sinf(x);
		cell[9]  = sinf(x);
		cell[10] = cosf(x);
	}
	
	//! rotation (Y-axis)
	/**
	 * Sets the rotation on the Y axis to value given in parameter
	 * @param y degrees of rotation to be applied, in \b radians
	 **/
	void setRotationY(const float y)
	{
		cell[0]  = cosf(y);
		cell[2]  = sinf(y);
		cell[8]  = -sinf(y);
		cell[10] = cosf(y);
	}

	//! rotation (Z-axis)
	/**
	 * Sets the rotation on the Z axis to value given in parameter
	 * @param z degrees of rotation to be applied, in \b radians
	 **/
	void setRotationZ(const float z)
	{
		cell[0]  = cosf(z);
		cell[1]  = -sinf(z);
		cell[4]  = sinf(z);
		cell[5]  = cosf(z);
	}

	/**
	 * Applies rotation to the matrix, values taken from the
	 * corresponding vector components
	 * @param vec vector with rotation values, in \b radians.
	 * @return reference to self for chaining.
	 **/
	matrix4& rotate(const vector3 &vec)
	{
		matrix4 mx(true), my(true), mz(true);
		mx.setRotationX(vec.x);
		my.setRotationY(vec.y);
		mz.setRotationZ(vec.z);
		*this *= (mx * my * mz);
		return *this;
	}

	/**
	 * Applies rotation to the matrix. Values must be expressed in
	 * \b radians
	 * @param x rotation on x axis
	 * @param y rotation on y axis
	 * @param z rotation on z axis
	 * @return reference to self for chaining
	 **/
	matrix4& rotate(const float x, const float y, const float z)
	{
		matrix4 mx(true), my(true), mz(true);
		mx.setRotationX(x);
		my.setRotationY(y);
		mz.setRotationZ(z);
		*this *= (mx * my * mz);
		return *this;
	}
	static string name()
	{ return "matrix4"; }
	static string name2()
	{ return "matrix4"; }

	// io
	/**
	 * Reads 16 float values from file stream
	 **/
	bool read(std::fstream &file)
	{
		file.read((char*)cell, sizeof(cell));
		return true;
	}

	/**
	 * Reads 16 float values from C-style file pointer
	 **/
	bool read(FILE *file)
	{
		fread(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Writes 16 float values (IEEE 754) to file stream
	 **/
	bool write(std::fstream &file)
	{
		file.write((char*)cell, sizeof(cell));
		return true;
	}

	/**
	 * Writes 16 float values (IEEE 754) to C-style file pointer
	 **/
	bool write(FILE *file)
	{
		fwrite(cell, sizeof(cell), 1, file);
		return true;
	}

	/**
	 * Reads 16 string representations of floating point numbers (IEEE 754)
	 * from file stream.
	 * @return reference to file stream for chaining
	 **/
	friend std::istream& operator>>( std::istream& is, matrix4& v){
		for(size_t i=0; i<16; i++)
		{
			is >> v.cell[i];
		}
		return is;
	}

	/**
	 * Writes 16 string representations of floating point numbers (IEEE 754)
	 * to file stream.
	 * @return reference to file stream for chaining
	 **/
	friend std::ostream& operator<<( std::ostream& os, const matrix4& v){
		for(size_t i=0; i<16; i++)
		{
			os <<  v.cell[i];
		}
		return os;
	}

	// data
	union
	{
		float	matrix[4][4];
		float	cell[16];
	};
};

typedef matrix4	mat4;
typedef matrix4	float16;

} // Vorticity

#endif
