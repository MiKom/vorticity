#include "config.h"
#include "opengl.h"
#include "sg/attribute.h"
#include "sg/node.h"
#include "resources/resource.h"
#include "resources/shader.h"
#include "resources/image.h"
#include "overlay/overlay.h"
#include "overlay/gloverlay.h"

using namespace Vorticity;

OpenGL::OpenGL() : Renderer()
{
	getCore()->setDevice(this);
	glEnable(GL_TEXTURE_2D);

	glewInit();
	cgContext = cgCreateContext();
	cgGLRegisterStates(cgContext);
	cgGLSetManageTextureParameters(cgContext, CG_TRUE);

	memset(projectionMatrix, 0, 64);
	projectionMatrix[0] = 1.0f;
	projectionMatrix[5] = 1.0f;
	projectionMatrix[10] = 1.0f;
	projectionMatrix[15] = 1.0f;
}

OpenGL::~OpenGL()
{
	size_t i;
	for(i=0; i<textures.size(); i++)
		destroyTexture(SETID(i));
	for(i=0; i<shaders.size(); i++)
		destroyShader(SETID(i));
	for(i=0; i<vertexBuffers.size(); i++)
		destroyVertexBuffer(SETID(i));
	for(i=0; i<frameBuffers.size(); i++)
		destroyFramebuffer(SETID(i));
	for(i=0; i<feedbackBuffers.size(); i++)
		destroyFeedbackBuffer(SETID(i));

	delete overlay;
	cgDestroyContext(cgContext);
}

const char* OpenGL::getVendorString() const
{
	return (const char*)glGetString(GL_VENDOR);
}

const char* OpenGL::getRendererString() const
{
	return (const char*)glGetString(GL_RENDERER);
}

const char* OpenGL::getCompilationLog() const
{
	return cgGetLastListing(cgContext);
}

void OpenGL::clear(Vorticity::ClearMode mode)
{
	if(mode == Vorticity::ClearColor)
		glClear(GL_COLOR_BUFFER_BIT);
	else if(mode == Vorticity::ClearDepth)
		glClear(GL_DEPTH_BUFFER_BIT);
	else if(mode == Vorticity::ClearStencil)
		glClear(GL_STENCIL_BUFFER_BIT);
	else if(mode == Vorticity::ClearColorDepth)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGL::clearColor(const float r, const float g, const float b, const float a)
{ glClearColor(r, g, b, a); }

void OpenGL::viewport(const int xoffset, const int yoffset, const int w, const int h)
{ glViewport(xoffset, yoffset, w, h); }

void OpenGL::projectionTransform(const float *matrix)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectionMatrix);
	if(matrix)
		glMultMatrixf(matrix);
}

void OpenGL::modelTransform(const float *matrix)
{
	glMatrixMode(GL_MODELVIEW);
	if(matrix)
		glLoadMatrixf(matrix);
	else
		glLoadIdentity();
}

void OpenGL::setPerspectiveProjection(const float fov, const float aspect, const float zNear, const float zFar)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
}

void OpenGL::setIdentityProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
}

void OpenGL::setProjection(const float* matrix)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matrix);
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
}

float* OpenGL::getProjection()
{
	// NOT thread safe!
	static float _projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, _projection);
	return _projection;
}

float* OpenGL::getModelview()
{
	// NOT thread safe!
	static float _modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, _modelview);
	return _modelview;
}

void OpenGL::setOrthoProjection(const float xmin, const float xmax, const float ymin, const float ymax)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xmin, xmax, ymax, ymin, -1.0f, 1.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
}

void OpenGL::drawQuad(const float xmin, const float xmax, const float ymin, const float ymax)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(xmin, ymin);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(xmax, ymin);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(xmax, ymax);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(xmin, ymax);
	glEnd();
}

