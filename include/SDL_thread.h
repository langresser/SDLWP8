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

#ifndef _SDL_thread_h
#define _SDL_thread_h

/**
 *  \file SDL_thread.h
 *
 *  Header for the SDL thread management routines.
 */

#include "SDL_stdinc.h"
#include "SDL_error.h"

/* Thread synchronization primitives */
#include "SDL_atomic.h"
#include "SDL_mutex.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* The SDL thread structure, defined in SDL_thread.c */
struct SDL_Thread;
typedef struct SDL_Thread SDL_Thread;

/* The SDL thread ID */
typedef unsigned long SDL_threadID;

/* Thread local storage ID, 0 is the invalid ID */
typedef unsigned int SDL_TLSID;

/**
 *  The SDL thread priority.
 *
 *  \note On many systems you require special privileges to set high priority.
 */
typedef enum {
    SDL_THREAD_PRIORITY_LOW,
    SDL_THREAD_PRIORITY_NORMAL,
    SDL_THREAD_PRIORITY_HIGH
} SDL_ThreadPriority;

/**
 *  The function passed to SDL_CreateThread().
 *  It is passed a void* user context parameter and returns an int.
 */
typedef int (SDLCALL * SDL_ThreadFunction) (void *data);
extern DECLSPEC SDL_Thread *SDLCALL
	SDL_CreateThread(SDL_ThreadFunction fn, const char *name, void *data);

/**
 * Get the thread name, as it was specified in SDL_CreateThread().
 *  This function returns a pointer to a UTF-8 string that names the
 *  specified thread, or NULL if it doesn't have a name. This is internal
 *  memory, not to be free()'d by the caller, and remains valid until the
 *  specified thread is cleaned up by SDL_WaitThread().
 */
extern DECLSPEC const char *SDLCALL SDL_GetThreadName(SDL_Thread *thread);

/**
 *  Get the thread identifier for the current thread.
 */
extern DECLSPEC SDL_threadID SDLCALL SDL_ThreadID(void);

/**
 *  Get the thread identifier for the specified thread.
 *
 *  Equivalent to SDL_ThreadID() if the specified thread is NULL.
 */
extern DECLSPEC SDL_threadID SDLCALL SDL_GetThreadID(SDL_Thread * thread);

/**
 *  Set the priority for the current thread
 */
extern DECLSPEC int SDLCALL SDL_SetThreadPriority(SDL_ThreadPriority priority);

/**
 *  Wait for a thread to finish.
 *
 *  The return code for the thread function is placed in the area
 *  pointed to by \c status, if \c status is not NULL.
 */
extern DECLSPEC void SDLCALL SDL_WaitThread(SDL_Thread * thread, int *status);

/**
 *  \brief Create an identifier that is globally visible to all threads but refers to data that is thread-specific.
 *
 *  \return The newly created thread local storage identifier, or 0 on error
 *
 *  \code
 *  static SDL_SpinLock tls_lock;
 *  static SDL_TLSID thread_local_storage;
 * 
 *  void SetMyThreadData(void *value)
 *  {
 *      if (!thread_local_storage) {
 *          SDL_AtomicLock(&tls_lock);
 *          if (!thread_local_storage) {
 *              thread_local_storage = SDL_TLSCreate();
 *          }
 *          SDL_AtomicUnLock(&tls_lock);
 *      }
 *      SDL_TLSSet(thread_local_storage, value);
 *  }
 *  
 *  void *GetMyThreadData(void)
 *  {
 *      return SDL_TLSGet(thread_local_storage);
 *  }
 *  \endcode
 *
 *  \sa SDL_TLSGet()
 *  \sa SDL_TLSSet()
 */
extern DECLSPEC SDL_TLSID SDLCALL SDL_TLSCreate(void);

/**
 *  \brief Get the value associated with a thread local storage ID for the current thread.
 *
 *  \param id The thread local storage ID
 *
 *  \return The value associated with the ID for the current thread, or NULL if no value has been set.
 *
 *  \sa SDL_TLSCreate()
 *  \sa SDL_TLSSet()
 */
extern DECLSPEC void * SDLCALL SDL_TLSGet(SDL_TLSID id);

/**
 *  \brief Set the value associated with a thread local storage ID for the current thread.
 *
 *  \param id The thread local storage ID
 *  \param value The value to associate with the ID for the current thread
 *  \param destructor A function called when the thread exits, to free the value.
 *
 *  \return 0 on success, -1 on error
 *
 *  \sa SDL_TLSCreate()
 *  \sa SDL_TLSGet()
 */
extern DECLSPEC int SDLCALL SDL_TLSSet(SDL_TLSID id, const void *value, void (*destructor)(void*));


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_thread_h */

/* vi: set ts=4 sw=4 expandtab: */
