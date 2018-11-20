/*
   zipup.c - Zip 3

   Copyright (c) 1990-2008 Info-ZIP.  All rights reserved.

   See the accompanying file LICENSE, version 2007-Mar-4 or later
   (the contents of which are also included in zip.h) for terms of use.
   If, for some reason, all these files are missing, the Info-ZIP license
   also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
   */
/*
 *  zipup.c by Mark Adler and Jean-loup Gailly.
 */
#define __ZIPUP_C

/* Found that for at least unix port zip.h has to be first or ctype.h will
   define off_t and when using 64-bit file environment off_t in other files
   is 8 bytes while off_t here is 4 bytes, and this makes the zlist struct
   different sizes and needless to say leads to segmentation faults.  Putting
   zip.h first seems to fix this.  8/14/04 EG */
#include "zip.h"
#include <ctype.h>
#include <errno.h>

#ifndef UTIL            /* This module contains no code for Zip Utilities */

#include "revision.h"
#include "crc32.h"
#include "crypt.h"

/* Use the raw functions for MSDOS and Unix to save on buffer space.
   They're not used for VMS since it doesn't work (raw is weird on VMS).
   */

#  include "unix/zipup.h"

/* Local functions */
local int suffixes OF((char *, char *));
local unsigned file_read OF((char *buf, unsigned size));

/* zip64 support 08/29/2003 R.Nausedat */
local zoff_t filecompress OF((struct zlist far *z_entry, int *cmpr_method));

/* Deflate "internal" global data (currently not in zip.h) */
extern ulg window_size;       /* size of said window */

unsigned (*read_buf) OF((char *buf, unsigned size)) = file_read;
/* Current input function. Set to mem_read for in-memory compression */


/* Local data */
local ulg crc;                  /* crc on uncompressed file data */
local ftype ifile;              /* file to compress */
local char file_outbuf[1024]; /* output buffer for compression to file */

local zoff_t isize;         /* input file size. global only for debugging */
/* If file_read detects binary it sets this flag - 12/16/04 EG */
local int file_binary = 0;        /* first buf */
local int file_binary_final = 0;  /* for bzip2 for entire file.  assume text until find binary */


/* moved check to function 3/14/05 EG */
int is_seekable(y)
    FILE *y;
{
    zoff_t pos;

    pos = zftello(y);
    if (zfseeko(y, pos, SEEK_SET)) {
        return 0;
    }

    return 1;
}


int percent(n, m)
    uzoff_t n;
    uzoff_t m;                    /* n is the original size, m is the new size */
    /* Return the percentage compression from n to m using only integer
       operations */
{
    zoff_t p;

    /* 2004-12-01 SMS.
     * Changed to do big-n test only for small zoff_t.
     * Changed big-n arithmetic to accomodate apparently negative values
     * when a small zoff_t value exceeds 2G.
     * Increased the reduction divisor from 256 to 512 to avoid the sign bit
     * in a reduced intermediate, allowing signed arithmetic for the final
     * result (which is no longer artificially limited to non-negative
     * values).
     * Note that right shifts must be on unsigned values to avoid undesired
     * sign extension.
     */

    /* Handle n = 0 case and account for int maybe being 16-bit.  12/28/2004 EG
    */

#define PC_MAX_SAFE 0x007fffffL    /* 9 clear bits at high end. */
#define PC_MAX_RND  0xffffff00L    /* 8 clear bits at low end. */

    if (sizeof(uzoff_t) < 8)          /* Don't fiddle with big zoff_t. */
    {
        if ((ulg)n > PC_MAX_SAFE)       /* Reduce large values.  (n > m) */
        {
            if ((ulg)n < PC_MAX_RND)      /* Divide n by 512 with rounding, */
                n = ((ulg)n + 0x100) >> 9;  /* if boost won't overflow. */
            else                          /* Otherwise, use max value. */
                n = PC_MAX_SAFE;

            if ((ulg)m < PC_MAX_RND)      /* Divide m by 512 with rounding, */
                m = ((ulg)m + 0x100) >> 9;  /* if boost won't overflow. */
            else                          /* Otherwise, use max value. */
                m = PC_MAX_SAFE;
        }
    }
    if (n != 0)
        p = ((200 * ((zoff_t)n - (zoff_t)m) / (zoff_t)n) + 1) / 2;
    else
        p = 0;
    return (int)p;  /* Return (rounded) % reduction. */
}


