/*
   ttyio.h - Zip 3

   Copyright (c) 1990-2005 Info-ZIP.  All rights reserved.

   See the accompanying file LICENSE, version 2005-Feb-10 or later
   (the contents of which are also included in zip.h) for terms of use.
   If, for some reason, all these files are missing, the Info-ZIP license
   also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
   */
/*
   ttyio.h
   */

#ifndef __ttyio_h   /* don't include more than once */
#define __ttyio_h

#    define ATH_BEO_UNX

/* For all other systems, ttyio.c supplies the two functions Echoff() and
 * Echon() for suppressing and (re)enabling console input echo.
 */
#ifndef echoff
#  define echoff(f)  Echoff(__G__ f)
#  define echon()    Echon(__G)
void Echoff OF((__GPRO__ int f));
void Echon OF((__GPRO));
#endif

char *getp OF((__GPRO__ ZCONST char *m, char *p, int n));

#endif /* !__ttyio_h */
