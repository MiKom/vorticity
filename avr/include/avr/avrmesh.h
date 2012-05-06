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
#pragma once

#if !defined (__AVRXX_H_INSIDE__) && !defined (AVR_COMPILATION)
#error "Only <avr/avr++.h> can be included directly."
#endif

namespace AVR
{

struct _AVRFace
{
	int32_t vertIdx[3];
	int32_t normalIdx[3];
	int32_t textCoordIdx[3];
};
typedef struct _AVRFace AVRFace;

void write(std::ostream& os, const AVRFace& face);

class AVRMesh
{
protected:
	std::string mName;
	std::vector<AVRFace> mFaces;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextCoords;
	int mMaterialId;
public:
	std::string getName() const { return mName; }
	void setName(const std::string& name) { mName = name; }

	void addVertex(const vec3& vertex);
	void addVertices(const std::vector<vec3> &vertices);
	void clearVertices();

	void addNormal(const vec3& normal);
	void addNormals(const std::vector<vec3>& normals);
	void clearNormals();

	void addTextCoord(const vec2& textCoord);
	void addTextCoords(const std::vector<vec2>& textCoords);
	void clearTextCoords();

	void addFace(const AVRFace& face);
	void addFaces(const std::vector<AVRFace>& faces);
	void clearFaces();
	
	int getMaterialId() const { return mMaterialId; }
	void setMaterialId(const int& materialId) { mMaterialId = materialId; }
	
	friend void write(std::ostream& os, const AVRMesh& mesh);
};
void write(std::ostream& os, const AVRMesh& mesh);
} //AVR
