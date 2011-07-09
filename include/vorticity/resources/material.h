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