void OpenGL::drawBox(const float xrad, const float yrad, const float zrad)
{
	static const GLfloat normals[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};
	static const GLfloat texcoords[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	};
	GLfloat vertices[] = {
		-xrad, yrad, -zrad, -xrad, yrad, zrad, xrad, yrad, zrad, xrad, yrad, -zrad,
		-xrad, -yrad, -zrad, xrad, -yrad, -zrad, xrad, -yrad, zrad, -xrad, -yrad, zrad,
		-xrad, yrad, -zrad, -xrad, -yrad, -zrad, -xrad, -yrad, zrad, -xrad, yrad, zrad,
		xrad, yrad, -zrad, xrad, yrad, zrad, xrad, -yrad, zrad, xrad, -yrad, -zrad,
		-xrad, yrad, -zrad, xrad, yrad, -zrad, xrad, -yrad, -zrad, -xrad, -yrad, -zrad,
		-xrad, yrad, zrad, -xrad, -yrad, zrad, xrad, -yrad, zrad, xrad, yrad, zrad,
	};

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

int OpenGL::createShader(Shader *shader)
{
	OpenGLShader glShader;
	glShader.effect = cgCreateEffect(cgContext, shader->getText(), NULL);
	if(!glShader.effect) return 0;

	glShader.technique = cgGetFirstTechnique(glShader.effect);
	while(glShader.technique)
	{
		if(cgValidateTechnique(glShader.technique) == CG_TRUE)
			break;
		glShader.technique = cgGetNextTechnique(glShader.technique);
	}
	if(!glShader.technique)
		return 0;

	int id = SETID(shaders.size());
	shaders.push_back(glShader);

	shader->setID(id);
	shader->setDevice(this);
	return id;
}

bool OpenGL::destroyShader(const int id)
{
	int index;
	if(!CHECKID(id, shaders.size()))
		return false;
	index = GETID(id);
	if(shaders.at(index).effect == 0)
		return false;

	cgDestroyEffect(shaders.at(index).effect);
	shaders.at(index).effect = 0;
	return true;
}

ShaderPass OpenGL::firstShaderPass(const int id, bool set)
{
	CGpass pass;
	if(!CHECKID(id, shaders.size()))
		return NULL;
	pass = cgGetFirstPass(shaders.at(GETID(id)).technique);
	if(set) cgSetPassState(pass);
	return (ShaderPass)pass;
}

ShaderPass OpenGL::nextShaderPass(ShaderPass pass, bool set)
{
	CGpass p = (CGpass)pass;
	CGpass nextp;

	cgResetPassState(p);
	nextp = cgGetNextPass(p);
	if(!nextp) return NULL;
	if(set) cgSetPassState(nextp);
	return (ShaderPass)nextp;
}

ShaderPass OpenGL::nextShaderPass(const int id, ShaderPass pass, bool set)
{
	if(!pass) return firstShaderPass(id, set);
	else return nextShaderPass(pass, set);
}

bool OpenGL::setShaderPass(ShaderPass pass)
{
	if(!pass) return false;
	cgSetPassState((CGpass)pass);
	return true;
}

void OpenGL::resetShaderPass(ShaderPass pass)
{
	cgResetPassState((CGpass)pass);
}

bool OpenGL::setShaderArrayParameter(CGparameter p, const ArrayAttribute *attrib)
{
	if(cgGetArrayDimension(p) > 1)
		return false;
	int size = cgGetArraySize(p, 0);
	//if(size > attrib->getCount())
	//	return false;
	for(int i=0; i<min(size, (int)attrib->getCount()); i++)
	{
		CGparameter pi = cgGetArrayParameter(p, i);
		if(!setSingleShaderParameter(pi, &attrib->getAt(i)))
			return false;
	}
	return true;
}

bool OpenGL::setShaderParameter(CGparameter p, const AnyAttribute *attrib, const int index)
{
	if(index > -1)
	{
		if(cgGetParameterType(p) != CG_ARRAY)
			return false;
		if(cgGetArrayDimension(p) > 1)
			return false;
		if(index >= cgGetArraySize(p, 0))
			return false;

		CGparameter pi = cgGetArrayParameter(p, index);
		return setSingleShaderParameter(pi, attrib);
	}
	if(attrib->getType() == "array")
		return setShaderArrayParameter(p, (ArrayAttribute*)attrib);
	else
		return setSingleShaderParameter(p, attrib);
}

bool OpenGL::setSingleShaderParameter(CGparameter p, const AnyAttribute *attrib)
{
	if(attrib->getType() == "int")
		cgSetParameter1i(p, (int)(*attrib));
	else if(attrib->getType() == "float")
		cgSetParameter1f(p, (float)(*attrib));
	else if(attrib->getType() == "float2")
	{
		vector2 v;
		attrib->getValue(v);
		cgSetParameter2f(p, v.x, v.y);
	}
	else if(attrib->getType() == "float3")
	{
		vector3 v;
		attrib->getValue(v);
		cgSetParameter3f(p, v.x, v.y, v.z);
	}
	else if(attrib->getType() == "float4")
	{
		vector4 v;
		attrib->getValue(v);
		cgSetParameter4f(p, v.x, v.y, v.z, v.w);
	}
	else if(attrib->getType() == "matrix4")
	{
		matrix4 v;
		attrib->getValue(v);
		cgSetMatrixParameterfc(p, v.cell);
	}
	else return false;
	return true;
}


bool OpenGL::setShaderParameter(const int id, const AnyAttribute *attrib, const int index)
{
	if(!CHECKID(id, shaders.size()))
		return false;
	CGparameter p = cgGetNamedEffectParameter(shaders.at(GETID(id)).effect,
		attrib->getName().c_str());
	if(!p) return false;
	return setShaderParameter(p, attrib, index);
}

bool OpenGL::setShaderParameterBySemantic(const int id, const AnyAttribute *attrib, const int index)
{
	if(!CHECKID(id, shaders.size()))
		return false;
	CGparameter p = cgGetEffectParameterBySemantic(shaders.at(GETID(id)).effect,
		attrib->getName().c_str());
	if(!p) return false;
	return setShaderParameter(p, attrib, index);
}

AnyAttribute* OpenGL::exportSingleShaderParameter(CGparameter p)
{
	ArrayAttribute *arrayAttrib;
	switch(cgGetParameterType(p))
	{
	case CG_ARRAY:
		if(cgGetArrayDimension(p) > 1)
			break;
		arrayAttrib = new ArrayAttribute(cgGetParameterName(p));
		for(int i=0; i<cgGetArraySize(p, 0); i++)
		{
			CGparameter pi = cgGetArrayParameter(p, i);
			AnyAttribute *attrib = exportSingleShaderParameter(pi);
			if(!attrib)
			{
				delete arrayAttrib;
				return NULL;
			}
			arrayAttrib->push(attrib);
		}
		return arrayAttrib;
	case CG_INT:
		return new Attribute<int>(cgGetParameterName(p));
	case CG_FLOAT:
		return new Attribute<float>(cgGetParameterName(p));
	case CG_FLOAT2:
		return new Attribute<vector2>(cgGetParameterName(p));
	case CG_FLOAT3:
		return new Attribute<vector3>(cgGetParameterName(p));
	case CG_FLOAT4:
		return new Attribute<vector4>(cgGetParameterName(p));
	case CG_FLOAT4x4:
		return new Attribute<matrix4>(cgGetParameterName(p));
	}
	return NULL;
}

bool OpenGL::exportShaderParameters(const int id, Node *node)
{
	if(!CHECKID(id, shaders.size()))
		return false;

	CGparameter p = cgGetFirstEffectParameter(shaders.at(GETID(id)).effect);
	if(!p) return true;
	
	do
	{
		AnyAttribute *attrib = exportSingleShaderParameter(p);
		if(attrib) node->addAttrib(attrib);
	} while(p = cgGetNextParameter(p));
	return true;
}

bool OpenGL::setShaderSampler(const int shader_id, const string& name, const int tex_id)
{
	if(!CHECKID(shader_id, shaders.size()))
		return false;
	if(!CHECKID(tex_id, textures.size()))
		return false;

	CGparameter p = cgGetNamedEffectParameter(shaders.at(GETID(shader_id)).effect, name.c_str());
	if(!p) return false;

	cgGLSetupSampler(p, textures.at(GETID(tex_id)).index);
	return true;
}

bool OpenGL::setShaderSamplerBySemantic(const int shader_id, const string& name, const int tex_id)
{
	if(!CHECKID(shader_id, shaders.size()))
		return false;
	if(!CHECKID(tex_id, textures.size()))
		return false;

	CGparameter p = cgGetEffectParameterBySemantic(shaders.at(GETID(shader_id)).effect, name.c_str());
	if(!p) return false;

	cgGLSetupSampler(p, textures.at(GETID(tex_id)).index);
	return true;
}

bool OpenGL::setShaderMatrix(const int id, MatrixMode mode, const string& name)
{
	if(!CHECKID(id, shaders.size()))
		return false;

	CGparameter p = cgGetEffectParameterBySemantic(
		shaders.at(GETID(id)).effect, name.c_str());
	if(!p) return false;

	switch(mode)
	{
	case ModelviewMatrix:
		cgGLSetStateMatrixParameter(p, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
		break;
	case ProjectionMatrix:
		cgGLSetStateMatrixParameter(p, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		break;
	case ModelviewProjMatrix:
		cgGLSetStateMatrixParameter(p, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		break;
	case NormalMatrix:
		cgGLSetStateMatrixParameter(p, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);
		break;
	}
	return true;
}

int OpenGL::createTexture(Image *image, TextureFilter defaultFilter)
{
	if(!image) return 0;
	GLuint gltex_id;
	GLenum format = GL_RGB;

	if(image->getBpp() == 4)
		format = GL_RGBA;

	glGenTextures(1, &gltex_id);
	glBindTexture(GL_TEXTURE_2D, gltex_id);
	
	if(defaultFilter == Vorticity::Trilinear)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, image->getBpp(), image->getWidth(), image->getHeight(),
			format, GL_UNSIGNED_BYTE, image->getPixels());
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, image->getBpp(), image->getWidth(), image->getHeight(),
			0, format, GL_UNSIGNED_BYTE, image->getPixels());
	}

	switch(defaultFilter)
	{
	case NoFilter:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case Bilinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case Trilinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	int id = SETID(textures.size());
	OpenGLTexture gltex;

	gltex.index		= gltex_id;
	gltex.width		= image->getWidth();
	gltex.height	= image->getHeight();
	gltex.bpp		= image->getBpp();
	
	textures.push_back(gltex);
	image->setID(id);
	image->setDevice(this);
	return id;
}

bool OpenGL::destroyTexture(const int id)
{
	if(!CHECKID(id, textures.size()))
		return false;
	int index = GETID(id);
	if(textures.at(index).index == 0)
		return false;
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textures.at(index).index);
	textures.at(index).index = 0;
	return true;
}

bool OpenGL::bindTexture(const int id, const int unit)
{
	if(id == 0)
	{
		glActiveTexture(GL_TEXTURE0+unit);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}

	if(!CHECKID(id, textures.size()))
		return false;
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, textures.at(GETID(id)).index);
	return true;
}

