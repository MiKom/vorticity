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

	const uint32_t& 	getVertexIndex(const size_t i) const;
	const bool		getVertexIndex(const size_t i, uint32_t& index);
	const uint32_t& 	getNormalIndex(const size_t i) const;
	const bool		getNormalIndex(const size_t i, uint32_t& index);
	const uint32_t& 	getTexCoordIndex(const size_t i) const;
	const bool		getTexCoordIndex(const size_t i, uint32_t& index);

	bool		setVertexIndex(const size_t i, const uint32_t index);
	bool		setNormalIndex(const size_t i, const uint32_t index);
	bool		setTexCoordIndex(const size_t i, const uint32_t index);

	vector3&	getVertex(const size_t i);
	bool		getVertex(const size_t i, vector3& v);
	vector3& getNormal(const size_t i);
	bool		getNormal(const size_t i, vector3& v);
	vector2& getTexCoord(const size_t i);
	bool		getTexCoord(const size_t i, vector2& v);

	bool		setVertex(const size_t i, const vector3& v);
	bool		setNormal(const size_t i, const vector3& v);
	bool		setTexCoord(const size_t i, const vector2& v);

	uint32_t* getVertexIndices()
	{ return vertex; 		}
	uint32_t* getNormalIndices()
	{ return normal;		}
	uint32_t* getTexCoordIndices()
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

	vector3&		getVertex(const size_t i)	{ return vertices[i];		}
	vector3&		getNormal(const size_t i)	{ return normals[i];		}
	vector2& 	getTexCoord(const size_t i)	{ return texcoords[i];		}
	TriangleFace& getFace(const size_t i)	{ return faces[i]; 			}

	bool	 		getVertex(const size_t i, vector3& v);
	bool			setVertex(const size_t i, const vector3& v);
	bool			getNormal(const size_t i, vector3& v);
	bool			setNormal(const size_t i, const vector3& v);
	bool			getTexCoord(const size_t i, vector2& v);
	bool			setTexCoord(const size_t i, const vector2& v);

	bool			getFace(const size_t i, TriangleFace& face);
	bool			setFace(const size_t i, const TriangleFace& face);

	const size_t		getRenderElementCount() const;

	bool				createRenderBuffers();
	bool				hasRenderBuffers() const;
	void				destroyRenderBuffers();

	friend class TriangleFace;
};

inline bool SubMesh::setFace(const size_t i, const TriangleFace& face)
{
	if(i >= faces.size())
		return false;
	faces[i] = face;
	faces[i].setMesh(this);
	return true;
}

inline const uint32_t& TriangleFace::getVertexIndex(const size_t i) const
{ return vertex[i]; 	}

inline const uint32_t& TriangleFace::getNormalIndex(const size_t i) const
{ return normal[i]; 	}

inline const uint32_t& TriangleFace::getTexCoordIndex(const size_t i) const
{ return texcoord[i]; 	}

inline const bool TriangleFace::getVertexIndex(const size_t i, uint32_t &index)
{
	if(i >= 3) return false;
	index = vertex[i];
	return true;
}

inline const bool TriangleFace::getNormalIndex(const size_t i, uint32_t &index)
{
	if(i >= 3) return false;
	index = normal[i];
	return true;
}

inline const bool TriangleFace::getTexCoordIndex(const size_t i, uint32_t &index)
{
	if(i >= 3) return false;
	index = texcoord[i];
	return true;
}

inline bool TriangleFace::setVertexIndex(const size_t i, const uint32_t index)
{
	if(i >= 3) return false;
	vertex[i] = index;
	return true;
}

inline bool TriangleFace::setNormalIndex(const size_t i, const uint32_t index)
{
	if(i >= 3) return false;
	normal[i] = index;
	return true;
}

inline bool TriangleFace::setTexCoordIndex(const size_t i, const uint32_t index)
{
	if(i >= 3) return false;
	texcoord[i] = index;
	return true;
}

inline vector3& TriangleFace::getVertex(const size_t i)
{ return mesh->vertices[vertex[i]]; 		}

inline vector3& TriangleFace::getNormal(const size_t i)
{
	return mesh->normals[normal[i]];
}

inline vector2& TriangleFace::getTexCoord(const size_t i)
{ return mesh->texcoords[texcoord[i]];	}

inline bool TriangleFace::getVertex(const size_t i, vector3& v)
{
	if(i >= 3) return false;
	v = mesh->vertices[vertex[i]];
	return true;
}

inline bool TriangleFace::getNormal(const size_t i, vector3& v)
{
	if(i >= 3) return false;
	v = mesh->normals[normal[i]];
	return true;
}

inline bool TriangleFace::getTexCoord(const size_t i, vector2& v)
{
	if(i >= 3) return false;
	v = mesh->texcoords[texcoord[i]];
	return true;
}

inline bool TriangleFace::setVertex(const size_t i, const vector3& v)
{
	if(i >= 3) return false;
	mesh->vertices[vertex[i]] = v;
	return true;
}

inline bool TriangleFace::setNormal(const size_t i, const vector3& v)
{
	if(i >= 3) return false;
	mesh->normals[normal[i]] = v;
	return true;
}

inline bool TriangleFace::setTexCoord(const size_t i, const vector2& v)
{
	if(i >= 3) return false;
	mesh->texcoords[texcoord[i]] = v;
	return true;
}inline bool SubMesh::getVertex(const size_t i, vector3& v)
{
	if(i >= vertices.size())
		return false;
	v = vertices[i];
	return true;
}

inline bool SubMesh::setVertex(const size_t i, const vector3& v)
{
	if(i >= vertices.size())
		return false;
	vertices[i] = v;
	return true;
}

inline bool SubMesh::getNormal(const size_t i, vector3& v)
{
	if(i >= normals.size())
		return false;
	v = normals[i];
	return true;
}

inline bool SubMesh::setNormal(const size_t i, const vector3& v)
{
	if(i >= normals.size())
		return false;
	normals[i] = v;
	return true;
}

inline bool SubMesh::getTexCoord(const size_t i, vector2& v)
{
	if(i >= texcoords.size())
		return false;
	v = texcoords[i];
	return true;
}

inline bool SubMesh::setTexCoord(const size_t i, const vector2& v)
{
	if(i >= texcoords.size())
		return false;
	texcoords[i] = v;
	return true;
}

inline bool SubMesh::getFace(const size_t i, TriangleFace& face)
{
	if(i >= faces.size())
		return false;
	face = faces[i];
	return true;
}

} // Vorticity

#endif
