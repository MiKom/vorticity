/* 
 * Copyright © 2009, 2010 Micha³ Siejak
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

#ifndef __VORTICITY_PPFX_HDR_H
#define __VORTICITY_PPFX_HDR_H

namespace Vorticity {

class VAPI PostProcessHDR : public PostProcess
{
protected:
	NodeCamera *srcColor;
	Shader *blurShader;
	Shader *extractShader;
	Shader *tonemapShader;
	int fb, fb_downsampled;
	int w,h;
protected:
	virtual bool acceptSource(const string& name);
	virtual bool acceptShader(const string& name);
public:
	PostProcessHDR(Renderer *device=NULL);
	virtual ~PostProcessHDR();

	virtual string getType() const
	{ return "hdr"; }

	virtual int render();
	virtual bool validate();
};

} // Vorticity

#endif