#define _GNU_SOURCE

#include <sys/fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __linux__
#define LIBC "libc.so.6"
#elif defined(__MACH__)
#define LIBC "libSystem.B.dylib"
#else
#error "Platform not supported"
#endif

int (*realopen)(const char *pathname, int flags, ...);

/* The trace output directory should be specified as absolute path. */
#define ENVVAR_TRACEDIR "TRACEDIR"
/* A new file will be created in the trace dir during the first call to open. */
FILE *outputfile;
static int fileopend;

static void resolve_realopen() {
    void *handle;    
    char *error;

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

#define FILETMPLATE "traceopenXXXXXX"
static void open_outputfile() {
    int r;
    const char *envdir = getenv(ENVVAR_TRACEDIR);
    char *filepath = NULL;

    fileopend = 1;

    if (envdir) {
        r = asprintf(&filepath, "%s/" FILETMPLATE, envdir);
    } else {
        r = asprintf(&filepath, "/tmp/" FILETMPLATE);
    }
    if (r == -1) {
        perror("asprintf failed");
        return;
    }

    r = mkstemp(filepath);
    free(filepath);
    filepath = NULL;
    if (r == -1) {
        perror("mkstemp failed failed");
        return;
    }

    outputfile = fdopen(r, "w");
    if (outputfile == NULL) {
        perror("fdopen failed");
        return;
    }
}

int open(const char *pathname, int flags, ...) {
    if (!realopen) {
        resolve_realopen();
    }
    if (!fileopend) {
        open_outputfile();
    }

    /* Do nothing if file not open or write has error. Just call original open. */
    if (outputfile && pathname) {
        const char *abspath = (pathname[0] == '/') ? pathname : realpath(pathname, NULL);
        if (abspath) {
            fprintf(outputfile, "%s\n", abspath);
            /*fprintf(stdout, "%s\n", abspath);*/
        }
        if (abspath != pathname) {
            free((void *)abspath);
        }
    }

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

