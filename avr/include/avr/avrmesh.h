#pragma once

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
