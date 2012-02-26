# Overview

This project provides some utilities to trace what files are opened during some
period of time.

I get this idea when I wanted to create a list of source files which are
actually used for building Qemu, so I can use that file list to build tag file
for Vim. (It's common in Qemu that a function is implemented in many different
target specific source files. I want to build a **clean tag file** which
contains only the functions of the target I'm interested in.)

I thought that task would be easy and tried to build a shared library at first.
It worked fine on Linux, but not expected on Mac OS X. The failure on OS X leads
me to try other approaches:

- DTrace on OS X
- SystemTap on Linux

The utilities are useful for me. (At least solves the problem of building a clean
tag file.) But the system instrumentation tools I learned are more valuable.

For more instructions on how to use these scripts to create a ctags/cscope tag
file, refer to `README.mkd` in each subdirectory.

# DTrace

# SystemTap

I've only glimpsed over the [SystemTap Tutorial](http://sourceware.org/systemtap/tutorial.pdf)
and created the script `systemtap/traceopen.stp`.

To use SystemTap, you need to first install SystemTap. On Debian 6:

    aptitude install systemtap linux-headers-2.6.32-5-amd64 linux-headers-2.6.32-5-amd6-dbg

**Make sure the running kernel matches the debug info installed.** Otherwise,
SystemTap will report build-id mismatch error.

Add the user to `stapdev` and `stapusr` group.

Run the trace as follows:

    stap traceopen.stp --vp 00001

The `--vp 00001` makes SystemTap output debug message at the 5th pass, which is
when the script actually runs. After that, start your program to trace its open
system call.

# Shared library

This works on Linux, but I failed to use this approach on OS X.

## Why not working on OS X?

From `man dyld`, about the `DYLD_INSERT_LIBRARIES` environment variable:

> This lets you test new modules of existing dynamic shared libraries that are
> used in flat-namespace images by loading a temporary dynamic shared library
> with just the new modules.  Note  that this has no effect on images built a
> two-level namespace images using a dynamic shared library unless
> DYLD\_FORCE\_FLAT\_NAMESPACE is also used.

By setting the `DYLD_PRINT_LIBRARIES` environment variable, `dyld` shows that it
indeed tries to load the specified library, but failed for some applications.
e.g. MacVim's Vim binary.

Another problem is that even if the library is successfully loaded, it only
works **some times**. I haven't figured out in which case it will succeed.

I though this approach would also work on OS X well, but seems not the case
actually. And I remembered DTrace. So instead of fighting against problem, I
moved to the DTrace approach, which would be more easy and possibly more
efficient.

Notes on getting non-existing file
==================================

The initial implementation will get non existing files outputed. It's because I
didn't check the return value of `open`. Maybe the traced program just opens an
non-existing file.

If you want only existing file to be outputed, find ways to check the return
value and print out only when `open` succeeded.

