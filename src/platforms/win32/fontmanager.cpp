/* 
 * Copyright © 2010 Micha³ Siejak
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
#include "vorticity/platforms/win32/win32.h"
#include "vorticity/overlay/overlay.h"
#include "fontmanager.h"

using namespace Vorticity;

Win32_FontManager::Win32_FontManager(HDC hAppDC) : FontManager()
{
	hDC 	= hAppDC;
	lastID 	= 1000;
}

Font* Win32_FontManager::createFont(const std::string& name, bool bold, bool italic, bool underline)
{
	HFONT hFont;
	HGDIOBJ hOldObject;
	GLYPHMETRICSFLOAT metrics[256];
	
	hFont = CreateFont(10, 0, 0, 0, bold?FW_BOLD:FW_NORMAL, italic?TRUE:FALSE, underline?TRUE:FALSE, FALSE, 
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, name.c_str());
	hOldObject = SelectObject(hDC, hFont);

	if(!wglUseFontOutlines(hDC, 0, 255, lastID, 0.0f, 0.0f, WGL_FONT_POLYGONS, metrics))
		return nullptr;
	DeleteObject(SelectObject(hDC, hOldObject));
	
	Font* font 		= new Font(name);
	font->bold 		= bold;
	font->underline = underline;
	font->italic 	= italic;
	for(int i=0; i<256; i++)
	{
		font->metrics[i].width  = metrics[i].gmfCellIncX;
		font->metrics[i].height = metrics[i].gmfBlackBoxY;
	}
	font->extra  = (long)lastID;
	lastID 		+= 256;
	return font;
}

bool Win32_FontManager::destroyFont(Font*& font)
{
	if(!font) return false;
	glDeleteLists(font->extra, 256);
	delete font;
	font = nullptr;
	return true;
}

bool Win32_FontManager::drawString(const Font* font, const std::string& str)
{
	if(!font) return false;
	glListBase(font->extra);
	glCallLists(str.length(), GL_UNSIGNED_BYTE, str.c_str());
	return true;
}