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