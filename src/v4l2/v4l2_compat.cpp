/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * v4l2_compat.cpp - V4L2 compatibility layer
 */

#include "v4l2_compat_manager.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LIBCAMERA_PUBLIC __attribute__((visibility("default")))

using namespace libcamera;

#define extract_va_arg(type, arg, last)	\
{					\
	va_list ap;			\
	va_start(ap, last);		\
	arg = va_arg(ap, type);		\
	va_end(ap);			\
}

extern "C" {

LIBCAMERA_PUBLIC int open(const char *path, int oflag, ...)
{
	mode_t mode = 0;
	if (oflag & O_CREAT || oflag & O_TMPFILE)
		extract_va_arg(mode_t, mode, oflag);

	return V4L2CompatManager::instance()->openat(AT_FDCWD, path,
						     oflag, mode);
}

/* _FORTIFY_SOURCE redirects open to __open_2 */
LIBCAMERA_PUBLIC extern __typeof(open) __open_2 __attribute__ ((alias("open")));

LIBCAMERA_PUBLIC int openat(int dirfd, const char *path, int oflag, ...)
{
	mode_t mode = 0;
	if (oflag & O_CREAT || oflag & O_TMPFILE)
		extract_va_arg(mode_t, mode, oflag);

	return V4L2CompatManager::instance()->openat(dirfd, path, oflag, mode);
}

LIBCAMERA_PUBLIC extern __typeof(openat) __openat_2 __attribute__ ((alias("openat")));

LIBCAMERA_PUBLIC int dup(int oldfd)
{
	return V4L2CompatManager::instance()->dup(oldfd);
}

LIBCAMERA_PUBLIC int close(int fd)
{
	return V4L2CompatManager::instance()->close(fd);
}

LIBCAMERA_PUBLIC void *mmap(void *addr, size_t length, int prot, int flags,
			    int fd, off_t offset)
{
	return V4L2CompatManager::instance()->mmap(addr, length, prot, flags,
						   fd, offset);
}

LIBCAMERA_PUBLIC int munmap(void *addr, size_t length)
{
	return V4L2CompatManager::instance()->munmap(addr, length);
}

LIBCAMERA_PUBLIC int ioctl(int fd, unsigned long request, ...)
{
	void *arg;
	extract_va_arg(void *, arg, request);

	return V4L2CompatManager::instance()->ioctl(fd, request, arg);
}

}
