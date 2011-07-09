#include"config.h"
#include"resource.h"
#include"material.h"
#include"mesh.h"

using namespace Vorticity;

Material::Material(const string& name) : Resource(), name(name)
{
	this->ambient = vector3(0.6f, 0.6f, 0.6f);
	this->diffuse = vector3(0.6f, 0.6f, 0.6f);

	this->glossiness = 10.0f;
	this->specularLevel = 0.0f;

	std::memset(textures, NULL, sizeof(textures));
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