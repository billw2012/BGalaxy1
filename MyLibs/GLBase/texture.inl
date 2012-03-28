inline Texture::Texture() 
	: _handle(0),
	_valid(false),
	_width(0),
	_height(0),
	_depth(0),
	_components(0),
	_format(0),
	_internalformat(0),
	_type(0),
	_target(0),
	_mipmapped(false) {}

inline Texture::Texture(const std::string& fileName, LoadOptions::type loadOptions)
	: _handle(0),
	_valid(false),
	_width(0),
	_height(0),
	_depth(0),
	_components(0),
	_format(0),
	_internalformat(0),
	_type(0),
	_target(0),
	_mipmapped(false)
{
	if(fileName != "")
		load(fileName, loadOptions);
}

inline void Texture::create1D(GLenum target, GLuint width, 
							GLint components, GLenum format, GLenum type, 
							const GLvoid *data, GLenum internalFormat, bool mipmaps) 
{
	create3D(target, width, 0, 0, components, format, type, data, internalFormat, mipmaps);
}

inline void Texture::create(GLenum target, GLuint width, GLuint height, 
							GLint components, GLenum format, GLenum type, 
							const GLvoid *data, GLenum internalFormat, bool mipmaps) 
{
	create3D(target, width, height, 0, components, format, type, data, internalFormat, mipmaps);
}

inline void Texture::create3D(GLenum target, GLuint width, GLuint height, GLuint depth,
	GLint components, GLenum format, GLenum type, 
	const GLvoid *data, GLenum internalFormat, bool mipmaps) 
{
	destroy();

	_target = target;
	glGenTextures(1, &_handle);
	glBindTexture(_target, _handle);
	setTextureParameters();
	//if(mipmaps && target != GL_TEXTURE_CUBE_MAP && data != NULL)
	//{
	//	gluBuild2DMipmaps(_target, components, width, height, format, type, data);
	//	_mipmapped = true;
	//}
	//else
	if(target == GL_TEXTURE_3D || target == GL_TEXTURE_2D_ARRAY)
	{
		glTexImage3D(_target, 0, internalFormat, width, height, depth, 0, format, type, data);
	}
	else if(target == GL_TEXTURE_2D || target == GL_TEXTURE_RECTANGLE || target == GL_TEXTURE_1D_ARRAY)
	{
		glTexImage2D(_target, 0, internalFormat, width, height, 0, format, type, data);
		if(mipmaps)
			glGenerateMipmapEXT(_target);
		_mipmapped = mipmaps;
	}
	else if(target == GL_TEXTURE_1D)
	{
		glTexImage1D(_target, 0, internalFormat, width, 0, format, type, data);
		if(mipmaps)
			glGenerateMipmapEXT(_target);
		_mipmapped = mipmaps;
	}
	else
	{
		_mipmapped = false;
	}

	_width = width;
	_height = height;
	_components = components;
	_format = format;
	_internalformat = internalFormat;
	_type = type;
	_target = target;

	_valid = true;

	glBindTexture(_target, 0);
}

inline void Texture::setCubeFace(GLenum face, GLvoid* data)
{
	assert(_target == GL_TEXTURE_CUBE_MAP);
	glTexImage2D(face, 0, _internalformat, _width, _height, 0, _format, _type, data);
}

