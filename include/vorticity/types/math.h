/*
 * CORE Engine, version 0.2 [alpha]
 * Copyright (c) 2007-2008, Micha³ Siejak
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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