#include "config.h"
#include "resource.h"
#include "io\iobase.h"
#include "shader.h"

using namespace Vorticity;

Shader::Shader(FileReader& reader) : TextResource()
{
	reader.setResource(this);
	reader.readFile();
	if(getCore()->getDevice()->createShader(this) == 0)
		throw DeviceException("Shader compilation failed", reader.getPath());
}

Shader::~Shader()
{
	if(device)
		device->destroyShader(id);
}
