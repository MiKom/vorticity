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

#ifndef __VORTICITY_RENDERER_H
#define __VORTICITY_RENDERER_H

namespace Vorticity {

class AnyAttribute;
class ArrayAttribute;
class Node;
class Overlay;

typedef void* ShaderPass;

typedef enum
{
	ClearColor,
	ClearDepth,
	ClearStencil,
	ClearColorDepth,
	ClearAll,
} ClearMode;

typedef enum
{
	ModelviewMatrix,
	ProjectionMatrix,
	ModelviewProjMatrix,
	NormalMatrix,
} MatrixMode;

typedef enum
{
	NoFilter,
	Bilinear,
	Trilinear,
} TextureFilter;

typedef enum
{
	ColorBuffer = 0,
	DepthBuffer = 1,
	StencilBuffer = 2,
} RenderbufferType;

typedef enum
{
	BufferRGBA,
	BufferFloat16,
	BufferFloat32,
} FramebufferFormat;

typedef enum
{
	VertexBuffer,
	NormalBuffer,
	TexCoordBuffer,
} VertexBufferType;

typedef enum
{
	WriteBuffer,
	ReadBuffer,
} PixelBufferType;

// Resources
class Shader;
class Image;

class DeviceException : public std::runtime_error
{
protected:
	std::string object_info;
public:
	DeviceException(const std::string& msg, const std::string& obj="")
		: std::runtime_error(msg), object_info(obj)
	{ }

	const std::string& object() const
	{ return object_info; }
};

class VAPI Renderer
{
protected:
	CGcontext	cgContext;
	Overlay*	overlay;
	int screenWidth, screenHeight;

	// To ¿eby siê nie pomyliæ :P
	static __forceinline int SETID(const int id) { return id+1; }
	static __forceinline int GETID(const int id) { return id-1; }
	static __forceinline bool CHECKID(const int id, const int max)
	{ if(id <= 0 || id > max) return false; return true; }

	Renderer() : screenWidth(0), screenHeight(0), cgContext(NULL)
	{ }
public:
	virtual ~Renderer() { }

	Overlay*		getOverlay() const				{ return overlay;	}
	void			setOverlay(Overlay* ovr)		{ overlay = ovr;	}

	CGcontext		getCGContext() const			{ return cgContext; }
	int				getScreenWidth() const			{ return screenWidth; }
	int				getScreenHeight() const			{ return screenHeight; }
	void			setScreenWidth(const int w)		{ screenWidth = w;	}
	void			setScreenHeight(const int h)	{ screenHeight = h; }

	virtual const char* getVendorString() const = 0;
	virtual const char*	getRendererString() const = 0;
	virtual const char* getCompilationLog() const = 0;

	virtual void	clear(ClearMode mode) = 0;
	virtual void	clearColor(const float r, const float g, const float b, const float a) = 0;
	virtual void	viewport(const int xoffset, const int yoffset, const int w, const int h) = 0;
	virtual void	setPerspectiveProjection(const float fov, const float aspect, const float zNear, const float zFar) = 0;
	virtual void	setOrthoProjection(const float xmin, const float xmax, const float ymin, const float ymax) = 0;
	virtual void 	setIdentityProjection() = 0;
	virtual void 	setProjection(const float* matrix) = 0;

	virtual void	projectionTransform(const float* matrix) = 0;
	virtual void	modelTransform(const float* matrix) = 0;

	virtual float*	getBaseProjection() = 0;
	virtual float*	getProjection() = 0;
	virtual float*	getModelview() = 0;

	virtual void	drawQuad(const float xmin, const float xmax, const float ymin, const float ymax) = 0;
	virtual void	drawBox(const float xradius, const float yradius, const float zradius) = 0;

