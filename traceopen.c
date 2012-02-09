#include <sys/fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef __linux__
#define LIBC "libc.so.6"
#elif defined(__MACH__)
#define LIBC "libSystem.B.dylib"
#else
#error "Platform not supported"
#endif

int (*realopen)(const char *pathname, int flags, ...);

int open(const char *pathname, int flags, ...) {

    void *handle;    
    char *error;

    if (!realopen) {
        handle = dlopen(LIBC, RTLD_LAZY);
        if ((error = dlerror())) {
            puts(error);
            exit(1);
        }

        realopen = dlsym(handle, "open");
        if ((error = dlerror())) {
            puts(error);
            exit(1);
        }
    }

    puts(pathname);

    /* Part of the glibc implementation uses the following check. But actually,
     * passing O_CREAT without specifying mode will not cause error in both
     * glibc  */
    if (flags & O_CREAT) {
        /* If mode is not passed, the following code may cause error. */
        mode_t mode;
        va_list ap;
        va_start(ap, flags);
#ifdef __MACH__
        /* Compiler will promote mode_t to int when passed in ... */
        mode = (mode_t) va_arg(ap, int);
#else
        mode = va_arg(ap, mode_t);
#endif
        va_end(ap);
        return realopen(pathname, flags, mode);
    } else {
        return realopen(pathname, flags);
    }

}

