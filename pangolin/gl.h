/* This file is part of the Pangolin Project.
 * http://github.com/stevenlovegrove/Pangolin
 *
 * Copyright (c) 2011 Steven Lovegrove
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PANGOLIN_GL_H
#define PANGOLIN_GL_H

#include "platform.h"

#ifdef _WIN_
#include <Windows.h>
#endif

#include <GL/glew.h>

#ifdef _OSX_
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <math.h>

namespace pangolin
{

////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////

class PANGOLIN_API GlTexture
{
public:
  //! internal_format normally one of GL_RGBA8, GL_LUMINANCE8, GL_INTENSITY16
  GlTexture(GLint width, GLint height, GLint internal_format = GL_RGBA8, bool sampling_linear = true );

  //! Default constructor represents 'no texture'
  GlTexture();
  ~GlTexture();

  //! Reinitialise teture width / height / format
  void Reinitialise(GLint width, GLint height, GLint internal_format = GL_RGBA8, bool sampling_linear = true );

  void Bind() const;
  void Unbind() const;

  //! data_layout normally one of GL_LUMINANCE, GL_RGB, ...
  //! data_type normally one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_FLOAT
  void Upload(void* image, GLenum data_layout = GL_LUMINANCE, GLenum data_type = GL_FLOAT);

  void SetLinear();
  void SetNearestNeighbour();

  void RenderToViewport(const bool flip) const;
  void RenderToViewport() const;
  void RenderToViewportFlipY() const;

  void Render3DPlanTexture(float width, float height) const;

  GLint internal_format;
  GLuint tid;
  GLint width;
  GLint height;

private:
  // Private copy constructor
  GlTexture(const GlTexture&) {}
};

struct PANGOLIN_API GlRenderBuffer
{
  GlRenderBuffer(GLint width, GLint height, GLint internal_format = GL_DEPTH_COMPONENT24);
  ~GlRenderBuffer();

  GLuint rbid;
};

struct PANGOLIN_API GlFramebuffer
{
  GlFramebuffer();
  GlFramebuffer(GlTexture& colour, GlRenderBuffer& depth);
  GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlRenderBuffer& depth);
  ~GlFramebuffer();

  void Bind() const;
  void Unbind() const;

  GLuint fbid;
  unsigned attachments;
};

PANGOLIN_API void glColorHSV( double hue, double s, double v );
PANGOLIN_API void glColorBin( int bin, int max_bins, double sat = 1.0, double val = 1.0 );
PANGOLIN_API void glPixelTransferScale( float r, float g, float b );
PANGOLIN_API void glPixelTransferScale( float scale );

////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////

const int MAX_ATTACHMENTS = 8;

const static GLuint attachment_buffers[] = {
    GL_COLOR_ATTACHMENT0_EXT,
    GL_COLOR_ATTACHMENT1_EXT,
    GL_COLOR_ATTACHMENT2_EXT,
    GL_COLOR_ATTACHMENT3_EXT,
    GL_COLOR_ATTACHMENT4_EXT,
    GL_COLOR_ATTACHMENT5_EXT,
    GL_COLOR_ATTACHMENT6_EXT,
    GL_COLOR_ATTACHMENT7_EXT
};

//template<typename T>
//struct GlDataTypeTrait {};
//template<> struct GlDataTypeTrait<float>{ static const GLenum type = GL_FLOAT; };
//template<> struct GlDataTypeTrait<int>{ static const GLenum type = GL_INT; };
//template<> struct GlDataTypeTrait<unsigned char>{ static const GLenum type = GL_UNSIGNED_BYTE; };

inline GlTexture::GlTexture()
    : internal_format(0), tid(0), width(0), height(0)
{
  // Not a texture constructor
}

inline GlTexture::GlTexture(GLint width, GLint height, GLint internal_format, bool sampling_linear )
    : internal_format(0), tid(0)
{
  Reinitialise(width,height,internal_format,sampling_linear);
}

inline GlTexture::~GlTexture()
{
  if(internal_format!=0) {
    glDeleteTextures(1,&tid);
  }
}

inline void GlTexture::Bind() const
{
  glBindTexture(GL_TEXTURE_2D, tid);
}

inline void GlTexture::Unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

inline void GlTexture::Reinitialise(GLint w, GLint h, GLint int_format, bool sampling_linear )
{
    if(tid!=0) {
      glDeleteTextures(1,&tid);
    }

    internal_format = int_format;
    width = w;
    height = h;

    glGenTextures(1,&tid);
    Bind();
    // GL_LUMINANCE and GL_FLOAT don't seem to actually affect buffer, but some values are required
    // for call to succeed.
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_LUMINANCE,GL_FLOAT,0);

    if(sampling_linear) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

inline void GlTexture::Upload(void* image, GLenum data_layout, GLenum data_type )
{
  Bind();
  glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,data_layout,data_type,image);
}

inline void GlTexture::SetLinear()
{
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  Unbind();
}

inline void GlTexture::SetNearestNeighbour()
{
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  Unbind();
}

inline void GlTexture::RenderToViewport(const bool flip) const
{
    if(flip) {
        RenderToViewportFlipY();
    }else{
        RenderToViewport();
    }
}

inline void GlTexture::RenderToViewport() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Bind();
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex2d(-1,-1);
  glTexCoord2f(1, 0); glVertex2d(1,-1);
  glTexCoord2f(1, 1); glVertex2d(1,1);
  glTexCoord2f(0, 1); glVertex2d(-1,1);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

inline void GlTexture::RenderToViewportFlipY() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Bind();
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex2d(-1,1);
  glTexCoord2f(1, 0); glVertex2d(1,1);
  glTexCoord2f(1, 1); glVertex2d(1,-1);
  glTexCoord2f(0, 1); glVertex2d(-1,-1);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

inline void GlTexture::Render3DPlanTexture(float width, float height) const
{
    Bind();
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

inline GlRenderBuffer::GlRenderBuffer(GLint width, GLint height, GLint internal_format )
{
  glGenRenderbuffersEXT(1, &rbid);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rbid);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, internal_format, width, height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

inline GlRenderBuffer::~GlRenderBuffer()
{
  glDeleteRenderbuffersEXT(1, &rbid);
}

inline GlFramebuffer::GlFramebuffer()
{
  glGenFramebuffersEXT(1, &fbid);
}

inline GlFramebuffer::GlFramebuffer(GlTexture& colour, GlRenderBuffer& depth)
{
  glGenFramebuffersEXT(1, &fbid);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbid);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colour.tid, 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth.rbid);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  attachments = 1;
}

inline GlFramebuffer::GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlRenderBuffer& depth)
{
  glGenFramebuffersEXT(1, &fbid);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbid);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colour0.tid, 0);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, colour1.tid, 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth.rbid);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  attachments = 2;
}

inline GlFramebuffer::~GlFramebuffer()
{
  glDeleteFramebuffersEXT(1, &fbid);
}

inline void GlFramebuffer::Bind() const
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbid);
  glDrawBuffers( attachments, attachment_buffers );
}

inline void GlFramebuffer::Unbind() const
{
  glDrawBuffers( 1, attachment_buffers );
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

}

#endif // PANGOLIN_GL_H