local int suffixes(a, s)
    char *a;                      /* name to check suffix of */
    char *s;                      /* list of suffixes separated by : or ; */
    /* Return true if a ends in any of the suffixes in the list s. */
{
    int m;                        /* true if suffix matches so far */
    char *p;                      /* pointer into special */
    char *q;                      /* pointer into name a */

    m = 1;
    q = a + strlen(a) - 1;
    for (p = s + strlen(s) - 1; p >= s; p--)
        if (*p == ':' || *p == ';')
        {
            if (m)
                return 1;
            else
            {
                m = 1;
                q = a + strlen(a) - 1;
            }
        }
        else
        {
            m = m && q >= a && case_map(*p) == case_map(*q);
            q--;
        }
    return m;
}



/* Note: a zip "entry" includes a local header (which includes the file
   name), an encryption header if encrypting, the compressed data
   and possibly an extended local header. */

int zipup(zipfileinfo)
    struct zlist far *zipfileinfo;    /* zip entry to compress */
    /* Compress the file z->name into the zip entry described by *z and write
       it to the file *y. Encrypt if requested.  Return an error code in the
       ZE_ class.  Also, update tempzn by the number of bytes written. */
    /* y is now global */
{
    iztimes f_utim;       /* UNIX GMT timestamps, filled by filetime() */
    ulg tim;              /* time returned by filetime() */
    ulg a = 0L;           /* attributes returned by filetime() */
    char *b;              /* malloc'ed file buffer */
    extent k = 0;         /* result of zread */
    int l = 0;            /* true if this file is a symbolic link */
    int m = method;                /* method for this entry */

    zoff_t o = 0, p;      /* offsets in zip file */
    zoff_t q = (zoff_t) -3; /* size returned by filetime */
    uzoff_t uq;           /* unsigned q */
    zoff_t s = 0;         /* size of compressed data */

    int r;                /* temporary variable */
    int isdir;            /* set for a directory name */
    int set_type = 0;     /* set if file type (ascii/binary) unknown */
    zoff_t last_o;        /* used to detect wrap around */

    ush tempext = 0;      /* temp copies of extra fields */
    ush tempcext = 0;
    char *tempextra = NULL;
    char *tempcextra = NULL;

    //printf("\n[Debug] [%d] zipup() start\n", __LINE__);

    zipfileinfo->nam = strlen(zipfileinfo->iname);
    isdir = zipfileinfo->iname[zipfileinfo->nam-1] == (char)0x2f; /* ascii[(unsigned)('/')] */

    file_binary = -1;      /* not set, set after first read */
    file_binary_final = 0; /* not set, set after first read */

    tim = filetime(zipfileinfo->name, &a, &q, &f_utim);

    /* reset dot_count for each file */
    if (!display_globaldots)
    {
        //printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        dot_count = -1;
    }

    /* display uncompressed size */
    uq = ((uzoff_t) q > (uzoff_t) -3) ? 0 : (uzoff_t) q;

    /* initial zipfileinfo->len so if error later have something */
    zipfileinfo->len = uq;

    zipfileinfo->att = (ush)UNKNOWN; /* will be changed later */
    zipfileinfo->atx = 0; /* may be changed by set_extra_field() */

    zipfileinfo->extra = zipfileinfo->cextra = NULL;
    zipfileinfo->ext = zipfileinfo->cext = 0;

    window_size = 0L;

    /* create extra field and change zipfileinfo->att and zipfileinfo->atx if desired */
    set_extra_field(zipfileinfo, &f_utim);

    l = issymlnk(a);

    if ((ifile = zopen(zipfileinfo->name, fhow)) == fbad)
        return ZE_OPEN;

    zipfileinfo->tim = tim;

    if (q == 0)
        m = STORE;
    if (m == BEST)
        m = DEFLATE;

    zipfileinfo->vem = (ush)(dosify ? 20 : OS_CODE + Z_MAJORVER * 10 + Z_MINORVER);

    zipfileinfo->ver = (ush)(m == STORE ? 10 : 20); /* Need PKUNZIP 2.0 except for store */
    zipfileinfo->crc = 0;  /* to be updated later */
    /* Assume first that we will need an extended local header: */
    if (isdir)
    {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        /* If dir then q = 0 and extended header not needed */
        zipfileinfo->flg = 0;
    }
    else
    {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        zipfileinfo->flg = 8;  /* to be updated later */
    }
#if CRYPT
    if (!isdir && key != NULL) {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        zipfileinfo->flg |= 1;
        /* Since we do not yet know the crc here, we pretend that the crc
         * is the modification time:
         */
        zipfileinfo->crc = zipfileinfo->tim << 16;
        printf("[Debug] zipup(),[%s][%d], tim=[%x], crc=[%x], flg=[%x]\n", __FILE__, __LINE__, zipfileinfo->tim, zipfileinfo->crc, zipfileinfo->flg);
        /* More than pretend.  File is encrypted using crypt header with that. */
    }
#endif /* CRYPT */
    zipfileinfo->lflg = zipfileinfo->flg;
    zipfileinfo->how = (ush)m;                              /* may be changed later  */
    zipfileinfo->siz = (zoff_t)(m == STORE && q >= 0 ? q : 0); /* will be changed later */
    zipfileinfo->len = (zoff_t)(q != -1L ? q : 0);          /* may be changed later  */
    if (zipfileinfo->att == (ush)UNKNOWN) {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        zipfileinfo->att = BINARY;                    /* set sensible value in header */
        set_type = 1;
    }
    /* Attributes from filetime(), flag bits from set_extra_field(): */
    zipfileinfo->atx = dosify ? a & 0xff : a | (zipfileinfo->atx & 0x0000ff00);

    r = putlocal(zipfileinfo, PUTLOCAL_WRITE);

    /* now get split information set by bfwrite() */
    zipfileinfo->off = current_local_offset;

    /* disk local header was written to */
    zipfileinfo->dsk = current_local_disk;

    tempzn += 4 + LOCHEAD + zipfileinfo->nam + zipfileinfo->ext;

#if CRYPT
    if (!isdir && key != NULL) {
        printf("[Debug] zipup(),[%s][%d], key=[%s]\n", __FILE__, __LINE__, key);
        crypthead(key, zipfileinfo->crc);
        zipfileinfo->siz += RAND_HEAD_LEN;  /* to be updated later */
        tempzn += RAND_HEAD_LEN;
        printf("[Debug] zipup(),[%s][%d], tim=[%x], crc=[%x], flg=[%x]\n", __FILE__, __LINE__, zipfileinfo->tim, zipfileinfo->crc, zipfileinfo->flg);
    }
#endif /* CRYPT */
    ferror(y);

    last_o = o;
    o = zftello(y); /* for debugging only, ftell can fail on pipes */
    if (ferror(y))
        clearerr(y);


    /* Write stored or deflated file to zip file */
    isize = 0L;
    crc = CRCVAL_INITIAL;

    if (isdir) {
        /* nothing to write */
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
    }
    else if (m != STORE) {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        if (set_type) zipfileinfo->att = (ush)UNKNOWN;
        /* ... is finally set in file compression routine */
        {
            /*压缩文件，在压缩的过程中，获取文件原本大小，并返回压缩后的大小*/
            s = filecompress(zipfileinfo, &m);
        }
    }
    if (ifile != fbad)
    {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        zclose(ifile);
    }

    tempzn += s;
    p = tempzn; /* save for future fseek() */

    if (isdir)
    {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
    }
    else
    {
        printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        /* Try to rewrite the local header with correct information */
        zipfileinfo->crc = crc;
        zipfileinfo->siz = s;
#if CRYPT
        if (!isdir && key != NULL)
        {
            printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
            zipfileinfo->siz += RAND_HEAD_LEN;
        }
#endif /* CRYPT */
        zipfileinfo->len = isize;
        /* if can seek back to local header */
        if (use_descriptors || zfseeko(y, zipfileinfo->off, SEEK_SET))
        {
            printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
        } 
        else 
        {
            printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
            zipfileinfo->how = (ush)m;
            switch (m)
            {
                case STORE:
                case DEFLATE:
                    printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
                    zipfileinfo->ver = 20; break;
            }

            /* deflate may have set compression level bit markers in zipfileinfo->flg,
               and we can't think of any reason central and local flags should
               be different. */
            zipfileinfo->lflg = zipfileinfo->flg;

            /* if local header in another split, putlocal will close it */
            if ((r = putlocal(zipfileinfo, PUTLOCAL_REWRITE)) != ZE_OK)
            {
                printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
                return r;
            }

            if (zfseeko(y, bytes_this_split, SEEK_SET))
            {
                printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
                return ZE_READ;
            }

            if ((zipfileinfo->flg & 1) != 0) {
                printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
                /* encrypted file, extended header still required */
                if ((r = putextended(zipfileinfo)) != ZE_OK)
                {
                    printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
                    return r;
                }
                if (zip64_entry)
                {
                    printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
                    tempzn += 24L;
                }
                else
                {
                    printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
                    tempzn += 16L;
                }
            }
        }
    } /* isdir */
    /* Free the local extra field which is no longer needed */
    if (zipfileinfo->ext) {
        if (zipfileinfo->extra != zipfileinfo->cextra) {
            printf("[Debug] zipup(),[%s][%d]\n", __FILE__, __LINE__);
            free((zvoid *)(zipfileinfo->extra));
            zipfileinfo->extra = NULL;
        }
        zipfileinfo->ext = 0;
    }

    /* Display statistics */
    if (noisy)
    {
        printf("[Debug] [%d] zipup() [%d%%][oldsize=%d:compress size=%d]\n", __LINE__, percent(isize, s), isize, s);
        if (m == DEFLATE)
            fprintf(mesg, " (deflated %d%%)\n", percent(isize, s));
        else
            fprintf(mesg, " (stored 0%%)\n");
        mesg_line_started = 0;
        fflush(mesg);
    }
    return ZE_OK;
}




