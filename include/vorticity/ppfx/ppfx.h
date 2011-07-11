/* 
 * Copyright © 2009, 2010 Micha³ Siejak
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

#ifndef __VORTICITY_PPFX_H
#define __VORTICITY_PPFX_H

namespace Vorticity {

class Renderer;
class NodeCamera;
class NodePostProcess;

typedef std::map<string, NodeCamera*>	FrameSourceMap;
typedef std::map<string, Shader*>		FrameShaderMap;

class VAPI PostProcess : public Entity
{
protected:
	Renderer*			device;
	NodePostProcess*	node;
	FrameSourceMap		sources;
	FrameShaderMap		shaders;
protected:
	PostProcess(Renderer* device=NULL);
	virtual bool		acceptSource(const string& name) = 0;
	virtual bool		acceptShader(const string& name) = 0;
	virtual void		setShaderParameters(Shader *shader);

	virtual NodeCamera*	getCheckSource(const string &name) const;
	virtual Shader*		getCheckShader(const string &name) const;
	virtual bool		validateBuffer(int &buffer, FramebufferFormat format, const int w, const int h);
	virtual bool		validateBuffer(int &buffer, FramebufferFormat format, const NodeCamera *src);

	inline bool			setSampler(const int shader, const string& name, const int texture);
	int					beginBuffer(const int buffer, const int shader);
	int					swapBuffer(const int buffer, const int shader);
	inline  int			swapBuffer(const int buffer);
	inline	void		endBuffer();
public:
	virtual ~PostProcess();

	virtual string	getClass() const
	{ return "ppfx"; }

	Renderer* getDevice() const
	{ return device; }
	NodePostProcess* getNode() const
	{ return node; }

	virtual bool		addSource(const string& name, NodeCamera* cam);
	virtual bool		removeSource(const string &name);
	virtual NodeCamera*	getSource(const string& name) const;

	virtual bool		addShader(const string& name, Shader *shader);
	virtual bool		removeShader(const string& name);
	virtual Shader*		getShader(const string& name) const;

	virtual NodePostProcess& createNode(const string& name, XNode *parent=NULL);

	virtual int		render() = 0;
	virtual bool	validate() = 0;
};

} // Vorticity

#endif