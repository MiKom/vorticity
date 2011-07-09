#ifndef __VORTICITY_MESH_H
#define __VORTICITY_MESH_H

namespace Vorticity {

class FileReader;
class SubMesh;
class Material;

class VAPI Mesh : public Resource
{
protected:
	std::vector<SubMesh*>	subMeshes;
	std::vector<Material*>	materials;
public:
	Mesh(FileReader& reader);
	virtual ~Mesh();

	virtual string getType() const
	{ return "mesh"; }

	size_t getSubMeshCount() const	{ return subMeshes.size(); }
	size_t getMaterialCount() const	{ return materials.size(); }

	void		addSubMesh(SubMesh* submesh);
	SubMesh* 	getSubMesh(const size_t index);
	bool		removeSubMesh(const size_t index);

	void		addMaterial(Material* mat);
	Material* 	getMaterial(const size_t index);
	bool		removeMaterial(const size_t index);

	void		clearSubMeshes();
	void		clearMaterials();

	bool		createRenderBuffers();
	void		destroyRenderBuffers();
};

} // Vorticity

#endif