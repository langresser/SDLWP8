/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_WIN8_METRO

#include "SDL_assert.h"
#include "SDL_windowsvideo.h"

/* WGL implementation of SDL OpenGL support */

#if SDL_VIDEO_OPENGL_WGL
#include "SDL_windowsopengl.h"

static EGLDisplay  m_eglDisplay;
static EGLContext  m_eglContext;
static EGLSurface  m_eglSurface;

SDL_GLContext
WIN_GL_CreateContext(_THIS, SDL_Window * window)
{
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	m_eglDisplay = eglGetDisplay(0);
	if ( m_eglDisplay == EGL_NO_DISPLAY )
	{
		return EGL_FALSE;
	}

	// Initialize EGL
	if ( !eglInitialize(m_eglDisplay, &majorVersion, &minorVersion) )
	{
		return EGL_FALSE;
	}

	// Create a surface
	EGLNativeWindowType hWnd;
	hWnd = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, NULL, hWnd, NULL);
	if ( m_eglSurface == EGL_NO_SURFACE )
	{
		return EGL_FALSE;
	}

	// Create a GL context
	m_eglContext = eglCreateContext(m_eglDisplay, NULL, EGL_NO_CONTEXT, contextAttribs );
	if ( m_eglContext == EGL_NO_CONTEXT )
	{
		return EGL_FALSE;
	}   

	// Make the context current
	if ( !eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) )
	{
		return EGL_FALSE;
	}
    return m_eglContext;
}

int
WIN_GL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context)
{
	// Make the context current
	if ( !eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) )
	{
		return EGL_FALSE;
	}
    return 0;
}

int WIN_GL_LoadLibrary(_THIS, const char* path)
{
	return 0;
}

void WIN_GL_UnloadLibrary(_THIS)
{
}

void * WIN_GL_GetProcAddress(_THIS, const char *proc)
{
	return NULL;
}

int
WIN_GL_SetSwapInterval(_THIS, int interval)
{
    eglSwapInterval(m_eglDisplay, interval);
    return 0;
}

void
WIN_GL_SwapWindow(_THIS, SDL_Window * window)
{
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

void
WIN_GL_DeleteContext(_THIS, SDL_GLContext context)
{
	eglDestroySurface(m_eglDisplay, m_eglSurface);
	eglDestroyContext(m_eglDisplay, m_eglContext);
	eglTerminate(m_eglDisplay);
}

#endif /* SDL_VIDEO_OPENGL_WGL */

#endif /* SDL_VIDEO_DRIVER_WINDOWS */

/* vi: set ts=4 sw=4 expandtab: */
