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

#include<vector>
#include<ostream>
#include"avrmath.h"
#include"avrtexture.h"
#include"avrmaterial.h"
#include"avrmesh.h"
#include"avrfile.h"

using namespace AVR;

void AVRFile::addMaterial(const AVRMaterial& material)
{
	mMaterials.push_back(material);
}

void AVRFile::addMesh(AVRMesh* mesh)
{
	mMeshes.push_back(mesh);
}

void AVRFile::addTexture(const AVRTexture& texture)
{
	mTextures.push_back(texture);
}

AVRFile::AVRResult AVRFile::save(const std::string& path)
{
	//TODO: implement
	return AVR_OK;
}
