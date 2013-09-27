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

#ifndef _SDL_windowsopengl_h
#define _SDL_windowsopengl_h

#if SDL_VIDEO_OPENGL_WGL
#include "SDL_opengles2.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_GLContext WIN_GL_CreateContext(_THIS, SDL_Window * window);
extern int WIN_GL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
extern int WIN_GL_SetSwapInterval(_THIS, int interval);
extern int WIN_GL_GetSwapInterval(_THIS);
extern void WIN_GL_SwapWindow(_THIS, SDL_Window * window);
extern void WIN_GL_DeleteContext(_THIS, SDL_GLContext context);

extern int WIN_GL_LoadLibrary(_THIS, const char* path);
extern void WIN_GL_UnloadLibrary(_THIS);
extern void * WIN_GL_GetProcAddress(_THIS, const char *proc);
#ifdef __cplusplus
};
#endif

#endif /* SDL_VIDEO_OPENGL_WGL */

#endif /* _SDL_windowsopengl_h */

/* vi: set ts=4 sw=4 expandtab: */
