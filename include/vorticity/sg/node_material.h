#ifndef __VORTICITY_NODE_MATERIAL_H
#define __VORTICITY_NODE_MATERIAL_H

namespace Vorticity {

class Material;
class VAPI NodeMaterial : public Modifier
{
public:
	NodeMaterial(const string& name, XNode *parent=NULL);
	virtual ~NodeMaterial();

	virtual string getType() const
	{ return "material"; }

	virtual void setSampler(const string& name, const int texture);
	virtual int	 getSampler(const string& name) const;
	virtual void setupShader(const int shader, Renderer *device);

	virtual void evaluate();
	virtual bool bind(Resource *res);
protected:
	std::map<string, int> samplers;
};

} // Vorticity

#endif