local unsigned file_read(buf, size)
    char *buf;
    unsigned size;
    /* Read a new buffer from the current input file, perform end-of-line
     * translation, and update the crc and input file size.
     * IN assertion: size >= 2 (for end-of-line translation)
     */
{
    unsigned len;
    char *b;
    zoff_t isize_prev;    /* Previous isize.  Used for overflow check. */

    if (translate_eol == 0) {
        len = zread(ifile, buf, size);
        if (len == (unsigned)EOF || len == 0) return len;
    } else if (translate_eol == 1) {
        /* translate_eol == 1 */
        /* Transform LF to CR LF */
        size >>= 1;
        b = buf+size;
        size = len = zread(ifile, b, size);
        if (len == (unsigned)EOF || len == 0) return len;

        /* check buf for binary - 12/16/04 */
        if (file_binary == -1) {
            /* first read */
            file_binary = is_text_buf(b, size) ? 0 : 1;
        }

        if (file_binary != 1) {
            {
                do {
                    if ((*buf++ = *b++) == '\n') *(buf-1) = CR, *buf++ = LF, len++;
                } while (--size != 0);
            }
            buf -= len;
        } else { /* do not translate binary */
            memcpy(buf, b, size);
        }

    } else {
        /* translate_eol == 2 */
        /* Transform CR LF to LF and suppress final ^Z */
        b = buf;
        size = len = zread(ifile, buf, size-1);
        if (len == (unsigned)EOF || len == 0) return len;

        /* check buf for binary - 12/16/04 */
        if (file_binary == -1) {
            /* first read */
            file_binary = is_text_buf(b, size) ? 0 : 1;
        }

        if (file_binary != 1) {
            buf[len] = '\n'; /* I should check if next char is really a \n */
            {
                do {
                    if (( *buf++ = *b++) == CR && *b == LF) buf--, len--;
                } while (--size != 0);
            }
            if (len == 0) {
                zread(ifile, buf, 1); len = 1; /* keep single \r if EOF */
            } else {
                buf -= len;
                if (buf[len-1] == CTRLZ) len--; /* suppress final ^Z */
            }
        }
    }
    crc = crc32(crc, (uch *) buf, len);
    /* 2005-05-23 SMS.
       Increment file size.  A small-file program reading a large file may
       cause isize to overflow, so complain (and abort) if it goes
       negative or wraps around.  Awful things happen later otherwise.
       */
    isize_prev = isize;
    isize += (ulg)len;
    if (isize < isize_prev) {
        ZIPERR(ZE_BIG, "overflow in byte count");
    }
    return len;
}



