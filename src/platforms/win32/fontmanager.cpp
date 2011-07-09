#include "config.h"
#include "win32.h"
#include "overlay/overlay.h"
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