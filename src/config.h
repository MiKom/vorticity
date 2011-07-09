#ifndef __VORTICITY_CONFIG_H
#define __VORTICITY_CONFIG_H

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <math.h>
#include <stdint.h>
#include <stdexcept>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/glew.h>
//#include "IL/il.h"
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "vorticity_base.h"

#endif