/* ===========================================================================
 * Flush the current output buffer.
 */
void flush_outbuf(o_buf, o_idx)
    char *o_buf;
    unsigned *o_idx;
{
    if (y == NULL) {
        error("output buffer too small for in-memory compression");
    }
    /* Encrypt and write the output buffer: */
    if (*o_idx != 0) {
        zfwrite(o_buf, 1, (extent)*o_idx);
        if (ferror(y)) ziperr(ZE_WRITE, "write error on zip file");
    }
    *o_idx = 0;
}

/* ===========================================================================
 * Return true if the zip file can be seeked. This is used to check if
 * the local header can be re-rewritten. This function always returns
 * true for in-memory compression.
 * IN assertion: the local header has already been written (ftell() > 0).
 */
int seekable()
{
    return fseekable(y);
}

/* ===========================================================================
 * Compression to archive file.
 */
local zoff_t filecompress(struct zlist far *z_entry, int *cmpr_method)
{
    printf("[Debug] filecompress=%d\n", __LINE__);
    /* Set the defaults for file compression. */
    read_buf = file_read;

    /* Initialize deflate's internals and execute file compression. */
    bi_init(file_outbuf, sizeof(file_outbuf), TRUE);
    ct_init(&z_entry->att, cmpr_method);
    lm_init(level, &z_entry->flg);
    return deflate();
}
#endif /* !UTIL */
