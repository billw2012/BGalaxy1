#ifndef _GLBASE_TEXTURE_HPP
#define _GLBASE_TEXTURE_HPP

// stl
#include <stdexcept>
#include <map>
#include <sstream>

// boost
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <GL/glew.h>

// misc
#include <IL/il.h>
#include <IL/ilut.h>

#ifdef WIN32
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")
#endif

namespace glbase
{

struct TextureType { enum type {
	BYTE,
	FLOAT
};};

struct WrapMode { enum type {
	None,
	Repeat,
	ClampToEdge,
	MirroredRepeat
};};

struct FilterMode { enum type {
	None,
	Linear,
	Nearest
};};

struct LoadOptions { enum type {
	None				= 0x00,
	GenerateMipmaps		= 0x01,
	AsRect				= 0x02
};};

inline void throw_on_IL_error()
{
	switch(ilGetError())
	{
	case IL_NO_ERROR:
		break;
	case ILU_OUT_OF_MEMORY:
		throw std::exception("ILU_OUT_OF_MEMORY");
	case IL_INVALID_ENUM         :
		throw std::exception("IL_INVALID_ENUM");
	case IL_FORMAT_NOT_SUPPORTED :
		throw std::exception("IL_FORMAT_NOT_SUPPORTED");
	case IL_INTERNAL_ERROR       :
		throw std::exception("IL_INTERNAL_ERROR");
	case IL_INVALID_VALUE        :
		throw std::exception("IL_INVALID_VALUE");
	case IL_ILLEGAL_OPERATION    :
		throw std::exception("IL_ILLEGAL_OPERATION");
	case IL_ILLEGAL_FILE_VALUE   :
		throw std::exception("IL_ILLEGAL_FILE_VALUE");
	case IL_INVALID_FILE_HEADER  :
		throw std::exception("IL_INVALID_FILE_HEADER");
	case IL_INVALID_PARAM        :
		throw std::exception("IL_INVALID_PARAM");
	case IL_COULD_NOT_OPEN_FILE  :
		throw std::exception("IL_COULD_NOT_OPEN_FILE");
	case IL_INVALID_EXTENSION    :
		throw std::exception("IL_INVALID_EXTENSION");
	case IL_FILE_ALREADY_EXISTS  :
		throw std::exception("IL_FILE_ALREADY_EXISTS");
	case IL_OUT_FORMAT_SAME      :
		throw std::exception("IL_OUT_FORMAT_SAME");
	case IL_STACK_OVERFLOW       :
		throw std::exception("IL_STACK_OVERFLOW");
	case IL_STACK_UNDERFLOW      :
		throw std::exception("IL_STACK_UNDERFLOW");
	case IL_INVALID_CONVERSION   :
		throw std::exception("IL_INVALID_CONVERSION");
	case IL_BAD_DIMENSIONS       :
		throw std::exception("IL_BAD_DIMENSIONS");
	case IL_FILE_READ_ERROR      : // IL_FILE_WRITE_ERROR has same value?!
		throw std::exception("IL_FILE_READ_ERROR/IL_FILE_WRITE_ERROR");
	default:
		throw new std::runtime_error("Unknown error occurred during file load!");
	};
}


struct UVType { enum type {
	ZeroToOne,
	Pixel
};};

struct UVQuad
{
	UVQuad(float u_ = 0.0f, float v_ = 0.0f, float u2_ = 1.0f, float v2_ = 1.0f, UVType::type type_ = UVType::ZeroToOne) 
		: u(u_), v(v_), u2(u2_), v2(v2_), type(type_) {}

	float width() const { return u2 - u; }
	float height() const { return v2 - v; }

	float u, v, u2, v2;
	UVType::type type;
};

//////////////////////////////////////////////////////////////////////////
// Texture class
// For holding texture data.
struct Texture
{
	typedef boost::shared_ptr<Texture> ptr;
	typedef unsigned int size_type;

	// fields
private:
	GLuint _handle;
	std::string _name;
	std::string _fileName;
	bool _valid;

	GLuint _width;
	GLuint _height;
	GLuint _depth;
	GLubyte _components;
	GLenum _format;
	GLenum _internalformat;
	GLenum _type;
	GLenum _target;

	bool _mipmapped;

	// methods
public:
	Texture();

	Texture(const std::string& fileName, LoadOptions::type loadOptions = LoadOptions::None);