GLuint OpenGL::createGLRenderbuffer(const int w, const int h, const int samples,
									GLenum format, GLenum attachment)
{
	GLuint rb;
	glGenRenderbuffersEXT(1, &rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
	if(samples > 1)
		glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, format, w, h);
	else
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, format, w, h);

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	return rb;
}

GLuint OpenGL::createGLTexture(const int w, const int h, GLenum format, GLenum type, TextureFilter filter)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	switch(filter)
	{
	case NoFilter:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case Bilinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case Trilinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}

	GLenum tex_format = GL_RGBA;
	if(format >= GL_DEPTH_COMPONENT16 && format <= GL_DEPTH_COMPONENT32)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		tex_format = GL_DEPTH_COMPONENT;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, tex_format, type, NULL);
	if(filter == Trilinear)
		glGenerateMipmapEXT(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

int OpenGL::createFramebuffer(FramebufferFormat format, const int samples)
{ return createFramebuffer(format, getScreenWidth(), getScreenHeight(), samples); }

int OpenGL::createFramebuffer(FramebufferFormat format, const int w, const int h, const int samples)
{
	OpenGLFrameBuffer fb;
	size_t id;

	memset(&fb, 0, sizeof(struct OpenGLFrameBuffer));
	fb.samples = samples;
	fb.w = w;
	fb.h = h;

	glGenFramebuffersEXT(1, &fb.index[OpenGLFrameBuffer::GL_FB_Main]);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb.index[OpenGLFrameBuffer::GL_FB_Main]);

	GLenum colorFormat;	
	switch(format)
	{
	case BufferRGBA:
		colorFormat = GL_RGBA8; break;
	case BufferFloat16:
		colorFormat = GL_RGBA16F_ARB; break;
	case BufferFloat32:
		colorFormat = GL_RGBA32F_ARB; break;
	}

	fb.format[OpenGLFrameBuffer::GL_RB_Color]   = colorFormat;
	fb.format[OpenGLFrameBuffer::GL_RB_Depth]   = GL_DEPTH_COMPONENT24;
	fb.format[OpenGLFrameBuffer::GL_RB_Stencil] = GL_STENCIL_INDEX;

	fb.attachment[OpenGLFrameBuffer::GL_RB_Color]   = GL_COLOR_ATTACHMENT0_EXT;
	fb.attachment[OpenGLFrameBuffer::GL_RB_Depth]   = GL_DEPTH_ATTACHMENT_EXT;
	fb.attachment[OpenGLFrameBuffer::GL_RB_Stencil] = GL_STENCIL_ATTACHMENT_EXT;

	// Color renderbuffer
	fb.rbuffers[OpenGLFrameBuffer::GL_RB_Color] = 
		createGLRenderbuffer(w, h, samples, colorFormat, GL_COLOR_ATTACHMENT0_EXT);
	
	// Depth renderbuffer
	fb.rbuffers[OpenGLFrameBuffer::GL_RB_Depth] =
		createGLRenderbuffer(w, h, samples, GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT_EXT);

	// TODO: Implement stencil renderbuffer attachment!
	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		return 0;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	id = frameBuffers.size();
	frameBuffers.push_back(fb);
	return SETID(id);
}

