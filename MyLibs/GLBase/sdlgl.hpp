#ifndef _RENDER_SDLGL_HPP
#define _RENDER_SDLGL_HPP

#include <stdexcept>
//#define GL_GLEXT_PROTOTYPES
#define NO_SDL_GLEXT 
#include <gl/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

//#pragma comment(lib, "glew32.lib")

namespace glbase {;

struct SDLGl
{
private:
	bool _sdlInitialized;
	bool _openGLInitialized;
	SDL_Surface *_surface;
	unsigned int _viewWidth;
	unsigned int _viewHeight;

public:
	SDLGl() : _sdlInitialized(false), _openGLInitialized(false), _surface(NULL),
		_viewWidth(0), _viewHeight(0) {}
	~SDLGl()
	{
		shutDownSDL();
	}

	void initSDL() /*throw (std::runtime_error)*/
	{
		if(_sdlInitialized)
			return;
		if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			SDL_Quit();
			throw std::runtime_error("SDL library initialization failed!");
		}
		_sdlInitialized = true;
	}

	void shutDownSDL()
	{
		if(_sdlInitialized)
			SDL_Quit();
		_sdlInitialized = false;
	}

	void initOpenGL(unsigned int width, unsigned int height, unsigned int bpp = 0, bool fullscreen = false) /*throw (std::exception, std::runtime_error)*/
	{
		if(!_sdlInitialized)
			throw std::exception("Trying to initialize OpenGL before SDL has been initialized!");
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,  1);
		setVideoMode(width, height, bpp, fullscreen);

		_openGLInitialized = true;
	}

	bool sdlInitialized() const
	{
		return _sdlInitialized;
	}

	bool openGLInitialized() const
	{
		return _openGLInitialized;
	}

	void swapBuffers() /*throw (std::exception)*/
	{
		if(!_openGLInitialized)
			throw std::exception("Trying to swap buffers before OpenGL has been initialized!");
		SDL_GL_SwapBuffers();
	}

	unsigned int width() const { return _viewWidth; }
	unsigned int height() const { return _viewHeight; }

protected:
	void setVideoMode(unsigned int width, unsigned int height, unsigned int bpp, bool fullscreen) /*throw (std::runtime_error)*/
	{
		unsigned int flags = SDL_OPENGL | SDL_ANYFORMAT | SDL_HWSURFACE;
		if(fullscreen)
			flags |= SDL_FULLSCREEN;
		_surface = SDL_SetVideoMode(width, height, bpp, flags);
		if (_surface == NULL)
		{
			std::string errMsg = "Cannot set video mode: ";
			errMsg += SDL_GetError();
			throw std::runtime_error(errMsg);
		}
		glViewport(0, 0, width, height);
		_viewWidth = width;
		_viewHeight = height;
	}
};

}

#endif // _RENDER_SDLGL_HPP