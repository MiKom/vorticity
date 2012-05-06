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

class AVRFile
{
	enum AVRResult {
		AVR_OK,
		AVR_EPERM,
		AVR_EFULL,
		AVR_ERROR
	};
	
protected:
	std::vector<AVRMaterial> mMaterials;
	std::vector<AVRMesh*> mMeshes;
	std::vector<AVRTexture> mTextures;
	
public:
	std::vector<AVRMaterial> getMaterials() const { return mMaterials; }
	void addMaterial(const AVRMaterial& material);
	
	std::vector<AVRMesh*> getMeshes() const { return mMeshes; }
	void addMesh(AVRMesh* mesh);
	
	std::vector<AVRTexture> getTextures() const { return mTextures; }
	void addTexture(const AVRTexture& texture);
	
	AVRResult save(const std::string& path);
};

} //AVR