inline bool Texture::load_cube(const std::string &fileStart, const std::string &fileExt, bool mipmaps)
{
	destroy();

	glGenTextures(1, &_handle);

	ILint format; 
	ILint type;
	GLenum internalformat;
	_target = GL_TEXTURE_CUBE_MAP;
	glBindTexture(_target, _handle);
	setTextureParameters();
	for(unsigned short face = 0; face < 6; ++face)
	{
		ILuint ilhandle;
		ilGenImages(1, &ilhandle);
		ilBindImage(ilhandle);
		std::stringstream fileName;
		fileName << fileStart << face << '.' << fileExt;
		ilLoadImage(fileName.str().c_str());
		throw_on_IL_error();

		if(face == 0)
		{
			_width = ilGetInteger(IL_IMAGE_WIDTH);
			_height = ilGetInteger(IL_IMAGE_HEIGHT);
			if(!isPOT(_width) || !isPOT(_height) || _width != _height)
				throw std::exception("Image is a cube map but either width != height, or they are not both powers of 2.");
			_depth = ilGetInteger(IL_IMAGE_DEPTH);
			_components = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
			format = ilGetInteger(IL_IMAGE_FORMAT); 
			_format = formatILtoGL(format);
			type = ilGetInteger(IL_IMAGE_TYPE);
			_type = typeILtoGL(type);
			internalformat = internalILtoGL(format, type, _components);
		}
		else
		{
			if(_width != ilGetInteger(IL_IMAGE_WIDTH))
				throw std::exception("Image widths to not match.");
			if(_height != ilGetInteger(IL_IMAGE_HEIGHT))
				throw std::exception("Image widths to not match.");
			if(_depth != ilGetInteger(IL_IMAGE_DEPTH))
				throw std::exception("Image widths to not match.");
			if(_components != ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL))
				throw std::exception("Image widths to not match.");
			if(format != ilGetInteger(IL_IMAGE_FORMAT))
				throw std::exception("Face formats do not match.");
			if(type != ilGetInteger(IL_IMAGE_TYPE))
				throw std::exception("Face types do not match.");
		}
		if(ilGetInteger(IL_NUM_IMAGES) != 0)
			throw std::exception("Face contains more than a single image.");
		if(ilGetInteger(IL_IMAGE_CUBEFLAGS) != 0)
			throw std::exception("Yo dawg I heard you like cube-maps so you tried to put a cube-map inside your cube-map and crashed this program.");

		ilBindImage(ilhandle);
		if(mipmaps)
			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, _components, _width, _height, _format, _type, ilGetData());
		else
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, 0, internalformat, _width, _height, 0, _format, _type, ilGetData());
		ilDeleteImage(ilhandle);
	}
	_valid = true;
	glBindTexture(_target, 0);

	return _valid;
}

inline void Texture::load(const std::string &fileName, LoadOptions::type loadOptions)
{
	destroy();

	ILuint ilhandle;
	ilGenImages(1, &ilhandle);
	ilBindImage(ilhandle);
	ilLoadImage(fileName.c_str());
	throw_on_IL_error();

	_width = ilGetInteger(IL_IMAGE_WIDTH);
	_height = ilGetInteger(IL_IMAGE_HEIGHT);
	_depth = ilGetInteger(IL_IMAGE_DEPTH);
	_components = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	ILint format = ilGetInteger(IL_IMAGE_FORMAT); 
	ILint type = ilGetInteger(IL_IMAGE_TYPE);
	ILint images = ilGetInteger(IL_NUM_IMAGES);
	ILenum cubeFlags = ilGetInteger(IL_IMAGE_CUBEFLAGS);
	//ILint mips = ilGetInteger(IL_NUM_MIPMAPS);
	//ILint activelayer = ilGetInteger(IL_ACTIVE_LAYER);
	//ILint activeimage = ilGetInteger(IL_ACTIVE_IMAGE);


	_format = formatILtoGL(format);
	_type = typeILtoGL(type);
	GLenum internalformat = internalILtoGL(format, type, _components);

	unsigned char *data = ilGetData();

	// determine if image is pot or rect, if it is pot determine if it is a cube map or a 3d map
	glGenTextures(1, &_handle);
	// if its a cube map
	if(cubeFlags != 0)
	{
		if(!isPOT(_width) || !isPOT(_height) || _width != _height)
			throw std::exception("Image is a cube map but either width != height, or they are not both powers of 2.");
		_target = GL_TEXTURE_CUBE_MAP;
		//glEnable(_target);
		glBindTexture(_target, _handle);
		setTextureParameters();
		for(int i=0; i<=6; i++)
		{
			ilBindImage(ilhandle);
			ilActiveFace(i);
				//gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, _components, _width, _height, _format, _type, ilGetData());
			//else
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, internalformat, _width, _height, 0, _format, _type, ilGetData());
			if(loadOptions & LoadOptions::GenerateMipmaps)
				glGenerateMipmapEXT(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i);
		}
		ilBindImage(ilhandle);
		ilActiveFace(0);
		glBindTexture(_target, 0);
	}
	else if(images == 0)
	{
		loadTexture2D(loadOptions, data, internalformat);
		//if(isPOT(_width) && isPOT(_height))
		//{
		//	_target = GL_TEXTURE_2D;
		//}
		//else
		//{
		//	_target = GL_TEXTURE_RECTANGLE_ARB;
		//}
		////glEnable(_target);
		//glBindTexture(_target, _handle);
		//setTextureParameters();
		//if(mipmaps)
		//	gluBuild2DMipmaps(_target, _components, _width, _height, _format, _type, data);
		//else
		//	glTexImage2D(_target, 0, internalformat, _width, _height, 0, _format, _type, data);
	}
	else
	{
		//if(!isPOT(_width) || !isPOT(_height) || _width != height)
		//	throw new Exceptions::file_error("Image is a cube map but either width != height, or they are not both powers of 2.");
	}

	ilDeleteImage(ilhandle);

	_fileName = fileName;
	_valid = true;
}

