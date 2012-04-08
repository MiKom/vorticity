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

#include"config.h"
#include"vorticity/resources/resource.h"
#include"vorticity/resources/material.h"
#include"vorticity/resources/mesh.h"

using namespace Vorticity;

Material::Material(const string& name) : Resource(), name(name)
{
	this->ambient = vector3(0.6f, 0.6f, 0.6f);
	this->diffuse = vector3(0.6f, 0.6f, 0.6f);

	this->glossiness = 10.0f;
	this->specularLevel = 0.0f;

	std::memset(textures, 0, sizeof(textures));
}

Material::~Material()
{

}

Image* Material::getTextureChannel(const size_t i)
{
	if(i >= MaxTextureChannels)
		return NULL;
	return textures[i];
}

bool Material::setTextureChannel(const size_t i, Image* img)
{
	if(i >= MaxTextureChannels)
		return false;
	textures[i] = img;
	return true;
}
