/* 
 * Copyright © 2010 Michał Siejak
 * Copyright © 2010 Miłosz Kosobucki
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
#include "vorticity/resources/resource.h"
#include "vorticity/resources/material.h"
#include "vorticity/resources/mesh.h"
#include "vorticity/resources/submesh.h"
#include "vorticity/io/iobase.h"
#include "vorticity/io/iff_reader.h"
#include "vorticity/io/stdmesh_loader.h"
#include "vorticity/resources/rset.h"
#include "vorticity/resources/image.h"
#include "vorticity/io/image_loader.h"

using namespace Vorticity;

StdMeshLoader::StdMeshLoader(const std::string& path) : IFFReader(path)
{
	registerChunkHandler(ID("VMSH"), ChunkHandler<StdMeshLoader>(this, &StdMeshLoader::VMSHCallback));
	registerChunkHandler(ID("GEOM"), ChunkHandler<StdMeshLoader>(this, &StdMeshLoader::GEOMCallback));
	registerChunkHandler(ID("MESH"), ChunkHandler<StdMeshLoader>(this, &StdMeshLoader::MESHCallback));
	registerChunkHandler(ID("MATS"), ChunkHandler<StdMeshLoader>(this, &StdMeshLoader::MATSCallback));
	registerChunkHandler(ID("MATR"), ChunkHandler<StdMeshLoader>(this, &StdMeshLoader::MATRCallback));
	registerChunkHandler(ID("TEXS"), ChunkHandler<StdMeshLoader>(this, &StdMeshLoader::TEXSCallback));
	registerChunkHandler(ID("TEXT"), ChunkHandler<StdMeshLoader>(this, &StdMeshLoader::TEXTCallback));
}

StdMeshLoader::~StdMeshLoader()
{
}

#if 1

bool StdMeshLoader::VMSHCallback ()
{
	uint16_t version;
	file.read((char *) &version, 2);
	if(version > LATEST_VERSION)
		throw IOException("VMSH version mismatch", IOBase::path, IOBase::ReadError);
	else
		return true;
}

bool StdMeshLoader::GEOMCallback()
{
	Mesh *mesh = static_cast<Mesh *> (getResource());
	uint32_t meshes;
	file.read((char *) &meshes, 4);
	return true;
}
bool StdMeshLoader::MESHCallback()
{
	uint32_t faceCount, vertexCount, normalCount, textCoordCount;
	uint32_t materialId;
	string name;

	name.read(file);
	
	SubMesh* mesh = new SubMesh(name, getMesh());
	file.read((char *) &faceCount, 			4);
	file.read((char *) &vertexCount, 		4);
	file.read((char *) &normalCount, 		4);
	file.read((char *) &textCoordCount, 	4);
	file.read((char *) &materialId, 		4);

	meshes.push_back(std::pair<uint32_t, SubMesh*>(materialId, mesh));

	mesh->setFaceCount(faceCount);
	mesh->setVertexCount(vertexCount);
	mesh->setNormalCount(normalCount);
	mesh->setTexCoordCount(textCoordCount);

	for(size_t i=0; i<faceCount; i++)
	{
		TriangleFace face;
		//vertex indices
		file.read((char *) face.getVertexIndices(), sizeof(uint32_t) * 3);

		//normal indices
		file.read((char *) face.getNormalIndices(), sizeof(uint32_t) * 3);

		//UV indices
		file.read((char *) face.getTexCoordIndices(), sizeof(uint32_t) * 3);

		mesh->setFace(i, face);
	}

	for(size_t i=0; i<vertexCount; i++)
	{
		vector3 vert;
		vert.read(file);
		mesh->setVertex(i, vert);
	}

	for(size_t i=0; i<normalCount; i++)
	{
		vector3 normal;
		normal.read(file);
		mesh->setNormal(i, normal);
	}

	for(size_t i=0; i<textCoordCount; i++)
	{
		vector2 uv;
		uv.read(file);
		mesh->setTexCoord(i, uv);
	}
	return true;
	
}
bool StdMeshLoader::MATSCallback()
{
	uint32_t materialCount;
	file.read((char *) &materialCount, 4);

	return true;
}
bool StdMeshLoader::MATRCallback()
{
	string name;
	name.read(file);
	Material *mat = new Material(name);
	
	vector3 diffuse;  diffuse.read(file); 	mat->setDiffuse(diffuse);
	vector3 specular; specular.read(file); 	mat->setSpecular(specular);
	vector3 ambient;  ambient.read(file); 	mat->setAmbient(ambient);
	vector3 emissive; emissive.read(file); 	mat->setEmissive(emissive);

	float opacity; file.read((char *) &opacity, sizeof(float)); mat->setOpacity(opacity);
	
	float specularLevel; file.read((char *) &specularLevel, sizeof(float)); 
	mat->setSpecularLevel(specularLevel);

	float glossiness; file.read((char *) &glossiness, sizeof(float));
	mat->setGlossiness(glossiness);
	
	uint32_t textureCount;
	file.read((char *) &textureCount, 4);
	for(size_t i=0; i<textureCount; i++)
	{
		uint32_t textureId;
		file.read((char *) &textureId, 4);
		materials[mat].push_back(textureId);
	}

	size_t matIndex = getMesh()->getMaterialCount();
	getMesh()->addMaterial(mat);

	for(auto i=meshes.begin(); i<meshes.end(); i++)
		if(i->first == matIndex)
			i->second->setMaterial(mat);
	return true;
}
bool StdMeshLoader::TEXSCallback()
{
	uint32_t textureCount;
	file.read((char *) &textureCount, 4);
	textures.reserve(textureCount);
	return true;
}
bool StdMeshLoader::TEXTCallback()
{
	string path;
	path.read(file);
	textures.push_back(path);
	return true;
}

#endif

void StdMeshLoader::postload()
{
	for(auto it=materials.begin(); it != materials.end(); it++)
	{
		size_t count = std::min(it->second.size(), static_cast<size_t>(Material::MaxTextureChannels));
		for(size_t i=0; i<count; i++)
		{
			//HACK
			ImageLoader* loader = new ImageLoader(textures.at(it->second.at(i)));
			it->first->setTextureChannel(i, getCore()->getCurrentResourceSet()->loadResource<Image>(*loader));
		}
	}
}
