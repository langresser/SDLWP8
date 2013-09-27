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

extern "C" {
#include "SDL_main.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
};

#include "SDL_windowsvideo.h"

/* Initialization/Query functions */
static int WIN_VideoInit(_THIS);
static void WIN_VideoQuit(_THIS);

extern void get_screen_size(int& width, int& height);
/* Windows driver bootstrap functions */

static int
WIN_Available(void)
{
    return (1);
}

static void
WIN_DeleteDevice(SDL_VideoDevice * device)
{
    SDL_VideoData *data = (SDL_VideoData *) device->driverdata;
    SDL_free(device->driverdata);
    SDL_free(device);
}

static SDL_VideoDevice *
WIN_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;
    SDL_VideoData *data;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (device) {
        data = (struct SDL_VideoData *) SDL_calloc(1, sizeof(SDL_VideoData));
    } else {
        data = NULL;
    }
    if (!data) {
        SDL_free(device);
        SDL_OutOfMemory();
        return NULL;
    }
    device->driverdata = data;

//     data->userDLL = SDL_LoadObject("USER32.DLL");
//     if (data->userDLL) {
//         data->CloseTouchInputHandle = (BOOL (WINAPI *)( HTOUCHINPUT )) SDL_LoadFunction(data->userDLL, "CloseTouchInputHandle");
//         data->GetTouchInputInfo = (BOOL (WINAPI *)( HTOUCHINPUT, UINT, PTOUCHINPUT, int )) SDL_LoadFunction(data->userDLL, "GetTouchInputInfo");
//         data->RegisterTouchWindow = (BOOL (WINAPI *)( HWND, ULONG )) SDL_LoadFunction(data->userDLL, "RegisterTouchWindow");
//     }

    /* Set the function pointers */
    device->VideoInit = WIN_VideoInit;
    device->VideoQuit = WIN_VideoQuit;
    device->GetDisplayBounds = NULL;
    device->GetDisplayModes = NULL;
    device->SetDisplayMode = NULL;
    device->PumpEvents = WIN_PumpEvents;

#undef CreateWindow
    device->CreateWindow = WIN_CreateWindow;
    device->CreateWindowFrom = NULL;
    device->SetWindowTitle = NULL;
    device->SetWindowIcon = NULL;
    device->SetWindowPosition = NULL;
    device->SetWindowSize = NULL;
    device->ShowWindow = WIN_ShowWindow;
    device->HideWindow = WIN_HideWindow;
    device->RaiseWindow = NULL;
    device->MaximizeWindow = NULL;
    device->MinimizeWindow = NULL;
    device->RestoreWindow = NULL;
    device->SetWindowBordered = NULL;
    device->SetWindowFullscreen = NULL;
    device->SetWindowGammaRamp = NULL;
    device->GetWindowGammaRamp = NULL;
    device->SetWindowGrab = NULL;
    device->DestroyWindow = WIN_DestroyWindow;
    device->GetWindowWMInfo = NULL;
    device->OnWindowEnter = NULL;

#if SDL_VIDEO_OPENGL_WGL
	device->GL_LoadLibrary = WIN_GL_LoadLibrary;
	device->GL_GetProcAddress = WIN_GL_GetProcAddress;
	device->GL_UnloadLibrary = WIN_GL_UnloadLibrary;
    device->GL_CreateContext = WIN_GL_CreateContext;
    device->GL_MakeCurrent = WIN_GL_MakeCurrent;
    device->GL_SetSwapInterval = WIN_GL_SetSwapInterval;
    device->GL_GetSwapInterval = NULL;
    device->GL_SwapWindow = WIN_GL_SwapWindow;
    device->GL_DeleteContext = WIN_GL_DeleteContext;
#endif
    device->StartTextInput = NULL;
    device->StopTextInput = NULL;
    device->SetTextInputRect = NULL;

    device->SetClipboardText = WIN_SetClipboardText;
    device->GetClipboardText = WIN_GetClipboardText;
    device->HasClipboardText = WIN_HasClipboardText;

    device->free = WIN_DeleteDevice;

    return device;
}

VideoBootStrap WINDOWS_bootstrap = {
    "metro", "SDL Windows8 metro video driver", WIN_Available, WIN_CreateDevice
};

int
WIN_VideoInit(_THIS)
{
	SDL_DisplayMode mode;

	mode.format = SDL_PIXELFORMAT_ABGR8888;
	get_screen_size(mode.w, mode.h);
	mode.refresh_rate = 0;
	mode.driverdata = NULL;
	if (SDL_AddBasicVideoDisplay(&mode) < 0) {
		return -1;
	}

	SDL_AddDisplayMode(&_this->displays[0], &mode);

    return 0;
}

void
WIN_VideoQuit(_THIS)
{
}

int SDL_HelperWindowCreate(void)
{
    return 0;
}


/*
 * Destroys the HelperWindow previously created with SDL_HelperWindowCreate.
 */
void SDL_HelperWindowDestroy(void)
{
}


int WIN_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid)
{
	return 0;
}

#endif /* SDL_VIDEO_DRIVER_WINDOWS */

/* vim: set ts=4 sw=4 expandtab: */