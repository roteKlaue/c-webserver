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
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <process.h>

    // Define pthread types for Windows
    typedef HANDLE pthread_t;
    typedef CRITICAL_SECTION pthread_mutex_t;
    typedef CONDITION_VARIABLE pthread_cond_t;

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

    static inline int pthread_cond_init(pthread_cond_t *cond, const void *attr)
    {
        InitializeConditionVariable(cond);
        return 0;
    }

    static inline int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
    {
        return SleepConditionVariableCS(cond, mutex, INFINITE) ? 0 : -1;
    }

    static inline int pthread_cond_signal(pthread_cond_t *cond)
    {
        WakeConditionVariable(cond);
        return 0;
    }

    static inline int pthread_cond_broadcast(pthread_cond_t *cond)
    {
        WakeAllConditionVariable(cond);
        return 0;
    }

    static inline int pthread_cond_destroy(pthread_cond_t *cond)
    {
        // No specific destroy function for condition variables in Windows
        // This function is included for compatibility with the pthreads API.
        return 0;
    }

    // Mutex initialization wrapper for Windows
    #define pthread_mutex_init(mutex, attr) (InitializeCriticalSection(mutex), 0)
    #define pthread_mutex_lock(mutex) EnterCriticalSection(mutex)
    #define pthread_mutex_unlock(mutex) LeaveCriticalSection(mutex)
    #define pthread_mutex_destroy(mutex) DeleteCriticalSection(mutex)

    // Exit thread wrapper for Windows
    #define pthread_exit(value_ptr) _endthreadex(0);\
        return (unsigned int) 0;

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
