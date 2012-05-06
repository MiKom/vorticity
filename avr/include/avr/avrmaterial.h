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

#if !defined(__AVRXX_H_INSIDE__) && !defined(AVR_COMPILATION)
#error "Only <avr/avr++.h> can be included directly."
#endif

namespace AVR
{
class AVRMaterial
{
protected:
	std::string mName;
	vec3 mDiffuse;
	vec3 mSpecular;
	vec3 mAmbient;
	vec3 mEmissive;
	float mOpacity;
	float mSpecularLevel;
	float mGlossiness;
	std::vector<int> mTextureIds;

public:
	std::string getName() const;
	void setName(const std::string name);

	vec3 getDiffuse() const;
	void setDiffuse(const float r, const float g, const float b);
	void setDiffuse(const vec3& color);

	vec3 getSpecular() const;
	void setSpecular(const float r, const float g, const float b);
	void setSpecular(const vec3& color);

	vec3 getAmbient() const;
	void setAmbient(const float r, const float g, const float b);
	void setAmbient(const vec3& color);

	vec3 getEmissive() const;
	void setEmissive(const float r, const float g, const float b);
	void setEmissive(const vec3& color);

	float getOpacity() const;
	void setOpacity(const float opacity);

	float getGlossiness() const;
	void setGlossiness(const float glossiness);

	std::vector<int> getTextureIds() const;
	void addTextureId(const int id);
	void removeTextureId(const int id);

	friend void write(std::ostream& os, const AVRMaterial& mat);
private:
};
void write(std::ostream& os, const AVRMaterial& mat);
} //AVR
