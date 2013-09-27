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

#ifndef _SDL_windowswindow_h
#define _SDL_windowswindow_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    SDL_Window *window;
    SDL_bool created;
    WPARAM mouse_button_flags;
    struct SDL_VideoData *videodata;
} SDL_WindowData;

extern int WIN_CreateWindow(_THIS, SDL_Window * window);
extern void WIN_ShowWindow(_THIS, SDL_Window * window);
extern void WIN_HideWindow(_THIS, SDL_Window * window);
extern void WIN_DestroyWindow(_THIS, SDL_Window * window);

#ifdef __cplusplus
};
#endif

#endif /* _SDL_windowswindow_h */

/* vi: set ts=4 sw=4 expandtab: */
