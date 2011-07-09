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