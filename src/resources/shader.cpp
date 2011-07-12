/* 
 * Copyright © 2009, 2010 Michał Siejak
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

#include "config.h"
#include "vorticity/resources/resource.h"
#include "vorticity/io/iobase.h"
#include "vorticity/resources/shader.h"

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
