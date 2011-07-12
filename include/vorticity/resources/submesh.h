/* 
 * Copyright © 2009, 2010 Michał Siejak
 * Copyright © 2009, 2010 Miłosz Kosobucki
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

#ifndef __VORTICITY_SUBMESH_H
#define __VORTICITY_SUBMESH_H

namespace Vorticity {

class Mesh;
class SubMesh;

class VAPI TriangleFace
{
protected:
	uint32_t vertex[3];
	uint32_t normal[3];
	uint32_t texcoord[3];
	SubMesh* mesh;
public:
	TriangleFace(const uint32_t* vertex=NULL,
		const uint32_t* normal=NULL, const uint32_t* texcoord=NULL);

	SubMesh* 	getMesh() 				{ return mesh; 			}
	void		setMesh(SubMesh* mesh)	{ this->mesh = mesh; 	}

	inline const uint32_t& 	getVertexIndex(const size_t i) const;
	inline const bool		getVertexIndex(const size_t i, uint32_t& index);
	inline const uint32_t& 	getNormalIndex(const size_t i) const;
	inline const bool		getNormalIndex(const size_t i, uint32_t& index);
	inline const uint32_t& 	getTexCoordIndex(const size_t i) const;
	inline const bool		getTexCoordIndex(const size_t i, uint32_t& index);

	inline bool		setVertexIndex(const size_t i, const uint32_t index);
	inline bool		setNormalIndex(const size_t i, const uint32_t index);
	inline bool		setTexCoordIndex(const size_t i, const uint32_t index);

	inline vector3&	getVertex(const size_t i);
	inline bool		getVertex(const size_t i, vector3& v);
	inline vector3& getNormal(const size_t i);
	inline bool		getNormal(const size_t i, vector3& v);
	inline vector2& getTexCoord(const size_t i);
	inline bool		getTexCoord(const size_t i, vector2& v);

	inline bool		setVertex(const size_t i, const vector3& v);
	inline bool		setNormal(const size_t i, const vector3& v);
	inline bool		setTexCoord(const size_t i, const vector2& v);

	inline uint32_t* getVertexIndices()
	{ return vertex; 		}
	inline uint32_t* getNormalIndices()
	{ return normal;		}
	inline uint32_t* getTexCoordIndices()
	{ return texcoord; 		}
};

class VAPI SubMesh : public Resource
{
protected:
	string 		name;
	Mesh*		mesh;
	Material*	material;

	std::vector<vector3> 		vertices;
	std::vector<vector3> 		normals;
	std::vector<vector2> 		texcoords;
	std::vector<TriangleFace>	faces;
public:
	SubMesh(const string& name, Mesh* owner);
	virtual ~SubMesh();

	virtual string getType() const
	{ return "submesh"; }

	Mesh*				getMesh() const				{ return mesh; 				}
	void				setMesh(Mesh* mesh)			{ this->mesh = mesh; 		}
	Material*			getMaterial() const			{ return material; 			}
	void				setMaterial(Material* mat)	{ material = mat;			}
	const string& 		getName() const				{ return name;	 			}
	void 				setName(const string& name)	{ this->name = name; 		}

	const size_t 		getVertexCount() const		{ return vertices.size(); 	}
	const size_t 		getNormalCount() const		{ return normals.size();  	}
	const size_t 		getTexCoordCount() const	{ return texcoords.size(); 	}
	const size_t 		getFaceCount() const		{ return faces.size();		}
	
	void 				clearVertices()				{ vertices.clear();			}
	void 				clearNormals()				{ normals.clear();			}
	void 				clearTexCoords()			{ texcoords.clear();		}
	void 				clearFaces()				{ faces.clear();			}

	bool 				setVertexCount(const size_t count);
	bool 				setNormalCount(const size_t count);
	bool 				setTexCoordCount(const size_t count);
	bool 				setFaceCount(const size_t count);

	inline vector3&		getVertex(const size_t i)	{ return vertices[i];		}
	inline vector3&		getNormal(const size_t i)	{ return normals[i];		}
	inline vector2& 	getTexCoord(const size_t i)	{ return texcoords[i];		}
	inline TriangleFace& getFace(const size_t i)	{ return faces[i]; 			}

	inline bool	 		getVertex(const size_t i, vector3& v);
	inline bool			setVertex(const size_t i, const vector3& v);
	inline bool			getNormal(const size_t i, vector3& v);
	inline bool			setNormal(const size_t i, const vector3& v);
	inline bool			getTexCoord(const size_t i, vector2& v);
	inline bool			setTexCoord(const size_t i, const vector2& v);

	inline bool			getFace(const size_t i, TriangleFace& face);
	inline bool			setFace(const size_t i, const TriangleFace& face);

	const size_t		getRenderElementCount() const;

	bool				createRenderBuffers();
	bool				hasRenderBuffers() const;
	void				destroyRenderBuffers();

	friend class TriangleFace;
};

} // Vorticity

#endif