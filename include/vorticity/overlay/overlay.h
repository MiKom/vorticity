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