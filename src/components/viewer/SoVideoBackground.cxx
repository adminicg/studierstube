/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube 
 * ======================================================================== */
/** The header file for the SoVideoBackground class.
  *
  * @author Denis Kalkofen
  *
  * $Id: SoVideoBackground.cxx 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/viewer/SoVideoBackground.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/video/Video.h>
#include <stb/base/OS.h>

#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/kernel/SceneManager.h>

#ifdef HAVE_OPENVIDEO
#  define ENABLE_VIDEOSINK
#  include <openvideo/nodes/VideoSink.h>
#  include <openvideo/nodes/VideoSinkSubscriber.h>
#  include <openvideo/State.h>
#else
#  pragma message(">>> HAVE_OPENVIDEO not defined - video background will not be available!")
#endif


#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>	

BEGIN_NAMESPACE_STB


static void scale_2x_WinXP(const unsigned short *src, unsigned short *dst, int width, int height);


SO_NODE_SOURCE(SoVideoBackground);


void
SoVideoBackground::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackground, SoNode, "Node");
}


// used to calculate best texture size
static int
getNextPowerOfTwo(unsigned int nValue)
{
	if(nValue<=1)		return 1;
	if(nValue<=2)		return 2;
	if(nValue<=4)		return 4;
	if(nValue<=8)		return 8;
	if(nValue<=16)		return 16;
	if(nValue<=32)		return 32;
	if(nValue<=64)		return 64;
	if(nValue<=128)		return 128;
	if(nValue<=256)		return 256;
	if(nValue<=512)		return 512;
	if(nValue<=1024)	return 1024;
	if(nValue<=2048)	return 2048;

	return 0;
}


struct VideoBackgroundTexInfo
{
	GLuint texID;
	GLenum format;
	GLint internalFormat;

	int imgWidth,imgHeight;
	int texWidth,texHeight;
	float u0,v0, u1,v1;
	unsigned int updateCtr;
};


SoVideoBackground::SoVideoBackground()
{
	SO_NODE_CONSTRUCTOR(SoVideoBackground);

	texInfo = NULL;
	initialized = false;

#ifdef _IS_KLIMTES_
	bufferSynchronizer = new openvideo::BufferSynchronizer();
#endif
}


void 
SoVideoBackground::GLRender(SoGLRenderAction*)
{
	if(!initialized)
		initialized = init();

#ifndef _IS_KLIMTES_
	drawTexture();
#else
	blitIntoVideoMemory();
#endif
}


bool
SoVideoBackground::init()
{
	if(Video* video = (Video*)Kernel::getInstance()->getComponentManager()->load("Video"))
	{
		video->vp_registerVideoUser(this);
		return true;
	}

    return false;
}


void
SoVideoBackground::vu_init(const openvideo::Buffer& frame)
{
}


void
SoVideoBackground::vu_update(const openvideo::Buffer& frame)
{
#ifdef HAVE_OPENVIDEO
#  ifndef _IS_KLIMTES_
	updateTexture(frame);
#  else
	bufferSynchronizer->assign(const_cast<openvideo::Buffer*>(&frame));
#  endif
#endif
}


bool
SoVideoBackground::createTexture(const openvideo::Buffer& buffer)
{
#if defined(HAVE_OPENVIDEO) && !defined(_IS_KLIMTES_)

	texInfo = new VideoBackgroundTexInfo;

	if(!openvideo::PixelFormat::toOGL(buffer.getFormat(), texInfo->format, texInfo->internalFormat))
	{
		delete texInfo;
		texInfo = NULL;
		logPrintE("SoVideoBackground:ERROR: unsupported pixel-format\n");
		return false;
	}

	texInfo->updateCtr = 0;
	texInfo->imgWidth = buffer.getWidth();
	texInfo->imgHeight = buffer.getHeight();
	texInfo->texWidth = getNextPowerOfTwo(buffer.getWidth());
	texInfo->texHeight = getNextPowerOfTwo(buffer.getHeight());

	bool	flip_h = false, flip_v = true;
	float	u_rt = texInfo->imgWidth / (float)texInfo->texWidth,
			v_rt = texInfo->imgHeight / (float)texInfo->texHeight;

	texInfo->u0 = (flip_h ? u_rt : 0   );
	texInfo->u1 = (flip_h ?    0 : u_rt);
	texInfo->v0 = (flip_v ? v_rt : 0   );
	texInfo->v1 = (flip_v ?    0 : v_rt);

	// Create the OpenGL texture
	//
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texInfo->texID);
	glBindTexture(GL_TEXTURE_2D, texInfo->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, texInfo->internalFormat, texInfo->texWidth, texInfo->texHeight, 0, texInfo->format, GL_UNSIGNED_BYTE, NULL);
	glDisable(GL_TEXTURE_2D);
	return true;
#endif // defined(HAVE_OPENVIDEO) && !defined(_IS_KLIMTES_)

	return false;
}


void
SoVideoBackground::updateTexture(const openvideo::Buffer& buffer)
{
#if defined(HAVE_OPENVIDEO) && !defined(_IS_KLIMTES_)
	if(!texInfo)
		if(!createTexture(buffer))
			return;

	if(buffer.getUpdateCounter() != texInfo->updateCtr)
	{
		//printf("tex-update: %d\n", buffer.getUpdateCounter());
		texInfo->updateCtr = buffer.getUpdateCounter();

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texInfo->texID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texInfo->imgWidth, texInfo->imgHeight, texInfo->format, GL_UNSIGNED_BYTE, (void*)buffer.getPixels());
		glDisable(GL_TEXTURE_2D);

		GLenum e;
		while ((e = glGetError ()) != GL_NO_ERROR)
		{
			printf("checkGLErrors(): GL error: %s\n", gluErrorString(e));
		}

		touch();
	}
#endif // defined(HAVE_OPENVIDEO) && !defined(_IS_KLIMTES_)
}


void
SoVideoBackground::drawTexture()
{
#if defined(HAVE_OPENVIDEO) && !defined(_IS_KLIMTES_)
	if(!texInfo)
		return;

	/////////preGLCalls()
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	///////////////////////    

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);


	// redraw texture
	glBindTexture(GL_TEXTURE_2D,  texInfo->texID);

	glBegin(GL_QUADS);
		glTexCoord2f(texInfo->u0,texInfo->v0); glVertex3f(-1.0f, -1.0f,  0.0f);
		glTexCoord2f(texInfo->u1,texInfo->v0); glVertex3f( 1.0f, -1.0f,  0.0f);
		glTexCoord2f(texInfo->u1,texInfo->v1); glVertex3f( 1.0f,  1.0f,  0.0f);
		glTexCoord2f(texInfo->u0,texInfo->v1); glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	///postGLCalls()
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
#endif // defined(HAVE_OPENVIDEO) !defined(_IS_KLIMTES_)
}


void
SoVideoBackground::blitIntoVideoMemory()
{
#if defined(HAVE_OPENVIDEO) && defined(_IS_KLIMTES_)
	if(openvideo::Buffer* buffer = bufferSynchronizer->getLocked())
	{
		if(!buffer->getPixels())
		{
			buffer->unlock();
			return;
		}

		if(unsigned short* oglPixels = (unsigned short*)klesGetBufferPixels())
		{
			int oglWidth = klesGetBufferWidth(),
				oglHeight = klesGetBufferHeight();

			if(oglWidth==buffer->getWidth()*2 && oglHeight==buffer->getHeight()*2)
				scale_2x_WinXP((unsigned short*)buffer->getPixels(), oglPixels, buffer->getWidth(),buffer->getHeight());
		}

		buffer->unlock();
	}
#endif // defined(HAVE_OPENVIDEO) && defined(_IS_KLIMTES_)
}



//////////////////////////////////////////////////////////////////////////
//
//      Tool Functions for blitting video images into video memory
//

static void
scale_2x_WinXP(const unsigned short *src, unsigned short *dst, int width, int height)
{
	// WinXP 16-bits rendering uses 1555 with first bit unused
	// We therefore need to shift the first two components one bit to the right
	//

	unsigned int *dst0 = reinterpret_cast<unsigned int*>(dst);					// destination pointer for even lines
	unsigned int *dst1 = reinterpret_cast<unsigned int*>(dst+2*width);			// destination pointer for odd lines
	int x, width_half = width/2;
	unsigned int srcRaw,srcFixed;

	for(int y=0; y<height; y++)
	{
		// do a fast scanline up-scale with loop unrolling, writing 8 pixels per loop
		//

		x = width_half+1;			// +1 for pre-decrement (faster than post-decrement)
		while(--x)
		{
			srcRaw = *src++;
			srcFixed = (srcRaw & 0x001F)  | ((srcRaw>>1) & 0x7FE0);

			srcFixed |= srcFixed<<16;

			// write 4 pixels at once
			*dst0++ = srcFixed;
			*dst1++ = srcFixed;


			srcRaw = *src++;
			srcFixed = (srcRaw & 0x001F)  | ((srcRaw>>1) & 0x7FE0);

			srcFixed |= srcFixed<<16;

			// write 4 pixels at once
			*dst0++ = srcFixed;
			*dst1++ = srcFixed;
		}

		dst0 += width;		// skip odd lines
		dst1 += width;		// skip even lines
	}
}



END_NAMESPACE_STB

