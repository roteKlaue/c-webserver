//
// Created by xxrot on 09.09.2024.
//

#ifndef C_WEBSERVER_THREADS_H
#define C_WEBSERVER_THREADS_H

#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>

    // Define pthread types for Windows
    #define pthread_t HANDLE
    #define pthread_mutex_t CRITICAL_SECTION

    // Create thread wrapper for Windows
    #define pthread_create(thread, attr, start_routine, arg) \
        *thread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *))start_routine, arg, 0, NULL)

    // Join thread wrapper for Windows
    #define pthread_join(thread, value_ptr) \
        do { \
            WaitForSingleObject(thread, INFINITE); \
            CloseHandle(thread);  /* Close thread handle after join */ \
        } while(0)

    // Mutex initialization wrapper for Windows
    #define pthread_mutex_init(mutex, attr) InitializeCriticalSection(mutex)
    #define pthread_mutex_lock(mutex) EnterCriticalSection(mutex)
    #define pthread_mutex_unlock(mutex) LeaveCriticalSection(mutex)
    #define pthread_mutex_destroy(mutex) DeleteCriticalSection(mutex)

    // Exit thread wrapper for Windows
    #define pthread_exit(value_ptr) _endthreadex(0)
#else
    #include <pthread.h>  // POSIX threads (pthreads) for Unix-like systems
#endif

#endif //C_WEBSERVER_THREADS_H
