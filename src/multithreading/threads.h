//
// Created by xxrot on 09.09.2024.
//

#ifndef C_WEBSERVER_THREADS_H
#define C_WEBSERVER_THREADS_H

/**
 * Header file defining thread-related functions and types for both Windows and Unix-like systems.
 * Provides wrappers for thread creation, joining, mutex operations, sleeping, and exiting.
 */

#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>

    // Define pthread types for Windows
    #define pthread_t HANDLE
    #define pthread_mutex_t CRITICAL_SECTION
    #define pthread_cond_t CONDITION_VARIABLE

    // Create thread wrapper for Windows
    static inline int pthread_create(pthread_t *thread, void *attr, unsigned int (__stdcall *start_routine)(void *), void *arg)
    {
        *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
        return *thread ? 0 : -1;  // Return 0 on success, -1 on failure
    }

    /**
    * Waits for the specified thread to terminate.
    *
    * @param thread The handle to the thread to wait for.
    * @param value_ptr A pointer to a variable to receive the exit status of the thread.
    * @return 0 on success, -1 on failure.
    */
    static inline int pthread_join(pthread_t thread, void **value_ptr)
    {
        DWORD result = WaitForSingleObject(thread, INFINITE);
        if (result == WAIT_FAILED) {
            return -1;  // Indicate error
        }
        CloseHandle(thread);  // Close thread handle after join
        return 0;
    }

    // Mutex initialization wrapper for Windows
    #define pthread_mutex_init(mutex, attr) (InitializeCriticalSection(mutex), 0)
    #define pthread_mutex_lock(mutex) EnterCriticalSection(mutex)
    #define pthread_mutex_unlock(mutex) LeaveCriticalSection(mutex)
    #define pthread_mutex_destroy(mutex) DeleteCriticalSection(mutex)

    // Exit thread wrapper for Windows
    #define pthread_exit(value_ptr) _endthreadex(0)

    // Defines macros for sleeping functions in milliseconds and microseconds.
    #define thread_sleep(seconds) Sleep((seconds) * 1000)
    #define thread_usleep(microseconds) Sleep((microseconds) / 1000)
#else
    #include <pthread.h>  // POSIX threads (pthreads) for Unix-like systems
    #include <unistd.h>   // For sleep and usleep

    #define thread_sleep(seconds) sleep(seconds)
    #define thread_usleep(microseconds) usleep(microseconds)
#endif

#endif //C_WEBSERVER_THREADS_H
