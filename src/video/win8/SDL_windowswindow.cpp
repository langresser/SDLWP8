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
#include "../SDL_sysvideo.h"

extern "C" {
#include "../../events/SDL_keyboard_c.h"
#include "../../events/SDL_mouse_c.h"
};

#include "SDL_windowsvideo.h"
#include "SDL_windowswindow.h"

/* This is included after SDL_windowsvideo.h, which includes windows.h */
#include "SDL_syswm.h"

// 当前唯一窗口
SDL_Window* g_currentWindow = NULL;
extern void get_screen_size(int& width, int& height);

int
WIN_CreateWindow(_THIS, SDL_Window * window)
{
	SDL_WindowData *data;

	if (g_currentWindow) {
		return SDL_SetError("Android only supports one window");
	}

	/* Adjust the window data to match the screen */
	window->x = 0;
	window->y = 0;
	get_screen_size(window->w, window->h);

	window->flags &= ~SDL_WINDOW_RESIZABLE;     /* window is NEVER resizeable */
	window->flags |= SDL_WINDOW_FULLSCREEN;     /* window is always fullscreen */
	window->flags &= ~SDL_WINDOW_HIDDEN;
	window->flags |= SDL_WINDOW_SHOWN;          /* only one window on Android */
	window->flags |= SDL_WINDOW_INPUT_FOCUS;    /* always has input focus */

	/* One window, it always has focus */
	SDL_SetMouseFocus(window);
	SDL_SetKeyboardFocus(window);

	data = (SDL_WindowData *) SDL_calloc(1, sizeof(*data));
	if (!data) {
		return SDL_OutOfMemory();
	}

	window->driverdata = data;
	g_currentWindow = window;

    return 0;
}

void
WIN_ShowWindow(_THIS, SDL_Window * window)
{
}

void
WIN_HideWindow(_THIS, SDL_Window * window)
{
}

void
WIN_DestroyWindow(_THIS, SDL_Window * window)
{
	SDL_WindowData *data;

	if (window == g_currentWindow) {
		g_currentWindow = NULL;
		if(window->driverdata) {
			data = (SDL_WindowData *) window->driverdata;
			SDL_free(window->driverdata);
			window->driverdata = NULL;
		}
	}
}

#endif /* SDL_VIDEO_DRIVER_WINDOWS */

/* vi: set ts=4 sw=4 expandtab: */
