#ifndef __VORTICITY_BASE_H
#define __VORTICITY_BASE_H

#pragma warning(disable:4275)
#pragma warning(disable:4251)

#define VORTICITY_NAME		"Vorticity"
#define VORTICITY_UNAME		L"Vorticity"
#define	VORTICITY_VERSION	"0.1.1"

#ifdef VORTICITY_EXPORTS
#define VAPI __declspec(dllexport)
#else
#define VAPI __declspec(dllimport)
#endif

#ifndef _cg_h
typedef struct _CGcontext *CGcontext;
#endif

// Vorticity:Types
#include "vorticity/types/string.h"
#include "vorticity/types/math.h"

// Vorticity:Core
#include "vorticity/core/renderer.h"
#include "vorticity/core/fonts.h"
#include "vorticity/core/application.h"
#ifdef WIN32
#include "vorticity/platforms/win32/win32.h"
#else
#include "vorticity/platforms/linux/linux.h"
#endif

#include "vorticity/core/timer.h"
#include "vorticity/core/core.h"
#include "vorticity/core/entity.h"

#endif