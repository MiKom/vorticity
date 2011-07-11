/* 
 * Copyright © 2009, 2010 Micha³ Siejak
 * Copyright © 2009, 2010 Mi³osz Kosobucki
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

#ifndef __VORTICITY_MATERIAL_H
#define __VORTICITY_MATERIAL_H

namespace Vorticity {

class VAPI Material : public Resource 
{
public:
	enum {
		MaxTextureChannels = 8,
	};
protected:
	string 		name;
	vector3 	diffuse;
	vector3 	specular;
	vector3 	ambient;
	vector3 	emissive;
	float 		opacity;
	float 		glossiness;
	float		specularLevel;
	Image*		textures[MaxTextureChannels];
public:
	Material(const string& name);
	virtual ~Material();
	
	virtual string getType() const
	{ return "material"; }
	
	vector3& 	getDiffuse()						{ return diffuse;			}
	void 		setDiffuse(const vector3& diff)		{ diffuse = diff;			}
	vector3& 	getSpecular()						{ return specular;			}
	void		setSpecular(const vector3& spec)	{ specular = spec;			}
	vector3& 	getAmbient()						{ return ambient;			}
	void		setAmbient(const vector3& amb)		{ ambient = amb;			}
	vector3& 	getEmissive()						{ return emissive;			}
	void		setEmissive(const vector3& emi)		{ emissive = emi;			}
	float& 	 	getOpacity()						{ return opacity;			}
	void		setOpacity(const float& opacity) 	{ this->opacity = opacity;	}
	float& 	 	getGlossiness()						{ return glossiness;		}
	void		setGlossiness(const float& glos)	{ this->glossiness = glos;	}
	float& 	 	getSpecularLevel()					{ return specularLevel;		}
	void		setSpecularLevel(const float& spec)	{ specularLevel = spec;		}

	Image*		getTextureChannel(const size_t i);
	bool		setTextureChannel(const size_t i, Image* img);

	const string& getName() const					{ return name; 				}
	void		setName(const string& name)			{ this->name = name;			}
};

} //Vorticity
#endif