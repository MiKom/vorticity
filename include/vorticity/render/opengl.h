#ifndef __VORTICITY_OPENGL_H
#define __VORTICITY_OPENGL_H

namespace Vorticity {

struct OpenGLShader
{
	CGeffect effect;
	CGtechnique technique;
};

struct OpenGLTexture
{
	GLuint	index;
	int		width;
	int		height;
	int		bpp;
};

struct OpenGLFrameBuffer
{
	enum {
		GL_FB_Main = 0,
		GL_FB_Targets = 1,
		GL_RB_Color = 0,
		GL_RB_Depth = 1,
		GL_RB_Stencil = 2,
	};
	GLuint	index[2];
	GLuint	rbuffers[3];
	GLenum	format[3];
	GLenum	attachment[3];
	bool	mipmap[3];
	int		targets[3];
	int		samples;
	int		w,h;
};

struct OpenGLFeedbackBuffer
{
	GLuint	index[2];
	int		targets[2];
	int		w,h;
	int		bound;
};

struct OpenGLVertexBuffer
{
	GLuint	vertex;
	GLuint	normal;
	GLuint	texcoord;
};

struct OpenGLPixelBuffer
{
	GLuint index;
	PixelBufferType type;
	int w,h;
	int target;
	size_t datasize;
	GLenum format;
	GLenum datatype;
};

class VAPI OpenGL : public Renderer
{
protected:
	float	projectionMatrix[16];
	std::vector<OpenGLShader> shaders;
	std::vector<OpenGLTexture> textures;
	std::vector<OpenGLFrameBuffer> frameBuffers;
	std::vector<OpenGLFeedbackBuffer> feedbackBuffers;
	std::vector<OpenGLVertexBuffer> vertexBuffers;
	std::vector<OpenGLPixelBuffer> pixelBuffers;

	static GLuint	createGLRenderbuffer(const int w, const int h, const int samples,
		GLenum format, GLenum attachment);
	static GLuint	createGLTexture(const int w, const int h, GLenum format, GLenum type,
		TextureFilter filter);
	static GLenum	bindGLVertexBuffer(const OpenGLVertexBuffer* vb, VertexBufferType type);

	AnyAttribute*	exportSingleShaderParameter(CGparameter p);
	bool			setSingleShaderParameter(CGparameter p, const AnyAttribute *attrib);
	bool			setShaderArrayParameter(CGparameter p, const ArrayAttribute *attrib);
	bool			setShaderParameter(CGparameter p, const AnyAttribute *attrib, const int index=-1);
public:
	OpenGL();
	virtual ~OpenGL();

	virtual const char* getVendorString() const;
	virtual const char*	getRendererString() const;
	virtual const char* getCompilationLog() const;

	virtual void clear(ClearMode mode);
	virtual void clearColor(const float r, const float g, const float b, const float a);
	virtual void viewport(const int xoffset, const int yoffset, const int w, const int h);
	virtual void setPerspectiveProjection(const float fov, const float aspect, const float zNear, const float zFar);
	virtual void setOrthoProjection(const float xmin, const float xmax, const float ymin, const float ymax);
	virtual void setIdentityProjection();
	virtual void setProjection(const float* matrix);

	virtual void projectionTransform(const float* matrix);
	virtual void modelTransform(const float* matrix);

	virtual float*	getBaseProjection() { return projectionMatrix; }

	virtual float*	getProjection();
	virtual float*	getModelview();	
	CGcontext		getCGContext() const;

	virtual void drawQuad(const float xmin, const float xmax, const float ymin, const float ymax);
	virtual void drawBox(const float xradius, const float yradius, const float zradius);

	virtual int			createShader(Shader* shader);
	virtual bool		destroyShader(const int id);
	virtual ShaderPass	firstShaderPass(const int id, bool set=true);
	virtual ShaderPass	nextShaderPass(ShaderPass pass, bool set=true);
	virtual ShaderPass	nextShaderPass(const int id, ShaderPass pass, bool set=true);
	virtual bool		setShaderPass(ShaderPass pass);
	virtual void		resetShaderPass(ShaderPass pass);

	virtual bool		setShaderParameter(const int id, const AnyAttribute *attrib, const int index=-1) ;
	virtual bool		setShaderParameterBySemantic(const int id, const AnyAttribute *attrib, const int index=-1);
	virtual bool		setShaderMatrix(const int id, MatrixMode mode, const string& name);
	virtual bool		setShaderSampler(const int shader_id, const string& name, const int tex_id);
	virtual bool		setShaderSamplerBySemantic(const int shader_id, const string& name, const int tex_id);

	virtual bool		exportShaderParameters(const int id, Node* node);

	virtual int			createTexture(Image* image, TextureFilter defaultFilter=Bilinear);
	virtual bool		destroyTexture(const int id);
	virtual bool		bindTexture(const int id, const int unit);
	virtual bool		generateTextureMipmaps(const int id);
	virtual bool		getTextureProperties(const int id, int* width=NULL, int* height=NULL, int* bpp=NULL);

	virtual int			createFramebuffer(FramebufferFormat format, const int w, const int h, const int samples);
	virtual int			createFramebuffer(FramebufferFormat format, const int samples);
	virtual int			createFeedbackBuffer(FramebufferFormat format, const int w, const int h, TextureFilter filter);
	virtual int			createFeedbackBuffer(FramebufferFormat format, TextureFilter filter);
	virtual bool		destroyFramebuffer(const int id);
	virtual bool		destroyFeedbackBuffer(const int id);
	virtual bool		generateBufferMipmaps(const int id);
	virtual bool		drawFramebuffer(const int id, RenderbufferType type);
	virtual bool		drawFeedbackBuffer(const int id);

	virtual int			addBufferTarget(const int id, RenderbufferType type, TextureFilter filter);
	virtual int			getBufferTarget(const int id, RenderbufferType type);
	virtual int			getFeedbackBufferTarget(const int id, const int which=0);
	virtual bool		getFramebufferProperties(const int id, int* width, int* height, FramebufferFormat* format);
	virtual bool		getFeedbackBufferProperties(const int id, int* width, int* height, FramebufferFormat* format);

	virtual bool		bindFramebuffer(const int id);
	virtual void		commitFramebuffer(const int id);
	virtual int			bindFeedbackBuffer(const int id);
	virtual int			swapFeedbackBuffer(const int id);

	virtual int			createVertexBuffer();
	virtual bool		drawVertexBuffer(const int id, const size_t start, const size_t end);
	virtual bool		allocVertexBuffer(const int id, VertexBufferType type, const size_t size);
	virtual void*		lockVertexBuffer(const int id, VertexBufferType type);
	virtual bool		unlockVertexBuffer(const int id);
	virtual bool		destroyVertexBuffer(const int id);

	virtual int			createPixelBuffer(FramebufferFormat format, const int w, const int h, TextureFilter defaultFilter=NoFilter);
	virtual int			getPixelBufferTarget(const int id);
	virtual void*		lockPixelBuffer(const int id);
	virtual bool		unlockPixelBuffer(const int id, bool commit=true);
	virtual bool		commitPixelBuffer(const int id);
	virtual bool		destroyPixelBuffer(const int id);
	virtual bool		drawPixelBuffer(const int id);
};

} // Vorticity

#endif