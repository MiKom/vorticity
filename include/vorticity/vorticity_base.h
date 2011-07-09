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
	#include "platforms/linux/linux.h"
#endif

#include "core/timer.h"
#include "core/core.h"
#include "core/entity.h"

#endif