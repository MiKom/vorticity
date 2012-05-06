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

void write(ostream& os, const AVRFace& face)
{
	for(int i=0; i<3; i++) {
		int32_t idx = face.vertIdx[i];
		os.write((char*) &idx, 4);
	}
	for(int i=0; i<3; i++) {
		int32_t idx = face.normalIdx[i];
		os.write((char*) &idx, 4);
	}
	for(int i=0; i<3; i++) {
		int32_t idx = face.textCoordIdx[i];
		os.write((char*) &idx, 4);
	}
}

void AVRMesh::addVertex(const vec3& vertex)
{
	mVertices.push_back(vertex);
}
void AVRMesh::addVertices(const std::vector<vec3>& vertices)
{
	mVertices.insert(mVertices.end(), vertices.begin(), vertices.end());
}
void AVRMesh::clearVertices()
{
	mVertices.clear();
}

void AVRMesh::addNormal(const vec3& normal)
{
	mNormals.push_back(normal);
}
void AVRMesh::addNormals(const std::vector<vec3>& normals)
{
	mNormals.insert(mNormals.end(), normals.begin(), normals.end());
}
void AVRMesh::clearNormals()
{
	mNormals.clear();
}

void AVRMesh::addTextCoord(const vec2& textCoord)
{
	mTextCoords.push_back(textCoord);
}
void AVRMesh::addTextCoords(const std::vector<vec2>& textCoords)
{
	mTextCoords.insert(mTextCoords.end(),
	                   textCoords.begin(), textCoords.end());
}
void AVRMesh::clearTextCoords()
{
	mTextCoords.clear();
}

void AVRMesh::addFace(const AVRFace& face)
{
	mFaces.push_back(face);
}
void AVRMesh::addFaces(const std::vector<AVRFace>& faces)
{
	mFaces.insert(mFaces.end(), faces.begin(), faces.end());
}
void AVRMesh::clearFaces()
{
	mFaces.clear();
}

void AVR::write(std::ostream& os, const AVRMesh& mesh)
{
	os.write("MESH", 4);
	auto sizePos = os.tellp();
	os.seekp(4, ios::cur);
	auto startPos = os.tellp();
	
	os.write(mesh.mName.c_str(), mesh.mName.length());
	
	int32_t nFaces = mesh.mFaces.size();
	os.write((char*) &nFaces, 4);
	
	int32_t nVertices = mesh.mVertices.size();
	os.write((char*) &nVertices, 4);
	
	int32_t nNormals = mesh.mNormals.size();
	os.write((char*) &nNormals, 4);
	
	int32_t nTextCoords = mesh.mTextCoords.size();
	os.write((char*) &nTextCoords, 4);
	
	int32_t materialId = static_cast<int32_t>(mesh.mMaterialId);
	os.write((char*) &materialId, 4);
	
	for(int i=0; i<mesh.mFaces.size(); i++) {
		write(os, mesh.mFaces[i]);
	}
	
	for(int i=0; i<mesh.mVertices.size(); i++) {
		os.write((char*) mesh.mVertices[i].cell, 4 * 3);
	}
	
	for(int i=0; i<mesh.mNormals.size(); i++) {
		os.write((char*) mesh.mNormals[i].cell, 4 * 3);
	}
	for(int i=0; i<mesh.mTextCoords.size(); i++) {
		os.write((char*) mesh.mTextCoords[i].cell, 4 * 2);
	}
	
	auto endPos = os.tellp();
	int32_t size = endPos - startPos;
	os.seekp(sizePos);
	os.write((char*) &size, 4);
	os.seekp(endPos);
}
