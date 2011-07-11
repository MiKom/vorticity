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
#include "vorticity/overlay/overlay.h"
#include "gloverlay.h"

using namespace Vorticity;

void OverlayOpenGL::begin(const float w, const float h)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void OverlayOpenGL::end()
{
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void OverlayOpenGL::text(const Font* font, const float x, const float y, const float size, const char* format, ...)
{
	va_list ap;
	char buffer[256];
	float width=0.0f, height=0.0f;

	va_start(ap, format);
	vsprintf_s<256>(buffer, format, ap);
	va_end(ap);

	for(char* c=buffer; (*c) != 0; c++)
	{
		width += font->metrics[*c].width;
		height = max(height, font->metrics[*c].height);
	}

	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glScalef(size, -size, size);

	if(background)
	{
		glColor4fv(bgColor);
		glBegin(GL_QUADS);
			glVertex2f(0.0f, height);
			glVertex2f(width + 2.0f*bgMargins[0], height);
			glVertex2f(width + 2.0f*bgMargins[0], -2.0f*bgMargins[1]);
			glVertex2f(0.0f, -2.0f*bgMargins[1]);
		glEnd();
		glTranslatef(bgMargins[0], -bgMargins[1], 0.0f);
	}

	glColor4fv(fgColor);
	fm->drawString(font, buffer);
	glPopMatrix();
}