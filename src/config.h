/* 
 * Copyright © 2009, 2010 Micha³ Siejak
 * Copyright © 2011 Mi³osz Kosobucki
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
#include <IL/il.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include <vorticity/vorticity_base.h>

#endif