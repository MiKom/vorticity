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

#ifndef __VORTICITY_WIN32FONTMANAGER_H
#define __VORTICITY_WIN32FONTMANAGER_H

namespace Vorticity {

class VAPI Win32_FontManager : public FontManager
{
	friend class Win32_Application;
protected:
	HDC	hDC;
	unsigned long lastID;
	std::vector<unsigned long> fonts;
protected:
	Win32_FontManager(HDC hAppDC);
public:
	Font*	createFont(const std::string& name, bool bold=false, bool italic=false, bool underline=false);
	bool 	destroyFont(Font*& font);
	bool	drawString(const Font* font, const std::string& str);
};

} // Vorticity

#endif