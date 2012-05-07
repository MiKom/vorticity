/* 
 * Copyright © 2012 Miłosz Kosobucki
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

#include <vector>
#include <ostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include "avrmath.h"
#include "avrmesh.h"

using namespace AVR;
using namespace std;

void AVR::write(ostream& os, const AVRFace& face)
{
	for(int i=0; i<3; i++) {
		uint32_t idx = face.vertIdx[i];
		os.write((char*) &idx, 4);
	}
	for(int i=0; i<3; i++) {
		uint32_t idx = face.normalIdx[i];
		os.write((char*) &idx, 4);
	}
	for(int i=0; i<3; i++) {
		uint32_t idx = face.textCoordIdx[i];
		os.write((char*) &idx, 4);
	}
}


AVRMesh::AVRMesh() : 
	mMaterialId(0), mFaces(new vector<AVRFace>()),
	mVertices(new vector<vec3>()), mNormals(new vector<vec3>()),
	mTextCoords(new vector<vec2>())
{
}

AVRMesh::~AVRMesh()
{
	delete mFaces;
	delete mVertices;
	delete mNormals;
	delete mTextCoords;
}

void AVRMesh::addVertex(const vec3& vertex)
{
	mVertices->push_back(vertex);
}
void AVRMesh::addVertices(const std::vector<vec3>& vertices)
{
	mVertices->insert(mVertices->end(), vertices.begin(), vertices.end());
}
void AVRMesh::clearVertices()
{
	mVertices->clear();
}

void AVRMesh::addNormal(const vec3& normal)
{
	mNormals->push_back(normal);
}
void AVRMesh::addNormals(const std::vector<vec3>& normals)
{
	mNormals->insert(mNormals->end(), normals.begin(), normals.end());
}
void AVRMesh::clearNormals()
{
	mNormals->clear();
}

void AVRMesh::addTextCoord(const vec2& textCoord)
{
	mTextCoords->push_back(textCoord);
}
void AVRMesh::addTextCoords(const std::vector<vec2>& textCoords)
{
	mTextCoords->insert(mTextCoords->end(),
	                    textCoords.begin(), textCoords.end());
}
void AVRMesh::clearTextCoords()
{
	mTextCoords->clear();
}

void AVRMesh::addFace(const AVRFace& face)
{
	mFaces->push_back(face);
}
void AVRMesh::addFaces(const std::vector<AVRFace>& faces)
{
	mFaces->insert(mFaces->end(), faces.begin(), faces.end());
}
void AVRMesh::clearFaces()
{
	mFaces->clear();
}

void AVR::write(std::ostream& os, const AVRMesh& mesh)
{
	os.write("MESH", 4);
	auto sizePos = os.tellp();
	os.seekp(4, ios::cur);
	auto startPos = os.tellp();
	
	os.write(mesh.mName.c_str(), mesh.mName.length() + 1);
	
	uint32_t nFaces = mesh.mFaces->size();
	os.write((char*) &nFaces, 4);
	
	uint32_t nVertices = mesh.mVertices->size();
	os.write((char*) &nVertices, 4);
	
	uint32_t nNormals = mesh.mNormals->size();
	os.write((char*) &nNormals, 4);
	
	uint32_t nTextCoords = mesh.mTextCoords->size();
	os.write((char*) &nTextCoords, 4);
	
	uint32_t materialId = static_cast<uint32_t>(mesh.mMaterialId);
	os.write((char*) &materialId, 4);
	
	for(int i=0; i<mesh.mFaces->size(); i++) {
		write(os, mesh.mFaces->at(i));
	}
	for(int i=0; i<mesh.mVertices->size(); i++) {
		os.write((char*) mesh.mVertices->at(i).cell, 4 * 3);
	}
	for(int i=0; i<mesh.mNormals->size(); i++) {
		os.write((char*) mesh.mNormals->at(i).cell, 4 * 3);
	}
	for(int i=0; i<mesh.mTextCoords->size(); i++) {
		os.write((char*) mesh.mTextCoords->at(i).cell, 4 * 2);
	}
	
	auto endPos = os.tellp();
	uint32_t size = endPos - startPos;
	os.seekp(sizePos);
	os.write((char*) &size, 4);
	os.seekp(endPos);
}
