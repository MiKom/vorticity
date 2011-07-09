#ifndef __VORTICITY_STDMESH_LOADER_H
#define __VORTICITY_STDMESH_LOADER_H

namespace Vorticity {

/**
 * \author Mi³osz Kosobucki
 *
 * Mesh loader for Vorticity AVR format, for description 
 * see doc/new_model_format.txt
 **/
class SubMesh;
class Mesh;
class Material;

class VAPI StdMeshLoader : public IFFReader<StdMeshLoader>
{
protected:
	std::vector<std::pair<uint32_t, SubMesh*>> meshes;
	std::map<Material*, std::vector<uint32_t>> materials;
	std::vector<std::string> textures;
protected:
	bool VMSHCallback();
	bool GEOMCallback();
	bool MESHCallback();
	bool MATSCallback();
	bool MATRCallback();
	bool TEXSCallback();
	bool TEXTCallback();

	void postload();
	
	/**
	 * Gets properly casted Mesh that is being furnished
	 **/
	Mesh* getMesh()
	{ return (Mesh*)getResource(); }

public:
	static const uint16_t LATEST_VERSION = 0x0001;
	StdMeshLoader(const std::string& path);
	virtual ~StdMeshLoader();
};

} // Vorticity

#endif