	virtual int			createShader(Shader* shader) = 0;
	virtual bool		destroyShader(const int id) = 0;
	virtual ShaderPass	firstShaderPass(const int id, bool set=true) = 0;
	virtual ShaderPass	nextShaderPass(ShaderPass pass, bool set=true) = 0;
	virtual ShaderPass	nextShaderPass(const int id, ShaderPass pass, bool set=true) = 0;
	virtual bool		setShaderPass(ShaderPass pass) = 0;
	virtual void		resetShaderPass(ShaderPass pass) = 0;

	virtual bool		setShaderParameter(const int id, const AnyAttribute *attrib, const int index=-1) = 0;
	virtual bool		setShaderParameterBySemantic(const int id, const AnyAttribute *attrib, const int index=-1) = 0;
	virtual bool		setShaderMatrix(const int id, MatrixMode mode, const string& name) = 0;
	virtual bool		setShaderSampler(const int shader_id, const string& name, const int tex_id) = 0;
	virtual bool		setShaderSamplerBySemantic(const int shader_id, const string& name, const int tex_id) = 0;

	virtual bool		exportShaderParameters(const int id, Node* node) = 0;

	virtual int			createTexture(Image* image, TextureFilter defaultFilter=Bilinear) = 0;
	virtual bool		destroyTexture(const int id) = 0;
	virtual bool		bindTexture(const int id, const int unit) = 0;
	virtual bool		generateTextureMipmaps(const int id) = 0;
	virtual bool		getTextureProperties(const int id, int* width=NULL, int* height=NULL, int* bpp=NULL) = 0;

	virtual int			createFramebuffer(FramebufferFormat format, const int w, const int h, const int samples) = 0;
	virtual int			createFramebuffer(FramebufferFormat foramt, const int samples) = 0;
	virtual int			createFeedbackBuffer(FramebufferFormat format, const int w, const int h, TextureFilter filter) = 0;
	virtual int			createFeedbackBuffer(FramebufferFormat format, TextureFilter filter) = 0;
	virtual bool		destroyFramebuffer(const int id) = 0;
	virtual bool		destroyFeedbackBuffer(const int id) = 0;
	virtual bool		generateBufferMipmaps(const int id) = 0;
	virtual bool		drawFramebuffer(const int id, RenderbufferType type) = 0;
	virtual bool		drawFeedbackBuffer(const int id) = 0;

	virtual int			addBufferTarget(const int id, RenderbufferType type, TextureFilter filter) = 0;
	virtual int			getBufferTarget(const int id, RenderbufferType type) = 0;
	virtual int			getFeedbackBufferTarget(const int id, const int which=0) = 0;
	virtual bool		getFramebufferProperties(const int id, int* width, int* height, FramebufferFormat* format) = 0;
	virtual bool		getFeedbackBufferProperties(const int id, int* width, int* height, FramebufferFormat* format) = 0;

	virtual bool		bindFramebuffer(const int id) = 0;
	virtual void		commitFramebuffer(const int id) = 0;
	virtual int			bindFeedbackBuffer(const int id) = 0;
	virtual int			swapFeedbackBuffer(const int id) = 0;

	virtual int			createVertexBuffer() = 0;
	virtual bool		drawVertexBuffer(const int id, const size_t start, const size_t end) = 0;
	virtual bool		allocVertexBuffer(const int id, VertexBufferType type, const size_t size) = 0;
	virtual void*		lockVertexBuffer(const int id, VertexBufferType type) = 0;
	virtual bool		unlockVertexBuffer(const int id) = 0;
	virtual bool		destroyVertexBuffer(const int id) = 0;

	virtual int			createPixelBuffer(FramebufferFormat format, const int w, const int h, TextureFilter defaultFilter=NoFilter) = 0;
	virtual int			getPixelBufferTarget(const int id) = 0;
	virtual void*		lockPixelBuffer(const int id) = 0;
	virtual bool		unlockPixelBuffer(const int id, bool commit=true) = 0;
	virtual bool		commitPixelBuffer(const int id) = 0;
	virtual bool		destroyPixelBuffer(const int id) = 0;
	virtual bool		drawPixelBuffer(const int id) = 0;
};

} // Vorticity

#endif