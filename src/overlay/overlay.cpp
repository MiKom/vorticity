/* 
 * Copyright © 2010 Michał Siejak
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

#include "config.h"
#include "vorticity/overlay/overlay.h"

using namespace Vorticity;

Overlay::Overlay(FontManager* fm)
{
	this->fm = fm;
	fgColor[0] = fgColor[1] = fgColor[2] = fgColor[3] = 1.0f;
	bgColor[0] = bgColor[1] = bgColor[2] = 0.0f;
	bgColor[3] = 1.0f;
	bgMargins[0] = bgMargins[1] = 0.0f;
	background = false;
}

Overlay& Overlay::fg(float r, float g, float b)
{
	fgColor[0] = r;
	fgColor[1] = g;
	fgColor[2] = b;
	return *this;
}

Overlay& Overlay::bg(float r, float g, float b)
{
	bgColor[0] = r;
	bgColor[1] = g;
	bgColor[2] = b;
	return *this;
}

Overlay& Overlay::alpha(float a)
{
	fgColor[3] = a;
	return *this;
}

Overlay& Overlay::bgalpha(float a)
{
	bgColor[3] = a;
	return *this;
}

Overlay& Overlay::margins(float x, float y)
{
	bgMargins[0] = x;
	bgMargins[1] = y;
	return *this;
}

Overlay& Overlay::usebg(bool b)
{
	background = b;
	return *this;
}