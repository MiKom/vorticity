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

#ifndef __VORTICITY_NODE_CAMERA_H
#define __VORTICITY_NODE_CAMERA_H

namespace Vorticity {

class RenderAsset;
class Backdrop;
class VAPI NodeCamera : public Object
{
protected:
	RenderAsset* target;
	Backdrop* backdrop;

	float 	fov, zNear, zFar;
	bool 	useProjectionMatrix;
	matrix4 customProjection;
public:
	NodeCamera(const string& name, XNode *parent=NULL);
	virtual ~NodeCamera();

	virtual string getType() const
	{ return "camera"; }
	RenderAsset* getTargetBuffer() const
	{ return target; }

	virtual void evaluate();
	virtual bool bind(Resource* resource);

	void 		setBackdrop(Backdrop* backdrop)	{ this->backdrop = backdrop; 	}
	Backdrop* 	getBackdrop() const				{ return backdrop;				}
	bool		isUsingCustomProjection() const	{ return useProjectionMatrix;	}
	void		useCustomProjection(bool val)	{ useProjectionMatrix = val;	}

	const float getFOV() const
	{ return fov; }
	const float getZNear() const
	{ return zNear; }
	const float getZFar() const
	{ return zFar; }

	virtual bool	setupProjection() const;
	virtual bool	beginRendering(ClearMode cmode) const;
	virtual bool	endRendering() const;
};

} // Vorticity

#endif