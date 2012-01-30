/* 
 * Copyright © 2009, 2010 Michał Siejak
 * Copyright © 2011 Miłosz Kosobucki
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

#ifndef __VORTICITY_BASE_H
#define __VORTICITY_BASE_H

#pragma warning(disable:4275)
#pragma warning(disable:4251)

#define VORTICITY_NAME		"Vorticity"
#define VORTICITY_UNAME		L"Vorticity"
#define	VORTICITY_VERSION	"0.1.1"

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
	#	ifdef VORTICITY_LIBRARY
	#		define VAPI __declspec(dllexport)
	#	elif VORTICITY_LIBRARY_STATIC
	#		define VAPI
	#	else
	#   	define VAPI __declspec(dllimport)
	#	endif
#else
	#	define VAPI
#endif

#ifndef _cg_h
typedef struct _CGcontext *CGcontext;
#endif

// Vorticity:Types
#include "types/string.h"
#include "types/math.h"

// Vorticity:Core
#include "core/renderer.h"
#include "core/fonts.h"
#include "core/application.h"
#ifdef WIN32
	#include "platforms/win32/win32.h"
#else
	#include "platforms/x11/x11.h"
#endif

#include "core/core.h"
#include "core/timer.h"
#include "core/entity.h"

#endif
