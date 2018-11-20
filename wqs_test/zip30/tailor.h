/*
  tailor.h - Zip 3

  Copyright (c) 1990-2008 Info-ZIP.  All rights reserved.

  See the accompanying file LICENSE, version 2007-Mar-4 or later
  (the contents of which are also included in zip.h) for terms of use.
  If, for some reason, all these files are missing, the Info-ZIP license
  also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
*/

/* Some compiler distributions for Win32/i386 systems try to emulate
 * a Unix (POSIX-compatible) environment.
 */
#include "unix/osdep.h"

#  define OF(a) a
#  define OFT(a) a
#    define ZCONST const

/*
 * Some compiler environments may require additional attributes attached
 * to declarations of runtime libary functions (e.g. to prepare for
 * linking against a "shared dll" version of the RTL).  Here, we provide
 * the "empty" default for these attributes.
 */
#  define IZ_IMP

/*
 * case mapping functions. case_map is used to ignore case in comparisons,
 * to_up is used to force upper case even on Unix (for dosify option).
 */
#  define case_map(c) (c)
#  define to_up(c)    ((c) >= 'a' && (c) <= 'z' ? (c)-'a'+'A' : (c))

/* Define void, zvoid, and extent (size_t) */
#include <stdio.h>

#  include <stddef.h>

#  include <stdlib.h>

#  include <unistd.h> /* usually defines _POSIX_VERSION */

#  include <fcntl.h>

#  include <string.h>

   typedef void zvoid;

#  define FALSE 0
#  define TRUE 1

   typedef size_t extent;

/* DBCS support for Info-ZIP's zip  (mainly for japanese (-: )
 * by Yoshioka Tsuneo (QWF00133@nifty.ne.jp,tsuneo-y@is.aist-nara.ac.jp)
 * This code is public domain!   Date: 1998/12/20
 */

/* 2007-07-29 SMS.
 * Include <locale.h> here if it will be needed later for Unicode.
 * Otherwise, SETLOCALE may be defined here, and then defined again
 * (differently) when <locale.h> is read later.
 */
#   include <locale.h>
# include <wchar.h>
# include <wctype.h>

#   define CLEN(ptr) 1
#   define PREINCSTR(ptr) (++(ptr))
#   define POSTINCSTR(ptr) ((ptr)++)
#   define lastchar(ptr) ((*(ptr)=='\0') ? '\0' : ptr[strlen(ptr)-1])
#   define MBSCHR(str, c) strchr(str, c)
#   define MBSRCHR(str, c) strrchr(str, c)
#      define SETLOCALE(category, locale)
#define INCSTR(ptr) PREINCSTR(ptr)


/* System independent replacement for "struct utimbuf", which is missing
 * in many older OS environments.
 */
typedef struct ztimbuf {
    time_t actime;              /* new access time */
    time_t modtime;             /* new modification time */
} ztimbuf;

/* This macro round a time_t value to the OS specific resolution */
#  define ROUNDED_TIME(time)   (time)

  typedef ulg                z_uint4;
# define Z_UINT4_DEFINED

#  define FOPR "r"
#  define FOPM "r+"
#  define FOPW "w"

#  define FOPW_TMP FOPW

/* Open the old zip file in exclusive mode if possible (to avoid adding
 * zip file to itself).
 */
#  define FOPR_EX FOPR


/* MSDOS file or directory attributes */
#define MSDOS_HIDDEN_ATTR 0x02
#define MSDOS_DIR_ATTR 0x10

#  define CBSZ 16384
#  define ZBSZ 16384

#  define SBSZ CBSZ     /* copy buf size for STORED entries, see zipup() */

#      define huge
#      define far
#      define near
#  define nearmalloc malloc
#  define nearfree free
#  define farmalloc malloc
#  define farfree free

#  define Far far

/* LARGE_FILE_SUPPORT
 *
 * Types are in osdep.h for each port
 *
 * LARGE_FILE_SUPPORT and ZIP64_SUPPORT are automatically
 * set in osdep.h (for some ports) based on the port and compiler.
 *
 * Function prototypes are below as OF is defined earlier in this file
 * but after osdep.h is included.  In the future ANSI prototype
 * support may be required and the OF define may then go away allowing
 * the function defines to be in the port osdep.h.
 *
 * E. Gordon 9/21/2003
 * Updated 7/24/04 EG
 */
  /* 64-bit Large File Support */

  /* Arguments for all functions are assumed to match the actual
     arguments of the various port calls.  As such only the
     function names are mapped below. */

  /* Assume 64-bit file environment is defined.  The below should all
     be set to their 64-bit versions automatically.  Neat.  7/20/2004 EG */

    /* 64-bit stat functions */
#   define zstat stat
#   define zfstat fstat
#   define zlstat lstat

     /* 64-bit fseeko */
#   define zfseeko fseeko

     /* 64-bit ftello */
#   define zftello ftello

    /* 64-bit fopen */
#   define zfopen fopen
#   define zfdopen fdopen

#  define SSTAT      zstat
#    define SSTATW   zwstat
#  define LSTAT      zlstat
#  define LSSTAT(n, s)  (linkput ? zlstat((n), (s)) : SSTAT((n), (s)))

/*---------------------------------------------------------------------*/


/* 2004-12-01 SMS.
 * Added fancy zofft() macros, et c.
 */

/* Default fzofft() format selection.
 * Modified 2004-12-27 EG
 */

# define FZOFFT_FMT      ZOFF_T_FORMAT_SIZE_PREFIX /* printf for zoff_t values */

#   define FZOFFT_HEX_WID_VALUE     "16"  /* width of 64-bit hex values */

#define FZOFFT_HEX_WID ((char *) -1)
#define FZOFFT_HEX_DOT_WID ((char *) -2)

/* The following default definition of the second input for the crypthead()
 * random seed computation can be used on most systems (all those that
 * supply a UNIX compatible getpid() function).
 */
#    define ZCR_SEED2     (unsigned) getpid()   /* use PID as seed pattern */

#define NUM_HOSTS 31
/* Number of operating systems. Should be updated when new ports are made */

#  define OS_CODE  0x300  /* assume Unix */

#  define EXIT  exit
#  define RETURN return

#  define ZIPERR ziperr

   /* Any system without a special calloc function */
#  define zcalloc(items,size) \
          (zvoid far *)calloc((unsigned)(items), (unsigned)(size))
#  define zcfree    free
/* end of tailor.h */
