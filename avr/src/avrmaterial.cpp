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
#include "avrmaterial.h"

using namespace AVR;

void AVRMaterial::setName(const std::string name)
{
	mName = name;
}

vec3 AVRMaterial::getDiffuse() const
{
	return mDiffuse;
}
void AVRMaterial::setDiffuse(const float r, const float g, const float b)
{
	mDiffuse.r = r;
	mDiffuse.g = g;
	mDiffuse.b = b;
}
void AVRMaterial::setDiffuse(const vec3 &color)
{
	mDiffuse = color;
}

vec3 AVRMaterial::getSpecular() const
{
	return mSpecular;
}
void AVRMaterial::setSpecular(const float r, const float g, const float b)
{
	mSpecular.r = r;
	mSpecular.g = g;
	mSpecular.b = b;
}
void AVRMaterial::setSpecular(const vec3 &color)
{
	mSpecular = color;
}

vec3 AVRMaterial::getAmbient() const
{
	return mAmbient;
}
void AVRMaterial::setAmbient(const float r, const float g, const float b)
{
	mAmbient.r = r;
	mAmbient.g = g;
	mAmbient.b = b;
}
void AVRMaterial::setAmbient(const vec3 &color)
{
	mAmbient = color;
}

vec3 AVRMaterial::getEmissive() const
{
	return mEmissive;
}
void AVRMaterial::setEmissive(const float r, const float g, const float b)
{
	mEmissive.r = r;
	mEmissive.g = g;
	mEmissive.b = b;
}
void AVRMaterial::setEmissive(const vec3 &color)
{
	mEmissive = color;
}

float AVRMaterial::getOpacity() const
{
	return mOpacity;
}
void AVRMaterial::setOpacity(const float opacity)
{
	mOpacity = opacity;
}

float AVRMaterial::getSpecularLevel() const
{
	return mSpecularLevel;
}
float AVRMaterial::setSpecularLevel(const float specularLevel)
{
	mSpecularLevel = specularLevel;
}
float AVRMaterial::getGlossiness() const
{
	return mGlossiness;
}
void AVRMaterial::setGlossiness(const float glossiness)
{
	mGlossiness = glossiness;
}

std::vector<int> AVRMaterial::getTextureIds() const
{
	return mTextureIds;
}
void AVRMaterial::addTextureId(const int id)
{
	mTextureIds.push_back(id);
}
void AVRMaterial::removeTextureId(const int id)
{
	mTextureIds.erase(std::remove_if(mTextureIds.begin(), mTextureIds.end(), [&](int _id) {
		return _id == id;
	}), mTextureIds.end());
}

void AVR::write(std::ostream& os, const AVRMaterial& mat)
{
	os.write("MATR", 4);
	std::ios::pos_type sizePos = os.tellp();
	os.seekp(4, std::ios::cur);
	std::ios::pos_type startPos = os.tellp();

	os.write(mat.mName.c_str(), mat.mName.length() + 1);
	os.write((char*) mat.mDiffuse.cell, 4 * 3);
	os.write((char*) mat.mSpecular.cell, 4 * 3);
	os.write((char*) mat.mAmbient.cell, 4 * 3);
	os.write((char*) mat.mEmissive.cell, 4 * 3);
	os.write((char*) &mat.mOpacity, 4);
	os.write((char*) &mat.mSpecularLevel, 4);
	os.write((char*) &mat.mGlossiness, 4);

	uint32_t textCount = static_cast<uint32_t>(mat.mTextureIds.size());
	os.write((char*) &textCount, 4);
	for(int i=0; i<mat.mTextureIds.size(); i++) {
		uint32_t textId = static_cast<uint32_t>(mat.mTextureIds[i]);
		os.write((char*) &textId, 4);
	}

	auto endPos = os.tellp();
	uint32_t size = endPos - startPos;
	os.seekp(sizePos);
	os.write((char*) &size, 4);
	os.seekp(endPos);
}
