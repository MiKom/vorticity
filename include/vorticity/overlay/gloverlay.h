#ifndef __VORTICITY_GLOVERLAY_H
#define __VORTICITY_GLOVERLAY_H

namespace Vorticity {

class VAPI OverlayOpenGL : public Overlay
{
public:
	OverlayOpenGL(FontManager *fm) : Overlay(fm)
	{ }

	virtual void begin(const float w, const float h);
	virtual void end();
	virtual void text(const Font* font, const float x, const float y, const float size,
		const char* format, ...);
};

} // Vorticity

#endif