bool OpenGL::destroyFramebuffer(const int id)
{
	if(!CHECKID(id, frameBuffers.size()))
		return false;

	OpenGLFrameBuffer* fb = &frameBuffers.at(GETID(id));
	if(fb->index[0] == 0)
		return false;

	for(int i=0; i<3; i++)
	{
		if(fb->rbuffers[i])
			glDeleteRenderbuffersEXT(1, &fb->rbuffers[i]);
	}
	if(fb->index[OpenGLFrameBuffer::GL_FB_Main])
		glDeleteFramebuffersEXT(1, &fb->index[OpenGLFrameBuffer::GL_FB_Main]);
	if(fb->index[OpenGLFrameBuffer::GL_FB_Targets])
	{
		destroyTexture(fb->targets[0]);
		destroyTexture(fb->targets[1]);
		destroyTexture(fb->targets[2]);
		glDeleteFramebuffersEXT(1, &fb->index[OpenGLFrameBuffer::GL_FB_Targets]);
	}
	fb->index[0] = 0;
	return true;
}

int OpenGL::createFeedbackBuffer(FramebufferFormat format, TextureFilter filter)
{ return createFeedbackBuffer(format, getScreenWidth(), getScreenHeight(), filter); }

int OpenGL::createFeedbackBuffer(FramebufferFormat format, const int w, const int h, Vorticity::TextureFilter filter)
{
	OpenGLFeedbackBuffer fb;
	OpenGLTexture tex[2];
	int id;

	if(filter == Trilinear)
		return 0;

	fb.w = w; fb.h = h;
	fb.bound = 1;
	tex[0].width = tex[1].width = w;
	tex[0].height = tex[1].height = h;

	GLenum data_type	= GL_FLOAT;
	GLenum data_format	= GL_RGBA8;

	switch(format)
	{
	case BufferRGBA:
		data_type = GL_UNSIGNED_BYTE; break;
	case BufferFloat16:
		data_format = GL_RGBA16F_ARB; break;
	case BufferFloat32:
		data_format = GL_RGBA32F_ARB; break;
	}	
	
	for(int i=0; i<2; i++)
	{
		glGenFramebuffersEXT(1, &fb.index[i]);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb.index[i]);

		tex[i].index = createGLTexture(w, h, data_format, data_type, filter);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 
			tex[i].index, 0);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glDeleteFramebuffersEXT(1, &fb.index[i]);
			if(i==1)
				glDeleteFramebuffersEXT(1, &fb.index[0]);
		}
		fb.targets[i] = SETID(textures.size());
		textures.push_back(tex[i]);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	id = SETID(feedbackBuffers.size());
	feedbackBuffers.push_back(fb);
	return id;
}

