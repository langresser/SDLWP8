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

#include "SDL_thread.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class semaphore {
public:
	semaphore(int value=0): count(value), wakeups(0) {}

	void wait(){
		std::unique_lock<std::mutex> lock(mutex);
		if (--count<0) { // count is not enough ?
			condition.wait(lock, [&]()->bool{ return wakeups>0;}); // suspend and wait ...
			--wakeups;  // ok, me wakeup !
		}
	}
	void signal(){
		std::lock_guard<std::mutex> lock(mutex);
		if(++count<=0) { // have some thread suspended ?
			++wakeups;
			condition.notify_one(); // notify one !
		}
	}

private:
	int count;
	int wakeups;
	std::mutex mutex;
	std::condition_variable condition;
};

struct SDL_semaphore
{
	semaphore sem;
};


/* Create a semaphore */
SDL_sem *
SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_sem *sem;

    /* Allocate sem memory */
    sem = new SDL_sem;
    return (sem);
}

/* Free the semaphore */
void
SDL_DestroySemaphore(SDL_sem * sem)
{
    if (sem) {
        delete (sem);
    }
}

int
SDL_SemWaitTimeout(SDL_sem * sem, Uint32 timeout)
{
    int retval = 1;
    if (!sem) {
        return SDL_SetError("Passed a NULL sem");
    }

    sem->sem.wait();
    return retval;
}

int
SDL_SemTryWait(SDL_sem * sem)
{
    return 1;
}

int
SDL_SemWait(SDL_sem * sem)
{
    return SDL_SemWaitTimeout(sem, SDL_MUTEX_MAXWAIT);
}

/* Returns the current count of the semaphore */
Uint32
SDL_SemValue(SDL_sem * sem)
{
    return 0;
}

int
SDL_SemPost(SDL_sem * sem)
{
    if (!sem) {
        return SDL_SetError("Passed a NULL sem");
    }
   
	 sem->sem.signal();
    return 0;
}

#endif /* SDL_THREAD_WINDOWS */

/* vi: set ts=4 sw=4 expandtab: */
