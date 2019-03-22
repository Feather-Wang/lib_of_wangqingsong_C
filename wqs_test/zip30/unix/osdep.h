/*
  unix/osdep.h - Zip 3

  Copyright (c) 1990-2005 Info-ZIP.  All rights reserved.

  See the accompanying file LICENSE, version 2005-Feb-10 or later
  (the contents of which are also included in zip.h) for terms of use.
  If, for some reason, both of these files are missing, the Info-ZIP license
  also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
*/

  /* 64-bit Large File Support */

  /* The following Large File Summit (LFS) defines turn on large file support on
     Linux (probably 2.4 or later kernel) and many other unixen */

# define _LARGEFILE_SOURCE      /* some OSes need this for fseeko */
# define _LARGEFILE64_SOURCE
# define _FILE_OFFSET_BITS 64   /* select default interface as 64 bit */
# define _LARGE_FILES           /* some OSes need this for 64-bit off_t */

#include <sys/types.h>
#include <sys/stat.h>

# define ZOFF_T_FORMAT_SIZE_PREFIX "ll"

  typedef off_t zoff_t;
  typedef unsigned long long uzoff_t;
  typedef struct stat z_stat;


/* Automatically set ZIP64_SUPPORT if LFS */

#     define ZIP64_SUPPORT

/* Enable the "UT" extra field (time info) */
#  define USE_EF_UT_TIME