bool OpenGL::destroyFeedbackBuffer(const int id)
{
	if(!CHECKID(id, feedbackBuffers.size()))
		return false;

	OpenGLFeedbackBuffer *fb = &feedbackBuffers.at(GETID(id));
	if(fb->index[0] == 0)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glDeleteFramebuffers(1, &fb->index[0]);
	glDeleteFramebuffers(1, &fb->index[1]);
	destroyTexture(fb->targets[0]);
	destroyTexture(fb->targets[1]);
	fb->index[0] = 0;
	return true;
}

int OpenGL::addBufferTarget(const int id, RenderbufferType type, Vorticity::TextureFilter filter)
{
	if(!CHECKID(id, frameBuffers.size()))
		return 0;

	OpenGLFrameBuffer* fb = &frameBuffers.at(GETID(id));
	if(fb->targets[type])
		return 0;

	if(!fb->index[OpenGLFrameBuffer::GL_FB_Targets])
		glGenFramebuffersEXT(1, &fb->index[OpenGLFrameBuffer::GL_FB_Targets]);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb->index[OpenGLFrameBuffer::GL_FB_Targets]);

	GLenum data_type = GL_UNSIGNED_BYTE;
	if(fb->format[type] == GL_RGBA16F_ARB || fb->format[type] == GL_RGBA32F_ARB)
		data_type = GL_FLOAT;

	OpenGLTexture textureObject;
	int textureID;

	textureObject.index = createGLTexture(fb->w, fb->h, fb->format[type], data_type, filter);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, fb->attachment[type], GL_TEXTURE_2D, 
		textureObject.index, 0);

	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		return 0;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	textureObject.width		= fb->w;
	textureObject.height	= fb->h;
	textureObject.bpp		= 4;

	textureID = textures.size();
	textures.push_back(textureObject);
	fb->targets[type] = SETID(textureID);
	fb->mipmap[type]  = filter == Trilinear;
	return SETID(textureID);
}

