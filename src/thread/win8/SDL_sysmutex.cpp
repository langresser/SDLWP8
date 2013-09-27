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

#if SDL_THREAD_WINDOWS
#include <thread>
#include <mutex>

#include "SDL_mutex.h"

struct SDL_mutex
{
	std::recursive_mutex m;
};

/* Create a mutex */
SDL_mutex *
SDL_CreateMutex(void)
{
    SDL_mutex *mutex;

    /* Allocate mutex memory */
    mutex = new SDL_mutex;
    if (mutex) {
    } else {
        SDL_OutOfMemory();
    }
    return (mutex);
}

/* Free the mutex */
void
SDL_DestroyMutex(SDL_mutex * mutex)
{
    if (mutex) {
        delete (mutex);
    }
}

/* Lock the mutex */
int
SDL_LockMutex(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

	mutex->m.lock();
    return (0);
}

/* TryLock the mutex */
int
SDL_TryLockMutex(SDL_mutex * mutex)
{
    int retval = 0;
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    mutex->m.try_lock();
    return retval;
}

/* Unlock the mutex */
int
SDL_UnlockMutex(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

	mutex->m.unlock();
    return (0);
}

#endif /* SDL_THREAD_WINDOWS */

/* vi: set ts=4 sw=4 expandtab: */
