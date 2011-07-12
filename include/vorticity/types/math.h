/* 
 * Copyright © 2009, 2010 Michał Siejak
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

#ifndef __MATH_H
#define __MATH_H

#define MATHLIB

// General math template functions
class VAPI Math
{
public:
	template <class T> static T SQR(const T& x)
	{ return x*x; }
	template <class T> static T INV(const T& x)
	{ return (1.0f)/x; }

	static const float Pi;
	static const float Pi180;
	static const float Epsilon;

	template <class T>
	static T catmullRom(const float t, const T& a, const T& b, const T& c, const T& d)
	{
		float t2 = t*t;
		float t3 = t2*t;
		return 0.5f * ((2.0f*b) + (-a + c)*t +
				(2.0f*a - 5.0f*b + 4.0f*c - d)*t2 + (-a + 3.0f*b - 3.0f*c + d)*t3);
	}

	template <class T>
	static T catmullRomTangent(const float t, const T& p0,
		const T& a, const T& b, const T& c, const T& d)
	{
		T p1 = Math::catmullRom(t + Math::Epsilon, a, b, c, d);
		return (p1 - p0).getNormalized();
	}

	template <class T>
	static T catmullRomTangent(const float t,
		const T& a, const T& b, const T& c, const T& d)
	{
		T p0 = Math::catmullRom(t, a, b, c, d);
		T p1 = Math::catmullRom(t + Math::Epsilon, a, b, c, d);
		return (p1 - p0).getNormalized();
	}

	template <class T>
	static T lerp(const float t, const T& a, const T& b)
	{
		return t * a + (1.0f - t) * b;
	}
};

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"

#endif // __TYPESLIB_H