inline void Texture::destroy()
{
	if(_valid)
		glDeleteTextures(1, &_handle);
	_valid = false;
}

inline void Texture::setWrap(WrapMode::type smode, WrapMode::type tmode, WrapMode::type rmode)
{
	glBindTexture(_target, _handle);
	setWrapInternal(smode, tmode, rmode);
	glBindTexture(_target, 0);
}

inline void Texture::setWrapInternal(WrapMode::type smode, WrapMode::type tmode, WrapMode::type rmode)
{
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, wrapModeToGL(smode));
	if(tmode != WrapMode::None)
		glTexParameteri(_target, GL_TEXTURE_WRAP_T, wrapModeToGL(tmode));
	if(rmode != WrapMode::None)
		glTexParameteri(_target, GL_TEXTURE_WRAP_R, wrapModeToGL(rmode));
}

inline void Texture::setFilter(FilterMode::type minFilter, FilterMode::type magFilter)
{
	switch(magFilter)
	{
	case FilterMode::Linear:
		glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	};

	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, filterModeToGL(minFilter));
}

inline void Texture::setTextureParameters()
{
	if (_target == GL_TEXTURE_2D || _target == GL_TEXTURE_3D || _target == GL_TEXTURE_RECTANGLE_ARB) 
	{
		setWrapInternal(WrapMode::Repeat, WrapMode::Repeat, WrapMode::Repeat);
		//glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//if(_target == GL_TEXTURE_3D)
		//	glTexParameteri(_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}
	else if (_target == GL_TEXTURE_CUBE_MAP) 
	{
		setWrapInternal(WrapMode::ClampToEdge, WrapMode::ClampToEdge, WrapMode::ClampToEdge);
		//glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	setFilter(FilterMode::Linear, FilterMode::Linear);
	if(_target == GL_TEXTURE_2D_ARRAY_EXT)
	{
		glTexParameteri( GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri( GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		setWrapInternal(WrapMode::ClampToEdge, WrapMode::ClampToEdge, WrapMode::ClampToEdge);	
	}
	////glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
}

inline GLenum Texture::wrapModeToGL(WrapMode::type mode)
{
	switch(mode)
	{
	case WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
	case WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
		//case WrapMode::Repeat: return GL_REPEAT;
	default: return GL_REPEAT;
	};
}

inline GLenum Texture::filterModeToGL(FilterMode::type mode)
{
	switch(mode)
	{
	case FilterMode::Linear: return (_mipmapped?  GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	default: return (_mipmapped? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
	};
}

inline GLenum Texture::internalILtoGL(int ilFormat, int ilType, int components)
{
	if(ilFormat == IL_BGR || ilFormat == IL_RGB)
		return GL_RGB8;
	if(ilFormat == IL_BGRA || ilFormat == IL_RGBA)
		return GL_RGBA8;
	if(components == 1)
		return GL_LUMINANCE8;
	return components;
}

inline GLenum Texture::formatILtoGL(int ilFormat)
{
	switch(ilFormat)
	{
	case IL_BGR:
		return GL_BGR;
	case IL_BGRA:
		return GL_BGRA;
	case IL_COLOUR_INDEX:
		return GL_COLOR_INDEX;
	case IL_LUMINANCE:
		return GL_LUMINANCE;
	case IL_LUMINANCE_ALPHA:
		return GL_LUMINANCE_ALPHA;
	case IL_RGB:
		return GL_RGB;
	case IL_RGBA:
		return GL_RGBA;
	default:
		return -1;
	};		
}

inline int Texture::formatGLtoIL(GLenum fmt)
{
	switch(fmt)
	{
	case GL_BGR:
		return IL_BGR;
	case GL_BGRA:
		return IL_BGRA;
	case GL_COLOR_INDEX:
		return IL_COLOUR_INDEX;
	case GL_LUMINANCE:
		return IL_LUMINANCE;
	case GL_LUMINANCE_ALPHA:
		return IL_LUMINANCE_ALPHA;
	case GL_RGB:
		return IL_RGB;
	case GL_RGBA:
		return IL_RGBA;
	default:
		return -1;
	};		
}

inline GLenum Texture::typeILtoGL(int ilType)
{
	switch(ilType)
	{
	case IL_BYTE:
		return GL_BYTE;
	case IL_SHORT:
		return GL_SHORT;
	case IL_INT:
		return GL_INT;
	case IL_UNSIGNED_BYTE:
		return GL_UNSIGNED_BYTE;
	case IL_UNSIGNED_SHORT:
		return GL_UNSIGNED_SHORT;
	case IL_UNSIGNED_INT:
		return GL_UNSIGNED_INT;
	case IL_FLOAT:
		return GL_FLOAT;
	case IL_DOUBLE:
		return GL_DOUBLE;
	default:
		return -1;
	};		
}

inline int Texture::typeGLtoIL(GLenum type)
{
	switch(type)
	{
	case GL_BYTE:
		return IL_BYTE;
	case GL_SHORT:
		return IL_SHORT;
	case GL_INT:
		return IL_INT;
	case GL_UNSIGNED_BYTE:
		return IL_UNSIGNED_BYTE;
	case GL_UNSIGNED_SHORT:
		return IL_UNSIGNED_SHORT;
	case GL_UNSIGNED_INT:
		return IL_UNSIGNED_INT;
	case GL_FLOAT:
		return IL_FLOAT;
	case GL_DOUBLE:
		return IL_DOUBLE;
	default:
		return -1;
	};		
}

inline bool Texture::isPOT(unsigned int val)
{
	while(val != 1)
	{
		if((val & 1) != 0)
			return false;
		val >>= 1;
	}
	return true;
}

inline void Texture::loadTexture2D( LoadOptions::type loadOptions, unsigned char * data, GLenum internalformat )
{
	if(isPOT(_width) && isPOT(_height) && !(loadOptions & LoadOptions::AsRect))
	{
		_target = GL_TEXTURE_2D;
	}
	else
	{
		_target = GL_TEXTURE_RECTANGLE_ARB;
	}

	glEnable(_target);
	glBindTexture(_target, _handle);
	if(loadOptions & LoadOptions::GenerateMipmaps)
		_mipmapped = true;
	setTextureParameters();
	if(_mipmapped)
		gluBuild2DMipmaps(_target, _components, _width, _height, _format, _type, data);
	else
		glTexImage2D(_target, 0, internalformat, _width, _height, 0, _format, _type, data);
	glBindTexture(_target, 0);
	glDisable(_target);
	//if(loadOptions & LoadOptions::GenerateMipmaps)
	//	glGenerateMipmapEXT(_target);
}