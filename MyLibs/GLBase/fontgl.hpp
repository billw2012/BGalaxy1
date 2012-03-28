#ifndef _RENDER_FONTGL_HPP
#define _RENDER_FONTGL_HPP

#include <string>

#include "texture.hpp"

namespace glbase
{

// font class ripped from a Nehe tutorial!
struct FontGL
{
private:
	Texture _data;
	GLuint _base;				// Base Display List For The Font
	GLuint _count;
	float _scale;

public:
	FontGL() : _data(), _base(0), _count(0), _scale(1.0f) {}
	~FontGL()
	{
		destroy();
	}

	void setScale(float scale)
	{
		_scale = scale;
	}

	void load(const std::string& filename)
	{
		static const GLuint charsx = 16;
		static const GLuint charsy = 16;

		_count = charsx * charsy;
		_data.load(filename);

		float charwid = static_cast<float>(_data.width()/charsx);
		float charhgt = static_cast<float>(_data.height()/charsy);

		float charu = 1.0f/charsx;
		float charv = 1.0f/charsy;
		_base=glGenLists(_count);					// Creating 256 Display Lists
		_data.bind();
	
		std::vector<unsigned char> buf(256*256*3);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &buf[0]);

		//glBindTexture(GL_TEXTURE_2D, texture[0]);			// Select Our Font Texture
		for (GLuint loop = 0; loop < _count; ++loop)		// Loop Through All 256 Lists
		{
			float cx=static_cast<float>(loop%charsx)/static_cast<float>(charsx);	// X Position Of Current Character
			float cy=static_cast<float>(loop/charsy)/static_cast<float>(charsy);	// Y Position Of Current Character

			glNewList(_base+loop, GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);								// Use A Quad For Each Character
			glTexCoord2f(cx, 1-cy-charv);					// Texture Coord (Bottom Left)
			glVertex2i(0, 0);								// Vertex Coord (Bottom Left)
			glTexCoord2f(cx+charu, 1-cy-charv);				// Texture Coord (Bottom Right)
			glVertex2i(16, 0);								// Vertex Coord (Bottom Right)
			glTexCoord2f(cx+charu, 1-cy);					// Texture Coord (Top Right)
			glVertex2i(16, 16);								// Vertex Coord (Top Right)
			glTexCoord2f(cx, 1-cy);							// Texture Coord (Top Left)
			glVertex2i(0, 16);								// Vertex Coord (Top Left)
			glEnd();										// Done Building Our Quad (Character)
			glTranslated(16, 0, 0);							// Move To The Right Of The Character
			glEndList();									// Done Building The Display List
		}
	}

	void destroy()
	{
		glDeleteLists(_base, _count);
	}

	void print(GLint x, GLint y, const std::string& str, unsigned int set = 0)	
	{
		if(set > 1)
		{
			set = 1;
		}

		_data.bind();
		//glBindTexture(GL_TEXTURE_2D, texture[0]);			// Select Our Font Texture
		glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glPushMatrix();										// Store The Projection Matrix
			glLoadIdentity();									// Reset The Projection Matrix
			glOrtho(0,640,0,480,-1,1);							// Set Up An Ortho Screen
			glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
			glPushMatrix();										// Store The Modelview Matrix
				glLoadIdentity();									// Reset The Modelview Matrix
				glScalef(_scale, _scale, _scale);
				glTranslated(x,y,0);								// Position The Text (0,0 - Bottom Left)
				glListBase(_base-32+(128*set));						// Choose The Font Set (0 or 1)
				glCallLists(static_cast<GLsizei>(str.length()), GL_UNSIGNED_BYTE, &str[0]);// Write The Text To The Screen
				glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
			glPopMatrix();										// Restore The Old Projection Matrix
			glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glPopMatrix();										// Restore The Old Projection Matrix
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	}

};

}

#endif // _RENDER_FONTGL_HPP