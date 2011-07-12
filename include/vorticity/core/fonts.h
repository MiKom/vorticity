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

#ifndef __VORTICITY_FONTS_H
#define __VORTICITY_FONTS_H

namespace Vorticity {

typedef struct
{
	float width;
	float height;
} GlyphMetric;

class Font
{
public:
	std::string name;
	bool 		bold;
	bool 		italic;
	bool 		underline;
	GlyphMetric metrics[256];
	long		extra;
public:
	Font(const std::string& name) :
		bold(false), italic(false), underline(false), extra(0)
	{ 
		this->name = name;
		std::memset(metrics, 0, sizeof(metrics));
	}
};

class FontManager
{
public:
	virtual Font*	createFont(const std::string& name, bool bold=false, bool italic=false, bool underline=false) = 0;
	virtual bool 	destroyFont(Font*& font) = 0;
	virtual bool	drawString(const Font* font, const std::string& str) = 0;
};


};

#endif