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
#include<fstream>
#include"avrmath.h"
#include"avrtexture.h"
#include"avrmaterial.h"
#include"avrmesh.h"
#include"avrutil.h"
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

AVRFile::AVRError AVRFile::writeGeometry(std::ostream& os)
{
	os.write("GEOM", 4);
	auto sizePos = os.tellp();
	os.seekp(4, std::ios::cur);
	auto startPos = os.tellp();
	
	uint32_t nMeshes = mMeshes.size();
	os.write((char*) &nMeshes, 4);
	
	for(int i=0; i<mMeshes.size(); i++) {
		write(os, *mMeshes[i]);
	}
	
	auto endPos = os.tellp();
	uint32_t size = endPos - startPos;
	os.seekp(sizePos);
	os.write((char*) &size, 4);
	os.seekp(endPos);
	return AVRError::None;
}
AVRFile::AVRError AVRFile::writeMaterials(std::ostream& os)
{
	auto startPos = startChunk(os, "MATS");
	
	uint32_t nMaterials = mMaterials.size();
	os.write((char*) &nMaterials, 4);
	
	for(int i=0; i<mMaterials.size(); i++) {
		write(os, mMaterials[i]);
	}
	
	endChunk(os, "MATS", startPos);
	return AVRError::None;
}
AVRFile::AVRError AVRFile::writeTextures(std::ostream& os)
{
	auto startPos = startChunk(os, "TEXS");
	
	uint32_t nTextures = mTextures.size();
	os.write((char*) &nTextures, 4);
	
	for(int i=0; i<mTextures.size(); i++) {
		write(os, mTextures[i]);
	}
	endChunk(os, "TEXS", startPos);
	return AVRError::None;
}

AVRFile::AVRError AVRFile::save(const std::string& path)
{
	//TODO: add error handling
	std::ofstream file(path, std::ios::binary);
	auto startPos = startChunk(file, "VMSH");
	uint16_t version = 1;
	file.write((char*) &version, 2);
	
	writeGeometry(file);
	writeMaterials(file);
	writeTextures(file);

	endChunk(file, "VMSH", startPos);
	file.close();
	return AVRError::None;
}
