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

#ifndef __VORTICITY_OVERLAY_H
#define __VORTICITY_OVERLAY_H

namespace Vorticity {

class FontManager;
class Font;

class VAPI Overlay
{
protected:
	FontManager *fm;
protected: // state
	float fgColor[4];
	float bgColor[4];
	float bgMargins[2];
	bool  background;
public:
	Overlay(FontManager *fm);

	Overlay& fg(float r, float g, float b);
	Overlay& bg(float r, float g, float b);
	Overlay& alpha(float a);
	Overlay& bgalpha(float a);
	Overlay& usebg(bool b);
	Overlay& margins(float x, float y);

	// Virtual methods
	virtual void begin(const float w, const float h) = 0;
	virtual void end() = 0;
	virtual void text(const Font* font, const float x, const float y, const float size,
		const char* format, ...) = 0;
};

} // Vorticity

#endif