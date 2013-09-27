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

#include "SDL_windowsvideo.h"
#include "SDL_windowswindow.h"
#include "../../events/SDL_clipboardevents_c.h"


#ifdef UNICODE
#define TEXT_FORMAT  CF_UNICODETEXT
#else
#define TEXT_FORMAT  CF_TEXT
#endif


/* Get any application owned window handle for clipboard association */
static HWND
GetWindowHandle(_THIS)
{
    return NULL;
}

int
WIN_SetClipboardText(_THIS, const char *text)
{
    return 0;
}

char *
WIN_GetClipboardText(_THIS)
{
    return NULL;
}

SDL_bool
WIN_HasClipboardText(_THIS)
{
    return SDL_FALSE;
}

void
WIN_CheckClipboardUpdate(struct SDL_VideoData * data)
{
}

#endif /* SDL_VIDEO_DRIVER_WINDOWS */

/* vi: set ts=4 sw=4 expandtab: */