int OpenGL::getBufferTarget(const int id, RenderbufferType type)
{
	if(!CHECKID(id, frameBuffers.size()))
		return 0;
	return (&frameBuffers.at(GETID(id)))->targets[type];
}

int OpenGL::getFeedbackBufferTarget(const int id, const int witch)
{
	if(!CHECKID(id, feedbackBuffers.size()))
		return 0;
	OpenGLFeedbackBuffer* fb = &feedbackBuffers.at(GETID(id));
	switch(id)
	{
	case 0: return fb->targets[fb->bound];
	case 1: return fb->targets[0];
	case 2: return fb->targets[1];
	default: return 0;
	}
}

bool OpenGL::bindFramebuffer(const int id)
{
	if(!CHECKID(id, frameBuffers.size()))
		return false;
	OpenGLFrameBuffer *fb = &frameBuffers.at(GETID(id));
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb->index[OpenGLFrameBuffer::GL_FB_Main]);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, fb->w, fb->h);
	return true;
}

void OpenGL::commitFramebuffer(const int id)
{
	if(!CHECKID(id, frameBuffers.size()))
		return;
	OpenGLFrameBuffer *fb = &frameBuffers.at(GETID(id));
	if(!fb->index[OpenGLFrameBuffer::GL_FB_Targets])
		return;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, fb->index[OpenGLFrameBuffer::GL_FB_Main]);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fb->index[OpenGLFrameBuffer::GL_FB_Targets]);

	glBlitFramebufferEXT(0, 0, fb->w, fb->h, 0, 0, fb->w, fb->h,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}

bool OpenGL::drawFramebuffer(const int id, RenderbufferType type)
{
	if(!CHECKID(id, frameBuffers.size()))
		return false;

	OpenGLFrameBuffer *fb = &frameBuffers.at(GETID(id));
	if(!fb->index[OpenGLFrameBuffer::GL_FB_Targets])
		return false;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	int colorTarget;
	if(type == Vorticity::ColorBuffer)
		colorTarget = fb->targets[OpenGLFrameBuffer::GL_RB_Color];
	else
		colorTarget = fb->targets[OpenGLFrameBuffer::GL_RB_Depth];

	glPushAttrib(GL_VIEWPORT_BIT);
	bindTexture(colorTarget, 0);
	drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
	bindTexture(0, 0);
	glPopAttrib();
	return true;
}

bool OpenGL::drawFeedbackBuffer(const int id)
{
	if(!CHECKID(id, feedbackBuffers.size()))
		return false;

	OpenGLFeedbackBuffer *fb = &feedbackBuffers.at(GETID(id));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	int colorTarget = fb->targets[fb->bound];
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, fb->w, fb->h);
	bindTexture(colorTarget, 0);
	drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
	bindTexture(0, 0);
	glPopAttrib();
	return true;
}

int OpenGL::bindFeedbackBuffer(const int id)
{
	if(id == 0)
	{
		glPopAttrib();
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		return 0;
	}
	if(!CHECKID(id, feedbackBuffers.size()))
		return 0;
	OpenGLFeedbackBuffer *fb = &feedbackBuffers.at(GETID(id));

	int old_bound = fb->bound;
	fb->bound = (fb->bound+1)%2;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb->index[fb->bound]);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, fb->w, fb->h);
	return fb->targets[old_bound];
}