	~Texture() { destroy(); }

	void create1D(GLenum target, GLuint width, GLint components,
		GLenum format, GLenum type, const GLvoid *data, GLenum internalFormat, bool mipmaps = false);
	void create(GLenum target, GLuint width, GLuint height, GLint components,
		GLenum format, GLenum type, const GLvoid *data, GLenum internalFormat, bool mipmaps = false);
	void create3D(GLenum target, GLuint width, GLuint height, GLuint depth, GLint components,
		GLenum format, GLenum type, const GLvoid *data, GLenum internalFormat, bool mipmaps = false);

	void setCubeFace(GLenum face, GLvoid* data);

	bool load_cube(const std::string &fileStart, const std::string &fileExt, bool mipmaps = false);

	void load(const std::string &fileName, LoadOptions::type loadOptions = LoadOptions::None);

	void destroy();

	void bind() { glBindTexture(_target, _handle); }
	void unbind() { glBindTexture(_target, 0); }

	GLuint width() const	{ return _width; }
	GLuint height() const { return _height; }
	GLuint depth() const { return _depth; }
	GLubyte components() const { return _components; }

	GLenum format() const { return _format; }
	GLenum type() const { return _type; }

	GLuint handle() const { return _handle; }

	bool valid() const { return _valid; }
	GLenum target() const { return _target; }
	//const GLubyte *getDataB(GLint mipmaplevel = 0) const throw (usage_error);
	//const float *getDataF() const throw (usage_error);

	const std::string& get_filename() const { return _fileName; }

	size_type size() { return _width * _height * _components; }

	void getData(void* data) { glGetTexImage(_target, 0, _format, _type, data); }
	//GLuint loadGL(bool mipmaps) const;

	void setWrap(WrapMode::type smode, WrapMode::type tmode = WrapMode::None, WrapMode::type rmode = WrapMode::None);

	void setFilter(FilterMode::type minFilter, FilterMode::type magFilter);

	UVQuad convert_uvs(const UVQuad& inuvs)
	{
		if(!valid()) return inuvs;
		if(GL_TEXTURE_RECTANGLE_ARB == target())
		{
			if(UVType::ZeroToOne == inuvs.type)
				return UVQuad(inuvs.u * width(), inuvs.v * height(), inuvs.u2 * width(), inuvs.v2 * height(), UVType::Pixel);
			else 
				return inuvs;
		}
		else
		{
			if(UVType::Pixel == inuvs.type)
				return UVQuad(inuvs.u / width(), inuvs.v / height(), inuvs.u2 / width(), inuvs.v2 / height(), UVType::ZeroToOne);
			else 
				return inuvs;
		}	
	}

private:

	void setWrapInternal(WrapMode::type smode, WrapMode::type tmode, WrapMode::type rmode);

	// 	template<class Archive> 
	// 	void serialize(Archive & ar, const unsigned int version);

	void setTextureParameters();
	GLenum wrapModeToGL(WrapMode::type mode);

	GLenum filterModeToGL(FilterMode::type mode);

	GLenum internalILtoGL(int ilFormat, int ilType, int components);

	GLenum formatILtoGL(int ilFormat);

	int formatGLtoIL(GLenum fmt);

	GLenum typeILtoGL(int ilType);

	int typeGLtoIL(GLenum type);

	bool isPOT(unsigned int val);

	void loadTexture2D( LoadOptions::type loadOptions, unsigned char * data, GLenum internalformat );
};

// class FUSION_API TextureManager
// {
// public:
// 	typedef std::map<std::string, TexturePtr> TextureList;
// 	typedef TextureList::iterator TextureListIterator;
// private:
// 	static boost::shared_ptr<TextureManager> _instance;
// 
// 	TextureList _textures;
// 
// public:
// 	TextureManager();
// 	~TextureManager();
// 	static TextureManager &globalManager();
// 
// 	TexturePtr load(const std::string &fileName, const std::string &textureName)
// 		throw (Exceptions::parameter_error, Exceptions::usage_error, Exceptions::memory_error);
// 	TexturePtr get(const std::string &name)
// 		throw (Exceptions::parameter_error);
// 	TextureListIterator getTextureListBegin();
// 	TextureListIterator getTextureListEnd();
// };
#include "texture.inl"

}

#endif // _GLBASE_TEXTURE_HPP