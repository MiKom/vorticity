#ifndef __VORTICITY_NODE_SHADER_H
#define __VORTICITY_NODE_SHADER_H

namespace Vorticity {

class VAPI NodeShader : public Modifier
{
protected:
	Shader* shader;
public:
	NodeShader(const string& name, XNode* parent=NULL);
	virtual ~NodeShader();

	virtual string getType() const
	{ return "shader"; }
	Shader* getShader() const
	{ return shader; }

	virtual bool	bind(Resource *shader);
	virtual void	evaluate();
};

} // Vorticity

#endif