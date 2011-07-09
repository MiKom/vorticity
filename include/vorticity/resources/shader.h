#ifndef __VORTICITY_RES_SHADER_H
#define __VORTICITY_RES_SHADER_H

namespace Vorticity {

class Renderer;
class FileReader;
class NodeShader;

class VAPI Shader : public TextResource
{
public:
	Shader(FileReader& reader);
	virtual ~Shader();

	virtual string getType() const
	{ return "shader"; }
};

} // Vorticity

#endif