int OpenGL::swapFeedbackBuffer(const int id)
{
	if(!CHECKID(id, feedbackBuffers.size()))
		return 0;
	OpenGLFeedbackBuffer *fb = &feedbackBuffers.at(GETID(id));
	int old_bound = fb->bound;
	fb->bound = (fb->bound+1)%2;
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb->index[fb->bound]);
	return fb->targets[old_bound];
}

bool OpenGL::getTextureProperties(const int id, int* width, int* height, int* bpp)
{
	if(!CHECKID(id, textures.size()))
		return false;
	OpenGLTexture* tex = &textures.at(GETID(id));
	if(width) *width = tex->width;
	if(height) *height = tex->height;
	if(bpp) *bpp = tex->bpp;
	return true;
}

bool OpenGL::getFramebufferProperties(const int id, int* width, int* height, FramebufferFormat* format)
{
	if(!CHECKID(id, frameBuffers.size()))
		return false;
	OpenGLFrameBuffer* fb = &frameBuffers.at(GETID(id));
	if(width)  *width = fb->w;
	if(height) *height = fb->h;
	// Fixme: Implement format info.
	return true;
}

bool OpenGL::getFeedbackBufferProperties(const int id, int* width, int* height, FramebufferFormat* format)
{
	if(!CHECKID(id, feedbackBuffers.size()))
		return false;
	OpenGLFeedbackBuffer* fb = &feedbackBuffers.at(GETID(id));
	if(width)  *width = fb->w;
	if(height) *height = fb->h;
	// Fixme: Implement format info.
	return true;
}

bool OpenGL::generateTextureMipmaps(const int id)
{
	if(!CHECKID(id, textures.size()))
		return false;

	glBindTexture(GL_TEXTURE_2D, textures.at(GETID(id)).index);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

bool OpenGL::generateBufferMipmaps(const int id)
{
	if(!CHECKID(id, frameBuffers.size()))
		return false;
	OpenGLFrameBuffer *fb = &frameBuffers.at(GETID(id));
	
	for(int i=0; i<3; i++)
	{
		if(fb->mipmap[i])
			generateTextureMipmaps(fb->targets[i]);
	}
	return true;
}

int OpenGL::createVertexBuffer()
{
	OpenGLVertexBuffer vb;
	glGenBuffers(1, &vb.vertex);
	glGenBuffers(1, &vb.normal);
	glGenBuffers(1, &vb.texcoord);

	int id = SETID(vertexBuffers.size());
	vertexBuffers.push_back(vb);
	return id;
}

GLenum OpenGL::bindGLVertexBuffer(const OpenGLVertexBuffer *vb, VertexBufferType type)
{
	GLuint glbuffer;
	switch(type)
	{
	case VertexBuffer: glbuffer = vb->vertex; break;
	case NormalBuffer: glbuffer = vb->normal; break;
	case TexCoordBuffer: glbuffer = vb->texcoord; break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, glbuffer);
	return GL_ARRAY_BUFFER;
}

bool OpenGL::allocVertexBuffer(const int id, VertexBufferType type, const size_t size)
{
	if(!CHECKID(id, vertexBuffers.size()))
		return false;

	size_t dataSize = 0;
	if(type == VertexBuffer || type == NormalBuffer)
		dataSize = size * 3 * sizeof(float);
	else if(type == TexCoordBuffer)
		dataSize = size * 2 * sizeof(float);

	GLenum target = bindGLVertexBuffer(&vertexBuffers.at(GETID(id)), type);
	glBufferData(target, dataSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(target, 0);
	return true;
}

void* OpenGL::lockVertexBuffer(const int id, VertexBufferType type)
{
	if(!CHECKID(id, vertexBuffers.size()))
		return NULL;
	GLenum target = bindGLVertexBuffer(&vertexBuffers.at(GETID(id)), type);
	return glMapBuffer(target, GL_WRITE_ONLY);
}

bool OpenGL::unlockVertexBuffer(const int id)
{
	if(!CHECKID(id, vertexBuffers.size()))
		return false;
	GLboolean result;
	result = glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return result == GL_TRUE;
}

bool OpenGL::destroyVertexBuffer(const int id)
{
	if(!CHECKID(id, vertexBuffers.size()))
		return false;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	OpenGLVertexBuffer* vb = &vertexBuffers.at(GETID(id));
	if(vb->vertex == 0)
		return false;
	glDeleteBuffers(1, &vb->vertex);
	glDeleteBuffers(1, &vb->normal);
	glDeleteBuffers(1, &vb->texcoord);
	vb->vertex = 0;
	return true;
}

bool OpenGL::drawVertexBuffer(const int id, const size_t start, const size_t end)
{
	if(!CHECKID(id, vertexBuffers.size()))
		return false;

	OpenGLVertexBuffer *vb = &vertexBuffers.at(GETID(id));
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vb->normal);
	glNormalPointer(GL_FLOAT, 0, NULL);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vb->texcoord);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	// Deffered to speed-up rendering.
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vb->vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_TRIANGLES, start, end);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

