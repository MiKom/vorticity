#pragma once

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