int OpenGL::createPixelBuffer(FramebufferFormat format, const int w, const int h, TextureFilter defaultFilter)
{
	OpenGLPixelBuffer pb;
	OpenGLTexture target;

	size_t pixel_bytes;
	switch(format)
	{
	case BufferRGBA:
		pb.format 	= GL_RGBA;
		pb.datatype = GL_UNSIGNED_BYTE;
		pixel_bytes = 4;
		break;
	case BufferFloat16:
		pb.format   = GL_RGBA16F_ARB;
		pb.datatype	= GL_FLOAT;
		pixel_bytes = 8;
		break;
	case BufferFloat32:
		pb.format   = GL_RGBA32F_ARB;
		pb.datatype	= GL_FLOAT;
		pixel_bytes = 16;
		break;
	}

	pb.w 			= w; pb.h = h;
	pb.type 		= WriteBuffer;
	target.width 	= w; target.height = h;
	pb.datasize  	= w*h*pixel_bytes;

	glGenBuffers(1, &pb.index);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pb.index);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, pb.datasize, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	OpenGLTexture textureObject;
	textureObject.width  = w;
	textureObject.height = h;
	textureObject.bpp    = 4;
	textureObject.index  = createGLTexture(w, h, pb.format, pb.datatype, defaultFilter);

	int textureID, bufferID;
	textureID 	  =	textures.size();
	bufferID	  = pixelBuffers.size();
	pb.target     = SETID(textureID);
	textures.push_back(textureObject);
	pixelBuffers.push_back(pb);
	return SETID(bufferID);
}

int OpenGL::getPixelBufferTarget(const int id)
{
	if(!CHECKID(id, pixelBuffers.size()))
		return 0;
	return pixelBuffers.at(GETID(id)).target;
}

void* OpenGL::lockPixelBuffer(const int id)
{
	if(!CHECKID(id, pixelBuffers.size()))
		return NULL;

	OpenGLPixelBuffer *pb = &pixelBuffers.at(GETID(id));
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pb->index);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, pb->datasize, NULL, GL_STREAM_DRAW);
	return glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
}

bool OpenGL::unlockPixelBuffer(const int id, bool commit)
{
	if(!CHECKID(id, pixelBuffers.size()))
		return false;

	GLboolean result;
	result = glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	if(commit)
		commitPixelBuffer(id);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	return result == GL_TRUE;
}

bool OpenGL::commitPixelBuffer(const int id)
{
	if(!CHECKID(id, pixelBuffers.size()))
		return false;

	OpenGLPixelBuffer *pb = &pixelBuffers.at(GETID(id));
	bindTexture(pb->target, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pb->index);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pb->w, pb->h, pb->format, pb->datatype, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	bindTexture(0, 0);
	return true;
}

bool OpenGL::destroyPixelBuffer(const int id)
{
	if(!CHECKID(id, pixelBuffers.size()))
		return false;

	OpenGLPixelBuffer *pb = &pixelBuffers.at(GETID(id));
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pb->index);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, pb->datasize, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	destroyTexture(pb->target);
	glDeleteBuffers(1, &pb->index);
	return true;
}

bool OpenGL::drawPixelBuffer(const int id)
{
	if(!CHECKID(id, pixelBuffers.size()))
		return false;

	OpenGLPixelBuffer* pb = &pixelBuffers.at(GETID(id));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, pb->w, pb->h);
	bindTexture(pb->target, 0);
	drawQuad(0.0f, 1.0f, 0.0f, 1.0f);
	bindTexture(0, 0);
	glPopAttrib();
	return true;
}