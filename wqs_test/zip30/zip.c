/*
   zip.c - Zip 3

   Copyright (c) 1990-2008 Info-ZIP.  All rights reserved.

   See the accompanying file LICENSE, version 2007-Mar-4 or later
   (the contents of which are also included in zip.h) for terms of use.
   If, for some reason, all these files are missing, the Info-ZIP license
   also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
   */
/*
 *  zip.c by Mark Adler.
 */
#define __ZIP_C

#include "zip.h"
#include <time.h>       /* for tzset() declaration */
#define DEFCPYRT        /* main module: enable copyright string defines! */
#include "revision.h"
#include "crc32.h"
#include "crypt.h"
#include "ttyio.h"
#include <ctype.h>
#include <errno.h>

#include <signal.h>
#include <stdio.h>

#define MAXCOM 256      /* Maximum one-line comment size */


/* Local option flags */
#define DELETE  0

#define ADD     1
#define UPDATE  2
#define FRESHEN 3
#define ARCHIVE 4
local int action = ADD; /* one of ADD, UPDATE, FRESHEN, DELETE, or ARCHIVE */
local int comadd = 0;   /* 1=add comments for new files */
local int zipedit = 0;  /* 1=edit zip comment and all file comments */
local int latest = 0;   /* 1=set zip file time to time of latest file */
local int test = 0;     /* 1=test zip file with unzip -t */
local char *unzip_path = NULL; /* where to find unzip */
local int tempdir = 0;  /* 1=use temp directory (-b) */
local int junk_sfx = 0; /* 1=junk the sfx prefix */

/* Pointer to crc_table, needed in crypt.c */
ZCONST ulg near *crc_32_tab;

/* Local functions */

local void freeup  OF((void));
local int  finish  OF((int));
local void handler OF((int));
local void license OF((void));
local void help    OF((void));
local void help_extended OF((void));

/* prereading of arguments is not supported in new command
   line interpreter get_option() so read filters as arguments
   are processed and convert to expected array later */
local int filterlist_to_patterns OF((void));
/* not used
   local int get_filters OF((int argc, char **argv));
   */

/* list to store file arguments */
local long add_name OF((char *filearg));


local int DisplayRunningStats OF((void));

local void version_info OF((void));
local void zipstdout OF((void));
local int check_unzip_version OF((char *unzippath));
local void check_zipfile OF((char *zipname, char *zippath));

/* structure used by add_filter to store filters */
struct filterlist_struct {
    char flag;
    char *pattern;
    struct filterlist_struct *next;
};
struct filterlist_struct *filterlist = NULL;  /* start of list */
struct filterlist_struct *lastfilter = NULL;  /* last filter in list */

/* structure used by add_filearg to store file arguments */
struct filelist_struct {
    char *name;
    struct filelist_struct *next;
};
long filearg_count = 0;
struct filelist_struct *filelist = NULL;  /* start of list */
struct filelist_struct *lastfile = NULL;  /* last file in list */

local void freeup()
    /* Free all allocations in the 'found' list, the 'zfiles' list and
       the 'patterns' list. */
{
    struct flist far *f;  /* steps through found list */
    struct zlist far *z;  /* pointer to next entry in zfiles list */

    for (f = found; f != NULL; f = fexpel(f))
        ;
    while (zfiles != NULL)
    {
        z = zfiles->nxt;
        if (zfiles->zname && zfiles->zname != zfiles->name)
            free((zvoid *)(zfiles->zname));
        if (zfiles->name)
            free((zvoid *)(zfiles->name));
        if (zfiles->iname)
            free((zvoid *)(zfiles->iname));
        if (zfiles->cext && zfiles->cextra && zfiles->cextra != zfiles->extra)
            free((zvoid *)(zfiles->cextra));
        if (zfiles->ext && zfiles->extra)
            free((zvoid *)(zfiles->extra));
        if (zfiles->com && zfiles->comment)
            free((zvoid *)(zfiles->comment));
        if (zfiles->oname)
            free((zvoid *)(zfiles->oname));
        if (zfiles->uname)
            free((zvoid *)(zfiles->uname));
        if (zfiles->zuname)
            free((zvoid *)(zfiles->zuname));
        if (zfiles->ouname)
            free((zvoid *)(zfiles->ouname));
        farfree((zvoid far *)zfiles);
        zfiles = z;
        zcount--;
    }

    if (patterns != NULL) {
        while (pcount-- > 0) {
            if (patterns[pcount].zname != NULL)
                free((zvoid *)(patterns[pcount].zname));
        }
        free((zvoid *)patterns);
        patterns = NULL;
    }

    /* close logfile */
    if (logfile) {
        fclose(logfile);
    }
}

local int finish(e)
    int e;                  /* exit code */
    /* Process -o and -m options (if specified), free up malloc'ed stuff, and
       exit with the code e. */
{
    int r;                /* return value from trash() */
    ulg t;                /* latest time in zip file */
    struct zlist far *z;  /* pointer into zfile list */

    /* If latest, set time to zip file to latest file in zip file */
    if (latest && zipfile && strcmp(zipfile, "-"))
    {
        diag("changing time of zip file to time of latest file in it");
        /* find latest time in zip file */
        if (zfiles == NULL)
            zipwarn("zip file is empty, can't make it as old as latest entry", "");
        else {
            t = 0;
            for (z = zfiles; z != NULL; z = z->nxt)
                /* Ignore directories in time comparisons */
                if (z->iname[z->nam-1] != (char)0x2f)   /* ascii '/' */
                {
                    iztimes z_utim;
                    ulg z_tim;

                    z_tim = ((get_ef_ut_ztime(z, &z_utim) & EB_UT_FL_MTIME) ?
                            unix2dostime(&z_utim.mtime) : z->tim);
                    if (t < z_tim)
                        t = z_tim;
                }
            /* set modified time of zip file to that time */
            if (t != 0)
                stamp(zipfile, t);
            else
                zipwarn(
                        "zip file has only directories, can't make it as old as latest entry",
                        "");
        }
    }
    if (tempath != NULL)
    {
        free((zvoid *)tempath);
        tempath = NULL;
    }
    if (zipfile != NULL)
    {
        free((zvoid *)zipfile);
        zipfile = NULL;
    }
    if (in_file != NULL)
    {
        fclose(in_file);
        in_file = NULL;
    }
    if (in_path != NULL)
    {
        free((zvoid *)in_path);
        in_path = NULL;
    }
    if (out_path != NULL)
    {
        free((zvoid *)out_path);
        out_path = NULL;
    }
    if (zcomment != NULL)
    {
        free((zvoid *)zcomment);
        zcomment = NULL;
    }


    /* If dispose, delete all files in the zfiles list that are marked */
    if (dispose)
    {
        diag("deleting files that were added to zip file");
        if ((r = trash()) != ZE_OK)
            ZIPERR(r, "was deleting moved files and directories");
    }


    /* Done! */
    freeup();
    return e;
}

void ziperr(c, h)
    int c;                  /* error code from the ZE_ class */
    ZCONST char *h;         /* message about how it happened */
    /* Issue a message for the error, clean up files and memory, and exit. */
{
    static int error_level = 0;

    if (error_level++ > 0)
        /* avoid recursive ziperr() printouts (his should never happen) */
        EXIT(ZE_LOGIC);  /* ziperr recursion is an internal logic error! */

    if (mesg_line_started) {
        fprintf(mesg, "\n");
        mesg_line_started = 0;
    }
    if (logfile && logfile_line_started) {
        fprintf(logfile, "\n");
        logfile_line_started = 0;
    }
    if (h != NULL) {
        if (PERR(c))
            fprintf(mesg, "zip I/O error: %s", strerror(errno));
        /* perror("zip I/O error"); */
        fflush(mesg);
        fprintf(mesg, "\nzip error: %s (%s)\n", ZIPERRORS(c), h);
        if (logfile) {
            if (PERR(c))
                fprintf(logfile, "zip I/O error: %s\n", strerror(errno));
            fprintf(logfile, "\nzip error: %s (%s)\n", ZIPERRORS(c), h);
            logfile_line_started = 0;
        }
    }
    if (tempzip != NULL)
    {
        if (tempzip != zipfile) {
            if (current_local_file)
                fclose(current_local_file);
            if (y != current_local_file && y != NULL)
                fclose(y);
            destroy(tempzip);
            free((zvoid *)tempzip);
        } else {
            /* -g option, attempt to restore the old file */

            /* zip64 support 09/05/2003 R.Nausedat */
            uzoff_t k = 0;                        /* keep count for end header */
            uzoff_t cb = cenbeg;                  /* get start of central */

            struct zlist far *z;  /* steps through zfiles linked list */

            fprintf(mesg, "attempting to restore %s to its previous state\n",
                    zipfile);
            if (logfile)
                fprintf(logfile, "attempting to restore %s to its previous state\n",
                        zipfile);

            zfseeko(y, cenbeg, SEEK_SET);

            tempzn = cenbeg;
            for (z = zfiles; z != NULL; z = z->nxt)
            {
                putcentral(z);
                tempzn += 4 + CENHEAD + z->nam + z->cext + z->com;
                k++;
            }
            putend(k, tempzn - cb, cb, zcomlen, zcomment);
            fclose(y);
            y = NULL;
        }
    }

    if (key != NULL) {
        free((zvoid *)key);
        key = NULL;
    }
    if (tempath != NULL) {
        free((zvoid *)tempath);
        tempath = NULL;
    }
    if (zipfile != NULL) {
        free((zvoid *)zipfile);
        zipfile = NULL;
    }
    if (out_path != NULL) {
        free((zvoid *)out_path);
        out_path = NULL;
    }
    if (zcomment != NULL) {
        free((zvoid *)zcomment);
        zcomment = NULL;
    }

    freeup();
    EXIT(c);
}


void error(h)
    ZCONST char *h;
    /* Internal error, should never happen */
{
    ziperr(ZE_LOGIC, h);
}

local void handler(s)
    int s;                  /* signal number (ignored) */
    /* Upon getting a user interrupt, turn echo back on for tty and abort
       cleanly using ziperr(). */
{
    echon();
    putc('\n', mesg);
    ziperr(ZE_ABORT, "aborting");
    s++;                                  /* keep some compilers happy */
}

void zipmessage_nl(a, nl)
    ZCONST char *a;     /* message string to output */
    int nl;             /* 1 = add nl to end */
    /* If nl false, print a message to mesg without new line.
       If nl true, print and add new line.  If logfile is
       open then also write message to log file. */
{
    if (noisy) {
        if (a && strlen(a)) {
            fprintf(mesg, "%s", a);
            mesg_line_started = 1;
        }
        if (nl) {
            if (mesg_line_started) {
                fprintf(mesg, "\n");
                mesg_line_started = 0;
            }
        } else if (a && strlen(a)) {
            mesg_line_started = 1;
        }
        fflush(mesg);
    }
    if (logfile) {
        if (a && strlen(a)) {
            fprintf(logfile, "%s", a);
            logfile_line_started = 1;
        }
        if (nl) {
            if (logfile_line_started) {
                fprintf(logfile, "\n");
                logfile_line_started = 0;
            }
        } else if (a && strlen(a)) {
            logfile_line_started = 1;
        }
        fflush(logfile);
    }
}

void zipmessage(a, b)
    ZCONST char *a, *b;     /* message strings juxtaposed in output */
    /* Print a message to mesg and flush.  Also write to log file if
       open.  Write new line first if current line has output already. */
{
}

void zipwarn(a, b)
    ZCONST char *a, *b;     /* message strings juxtaposed in output */
    /* Print a warning message to mesg (usually stderr) and return. */
{
    if (noisy) {
        if (mesg_line_started)
            fprintf(mesg, "\n");
        fprintf(mesg, "\tzip warning: %s%s\n", a, b);
        mesg_line_started = 0;
        fflush(mesg);
    }
    if (logfile) {
        if (logfile_line_started)
            fprintf(logfile, "\n");
        fprintf(logfile, "\tzip warning: %s%s\n", a, b);
        logfile_line_started = 0;
        fflush(logfile);
    }
}

local void license()
    /* Print license information to stdout. */
{
    extent i;             /* counter for copyright array */

    for (i = 0; i < sizeof(swlicense)/sizeof(char *); i++)
        puts(swlicense[i]);
}

local void help()
    /* Print help (along with license info) to stdout. */
{
    extent i;             /* counter for help array */

    /* help array */
    static ZCONST char *text[] = {
        "Zip %s (%s). Usage:",
        "zip [-options] [-b path] [-t mmddyyyy] [-n suffixes] [zipfile list] [-xi list]",
        "  The default action is to add or replace zipfile entries from list, which",
        "  can include the special name - to compress standard input.",
        "  If zipfile and list are omitted, zip compresses stdin to stdout.",
        "  -f   freshen: only changed files  -u   update: only changed or new files",
        "  -d   delete entries in zipfile    -m   move into zipfile (delete OS files)",
        "  -r   recurse into directories     -j   junk (don't record) directory names",
        "  -0   store only                   -l   convert LF to CR LF (-ll CR LF to LF)",
        "  -1   compress faster              -9   compress better",
        "  -q   quiet operation              -v   verbose operation/print version info",
        "  -c   add one-line comments        -z   add zipfile comment",
        "  -@   read names from stdin        -o   make zipfile as old as latest entry",
        "  -x   exclude the following names  -i   include only the following names",
        "  -F   fix zipfile (-FF try harder) -D   do not add directory entries",
        "  -A   adjust self-extracting exe   -J   junk zipfile prefix (unzipsfx)",
        "  -T   test zipfile integrity       -X   eXclude eXtra file attributes",
        "  -y   store symbolic links as the link instead of the referenced file",
        /*
           "  -R   PKZIP recursion (see manual)",
           */
        "  -e   encrypt                      -n   don't compress these suffixes"
            ,"  -h2  show more help"
            ,"  "
    };

    for (i = 0; i < sizeof(copyright)/sizeof(char *); i++)
    {
        printf(copyright[i], "zip");
        putchar('\n');
    }
    for (i = 0; i < sizeof(text)/sizeof(char *); i++)
    {
        printf(text[i], VERSION, REVDATE);
        putchar('\n');
    }
}

local void help_extended()
    /* Print extended help to stdout. */
{
    extent i;             /* counter for help array */

    /* help array */
    static ZCONST char *text[] = {
        "",
        "Extended Help for Zip",
        "",
        "See the Zip Manual for more detailed help",
        "",
        "",
        "Zip stores files in zip archives.  The default action is to add or replace",
        "zipfile entries.",
        "",
        "Basic command line:",
        "  zip options archive_name file file ...",
        "",
        "Some examples:",
        "  Add file.txt to z.zip (create z if needed):      zip z file.txt",
        "  Zip all files in current dir:                    zip z *",
        "  Zip files in current dir and subdirs also:       zip -r z .",
        "",
        "Basic modes:",
        " External modes (selects files from file system):",
        "        add      - add new files/update existing files in archive (default)",
        "  -u    update   - add new files/update existing files only if later date",
        "  -f    freshen  - update existing files only (no files added)",
        "  -FS   filesync - update if date or size changed, delete if no OS match",
        " Internal modes (selects entries in archive):",
        "  -d    delete   - delete files from archive (see below)",
        "  -U    copy     - select files in archive to copy (use with --out)",
        "",
        "Basic options:",
        "  -r        recurse into directories (see Recursion below)",
        "  -m        after archive created, delete original files (move into archive)",
        "  -j        junk directory names (store just file names)",
        "  -q        quiet operation",
        "  -v        verbose operation (just \"zip -v\" shows version information)",
        "  -c        prompt for one-line comment for each entry",
        "  -z        prompt for comment for archive (end with just \".\" line or EOF)",
        "  -@        read names to zip from stdin (one path per line)",
        "  -o        make zipfile as old as latest entry",
        "",
        "",
        "Syntax:",
        "  The full command line syntax is:",
        "",
        "    zip [-shortopts ...] [--longopt ...] [zipfile [path path ...]] [-xi list]",
        "",
        "  Any number of short option and long option arguments are allowed",
        "  (within limits) as well as any number of path arguments for files",
        "  to zip up.  If zipfile exists, the archive is read in.  If zipfile",
        "  is \"-\", stream to stdout.  If any path is \"-\", zip stdin.",
        "",
        "Options and Values:",
        "  For short options that take values, use -ovalue or -o value or -o=value",
        "  For long option values, use either --longoption=value or --longoption value",
        "  For example:",
        "    zip -ds 10 --temp-dir=path zipfile path1 path2 --exclude pattern pattern",
        "  Avoid -ovalue (no space between) to avoid confusion",
        "  In particular, be aware of 2-character options.  For example:",
        "    -d -s is (delete, split size) while -ds is (dot size)",
        "  Usually better to break short options across multiple arguments by function",
        "    zip -r -dbdcds 10m -lilalf logfile archive input_directory -ll",
        "",
        "  All args after just \"--\" arg are read verbatim as paths and not options.",
        "    zip zipfile path path ... -- verbatimpath verbatimpath ...",
        "  Use -nw to also disable wildcards, so paths are read literally:",
        "    zip zipfile -nw -- \"-leadingdashpath\" \"a[path].c\" \"path*withwildcard\"",
        "  You may still have to escape or quote arguments to avoid shell expansion",
        "",
        "Wildcards:",
        "  Internally zip supports the following wildcards:",
        "    ?       (or %% or #, depending on OS) matches any single character",
        "    *       matches any number of characters, including zero",
        "    [list]  matches char in list (regex), can do range [ac-f], all but [!bf]",
        "  If port supports [], must escape [ as [[] or use -nw to turn off wildcards",
        "  For shells that expand wildcards, escape (\\* or \"*\") so zip can recurse",
        "    zip zipfile -r . -i \"*.h\"",
        "",
        "  Normally * crosses dir bounds in path, e.g. 'a*b' can match 'ac/db'.  If",
        "   -ws option used, * does not cross dir bounds but ** does",
        "",
        "  For DOS and Windows, [list] is now disabled unless the new option",
        "  -RE       enable [list] (regular expression) matching",
        "  is used to avoid problems with file paths containing \"[\" and \"]\":",
        "    zip files_ending_with_number -RE foo[0-9].c",
        "",
        "Include and Exclude:",
        "  -i pattern pattern ...   include files that match a pattern",
        "  -x pattern pattern ...   exclude files that match a pattern",
        "  Patterns are paths with optional wildcards and match paths as stored in",
        "  archive.  Exclude and include lists end at next option, @, or end of line.",
        "    zip -x pattern pattern @ zipfile path path ...",
        "",
        "Case matching:",
        "  On most OS the case of patterns must match the case in the archive, unless",
        "  the -ic option is used.",
        "  -ic       ignore case of archive entries",
        "  This option not available on case-sensitive file systems.  On others, case",
        "  ignored when matching files on file system but matching against archive",
        "  entries remains case sensitive for modes -f (freshen), -U (archive copy),",
        "  and -d (delete) because archive paths are always case sensitive.  With",
        "  -ic, all matching ignores case, but it's then possible multiple archive",
        "  entries that differ only in case will match.",
        "",
        "End Of Line Translation (text files only):",
        "  -l        change CR or LF (depending on OS) line end to CR LF (Unix->Win)",
        "  -ll       change CR LF to CR or LF (depending on OS) line end (Win->Unix)",
        "  If first buffer read from file contains binary the translation is skipped",
        "",
        "Recursion:",
        "  -r        recurse paths, include files in subdirs:  zip -r a path path ...",
        "  -R        recurse current dir and match patterns:   zip -R a ptn ptn ...",
        "  Use -i and -x with either to include or exclude paths",
        "  Path root in archive starts at current dir, so if /a/b/c/file and",
        "   current dir is /a/b, 'zip -r archive .' puts c/file in archive",
        "",
        "Date filtering:",
        "  -t date   exclude before (include files modified on this date and later)",
        "  -tt date  include before (include files modified before date)",
        "  Can use both at same time to set a date range",
        "  Dates are mmddyyyy or yyyy-mm-dd",
        "",
        "Deletion, File Sync:",
        "  -d        delete files",
        "  Delete archive entries matching internal archive paths in list",
        "    zip archive -d pattern pattern ...",
        "  Can use -t and -tt to select files in archive, but NOT -x or -i, so",
        "    zip archive -d \"*\" -t 2005-12-27",
        "  deletes all files from archive.zip with date of 27 Dec 2005 and later",
        "  Note the * (escape as \"*\" on Unix) to select all files in archive",
        "",
        "  -FS       file sync",
        "  Similar to update, but files updated if date or size of entry does not",
        "  match file on OS.  Also deletes entry from archive if no matching file",
        "  on OS.",
        "    zip archive_to_update -FS -r dir_used_before",
        "  Result generally same as creating new archive, but unchanged entries",
        "  are copied instead of being read and compressed so can be faster.",
        "      WARNING:  -FS deletes entries so make backup copy of archive first",
        "",
        "Compression:",
        "  -0        store files (no compression)",
        "  -1 to -9  compress fastest to compress best (default is 6)",
        "  -Z cm     set compression method to cm:",
        "              store   - store without compression, same as option -0",
        "              deflate - original zip deflate, same as -1 to -9 (default)",
        "            if bzip2 is enabled:",
        "              bzip2 - use bzip2 compression (need modern unzip)",
        "",
        "Encryption:",
        "  -e        use standard (weak) PKZip 2.0 encryption, prompt for password",
        "  -P pswd   use standard encryption, password is pswd",
        "",
        "Splits (archives created as a set of split files):",
        "  -s ssize  create split archive with splits of size ssize, where ssize nm",
        "              n number and m multiplier (kmgt, default m), 100k -> 100 kB",
        "  -sp       pause after each split closed to allow changing disks",
        "      WARNING:  Archives created with -sp use data descriptors and should",
        "                work with most unzips but may not work with some",
        "  -sb       ring bell when pause",
        "  -sv       be verbose about creating splits",
        "      Split archives CANNOT be updated, but see --out and Copy Mode below",
        "",
        "Using --out (output to new archive):",
        "  --out oa  output to new archive oa",
        "  Instead of updating input archive, create new output archive oa.",
        "  Result is same as without --out but in new archive.  Input archive",
        "  unchanged.",
        "      WARNING:  --out ALWAYS overwrites any existing output file",
        "  For example, to create new_archive like old_archive but add newfile1",
        "  and newfile2:",
        "    zip old_archive newfile1 newfile2 --out new_archive",
        "  Cannot update split archive, so use --out to out new archive:",
        "    zip in_split_archive newfile1 newfile2 --out out_split_archive",
        "  If input is split, output will default to same split size",
        "  Use -s=0 or -s- to turn off splitting to convert split to single file:",
        "    zip in_split_archive -s 0 --out out_single_file_archive",
        "      WARNING:  If overwriting old split archive but need less splits,",
        "                old splits not overwritten are not needed but remain",
        "",
        "Copy Mode (copying from archive to archive):",
        "  -U        (also --copy) select entries in archive to copy (reverse delete)",
        "  Copy Mode copies entries from old to new archive with --out and is used by",
        "  zip when either no input files on command line or -U (--copy) used.",
        "    zip inarchive --copy pattern pattern ... --out outarchive",
        "  To copy only files matching *.c into new archive, excluding foo.c:",
        "    zip old_archive --copy \"*.c\" --out new_archive -x foo.c",
        "  If no input files and --out, copy all entries in old archive:",
        "    zip old_archive --out new_archive",
        "",
        "Streaming and FIFOs:",
        "  prog1 | zip -ll z -      zip output of prog1 to zipfile z, converting CR LF",
        "  zip - -R \"*.c\" | prog2   zip *.c files in current dir and stream to prog2 ",
        "  prog1 | zip | prog2      zip in pipe with no in or out acts like zip - -",
        "  If Zip is Zip64 enabled, streaming stdin creates Zip64 archives by default",
        "   that need PKZip 4.5 unzipper like UnZip 6.0",
        "  WARNING:  Some archives created with streaming use data descriptors and",
        "            should work with most unzips but may not work with some",
        "  Can use -fz- to turn off Zip64 if input not large (< 4 GB):",
        "    prog_with_small_output | zip archive -fz-",
        "",
        "  Zip now can read Unix FIFO (named pipes).  Off by default to prevent zip",
        "  from stopping unexpectedly on unfed pipe, use -FI to enable:",
        "    zip -FI archive fifo",
        "",
        "Dots, counts:",
        "  -db       display running count of bytes processed and bytes to go",
        "              (uncompressed size, except delete and copy show stored size)",
        "  -dc       display running count of entries done and entries to go",
        "  -dd       display dots every 10 MB (or dot size) while processing files",
        "  -dg       display dots globally for archive instead of for each file",
        "    zip -qdgds 10m   will turn off most output except dots every 10 MB",
        "  -ds siz   each dot is siz processed where siz is nm as splits (0 no dots)",
        "  -du       display original uncompressed size for each entry as added",
        "  -dv       display volume (disk) number in format in_disk>out_disk",
        "  Dot size is approximate, especially for dot sizes less than 1 MB",
        "  Dot options don't apply to Scanning files dots (dot/2sec) (-q turns off)",
        "",
        "Logging:",
        "  -lf path  open file at path as logfile (overwrite existing file)",
        "  -la       append to existing logfile",
        "  -li       include info messages (default just warnings and errors)",
        "",
        "Testing archives:",
        "  -T        test completed temp archive with unzip before updating archive",
        "  -TT cmd   use command cmd instead of 'unzip -tqq' to test archive",
        "             On Unix, to use unzip in current directory, could use:",
        "               zip archive file1 file2 -T -TT \"./unzip -tqq\"",
        "             In cmd, {} replaced by temp archive path, else temp appended.",
        "             The return code is checked for success (0 on Unix)",
        "",
        "Fixing archives:",
        "  -F        attempt to fix a mostly intact archive (try this first)",
        "  -FF       try to salvage what can (may get more but less reliable)",
        "  Fix options copy entries from potentially bad archive to new archive.",
        "  -F tries to read archive normally and copy only intact entries, while",
        "  -FF tries to salvage what can and may result in incomplete entries.",
        "  Must use --out option to specify output archive:",
        "    zip -F bad.zip --out fixed.zip",
        "  Use -v (verbose) with -FF to see details:",
        "    zip reallybad.zip -FF -v --out fixed.zip",
        "  Currently neither option fixes bad entries, as from text mode ftp get.",
        "",
        "Difference mode:",
        "  -DF       (also --dif) only include files that have changed or are",
        "             new as compared to the input archive",
        "  Difference mode can be used to create incremental backups.  For example:",
        "    zip --dif full_backup.zip -r somedir --out diff.zip",
        "  will store all new files, as well as any files in full_backup.zip where",
        "  either file time or size have changed from that in full_backup.zip,",
        "  in new diff.zip.  Output archive not excluded automatically if exists,",
        "  so either use -x to exclude it or put outside what is being zipped.",
        "",
        "DOS Archive bit (Windows only):",
        "  -AS       include only files with the DOS Archive bit set",
        "  -AC       after archive created, clear archive bit of included files",
        "      WARNING: Once the archive bits are cleared they are cleared",
        "               Use -T to test the archive before the bits are cleared",
        "               Can also use -sf to save file list before zipping files",
        "",
        "Show files:",
        "  -sf       show files to operate on and exit (-sf- logfile only)",
        "  -su       as -sf but show escaped UTF-8 Unicode names also if exist",
        "  -sU       as -sf but show escaped UTF-8 Unicode names instead",
        "  Any character not in the current locale is escaped as #Uxxxx, where x",
        "  is hex digit, if 16-bit code is sufficient, or #Lxxxxxx if 24-bits",
        "  are needed.  If add -UN=e, Zip escapes all non-ASCII characters.",
        "",
        "Unicode:",
        "  If compiled with Unicode support, Zip stores UTF-8 path of entries.",
        "  This is backward compatible.  Unicode paths allow better conversion",
        "  of entry names between different character sets.",
        "",
        "  New Unicode extra field includes checksum to verify Unicode path",
        "  goes with standard path for that entry (as utilities like ZipNote",
        "  can rename entries).  If these do not match, use below options to",
        "  set what Zip does:",
        "      -UN=Quit     - if mismatch, exit with error",
        "      -UN=Warn     - if mismatch, warn, ignore UTF-8 (default)",
        "      -UN=Ignore   - if mismatch, quietly ignore UTF-8",
        "      -UN=No       - ignore any UTF-8 paths, use standard paths for all",
        "  An exception to -UN=N are entries with new UTF-8 bit set (instead",
        "  of using extra fields).  These are always handled as Unicode.",
        "",
        "  Normally Zip escapes all chars outside current char set, but leaves",
        "  as is supported chars, which may not be OK in path names.  -UN=Escape",
        "  escapes any character not ASCII:",
        "    zip -sU -UN=e archive",
        "  Can use either normal path or escaped Unicode path on command line",
        "  to match files in archive.",
        "",
        "  Zip now stores UTF-8 in entry path and comment fields on systems",
        "  where UTF-8 char set is default, such as most modern Unix, and",
        "  and on other systems in new extra fields with escaped versions in",
        "  entry path and comment fields for backward compatibility.",
        "  Option -UN=UTF8 will force storing UTF-8 in entry path and comment",
        "  fields:",
        "      -UN=UTF8     - store UTF-8 in entry path and comment fields",
        "  This option can be useful for multi-byte char sets on Windows where",
        "  escaped paths and comments can be too long to be valid as the UTF-8",
        "  versions tend to be shorter.",
        "",
        "  Only UTF-8 comments on UTF-8 native systems supported.  UTF-8 comments",
        "  for other systems planned in next release.",
        "",
        "Self extractor:",
        "  -A        Adjust offsets - a self extractor is created by prepending",
        "             the extractor executable to archive, but internal offsets",
        "             are then off.  Use -A to fix offsets.",
        "  -J        Junk sfx - removes prepended extractor executable from",
        "             self extractor, leaving a plain zip archive.",
        "",
        "More option highlights (see manual for additional options and details):",
        "  -b dir    when creating or updating archive, create the temp archive in",
        "             dir, which allows using seekable temp file when writing to a",
        "             write once CD, such archives compatible with more unzips",
        "             (could require additional file copy if on another device)",
        "  -MM       input patterns must match at least one file and matched files",
        "             must be readable or exit with OPEN error and abort archive",
        "             (without -MM, both are warnings only, and if unreadable files",
        "             are skipped OPEN error (18) returned after archive created)",
        "  -nw       no wildcards (wildcards are like any other character)",
        "  -sc       show command line arguments as processed and exit",
        "  -sd       show debugging as Zip does each step",
        "  -so       show all available options on this system",
        "  -X        default=strip old extra fields, -X- keep old, -X strip most",
        "  -ws       wildcards don't span directory boundaries in paths",
        ""
    };

    for (i = 0; i < sizeof(text)/sizeof(char *); i++)
    {
        printf(text[i]);
        putchar('\n');
    }
}

/*
 * XXX version_info() in a separate file
 */
local void version_info()
    /* Print verbose info about program version and compile time options
       to stdout. */
{
    extent i;             /* counter in text arrays */
    char *envptr;

    /* Bzip2 option string storage (with version). */


    /* Options info array */
    static ZCONST char *comp_opts[] = {
        "MMAP",
        "USE_EF_UT_TIME       (store Universal Time)",
        "SYMLINK_SUPPORT      (symbolic links supported)",
        "LARGE_FILE_SUPPORT   (can read and write large files on file system)",
        "ZIP64_SUPPORT        (use Zip64 to store large files in archives)",
        "UNICODE_SUPPORT      (store and read UTF-8 Unicode paths)",

        "STORE_UNIX_UIDs_GIDs (store UID/GID sizes/values using new extra field)",
        "UIDGID_NOT_16BIT     (old Unix 16-bit UID/GID extra field not used)",
        NULL
    };

    static ZCONST char *zipenv_names[] = {
        "ZIP"
            ,"ZIPOPT"
    };

    for (i = 0; i < sizeof(copyright)/sizeof(char *); i++)
    {
        printf(copyright[i], "zip");
        putchar('\n');
    }

    for (i = 0; i < sizeof(versinfolines)/sizeof(char *); i++)
    {
        printf(versinfolines[i], "Zip", VERSION, REVDATE);
        putchar('\n');
    }

    version_local();

    puts("Zip special compilation options:");

    /* Fill in bzip2 version.  (32-char limit valid as of bzip 1.0.3.) */

    for (i = 0; (int)i < (int)(sizeof(comp_opts)/sizeof(char *) - 1); i++)
    {
        printf("\t%s\n",comp_opts[i]);
    }
    printf("\t[encryption, version %d.%d%s of %s] (modified for Zip 3)\n\n",
            CR_MAJORVER, CR_MINORVER, CR_BETA_VER, CR_VERSION_DATE);
    for (i = 0; i < sizeof(cryptnote)/sizeof(char *); i++)
    {
        printf(cryptnote[i]);
        putchar('\n');
    }
    ++i;  /* crypt support means there IS at least one compilation option */
    if (i == 0)
        puts("\t[none]");

    puts("\nZip environment options:");
    for (i = 0; i < sizeof(zipenv_names)/sizeof(char *); i++)
    {
        envptr = getenv(zipenv_names[i]);
        printf("%16s:  %s\n", zipenv_names[i],
                ((envptr == (char *)NULL || *envptr == 0) ? "[none]" : envptr));
    }
}


/* Default to case-sensitive matching of archive entries for the modes
   that specifically operate on archive entries, as this archive may
   have come from a system that allows paths in the archive to differ
   only by case.  Except for adding ARCHIVE (copy mode), this is how it
   was done before.  Note that some case-insensitive ports (WIN32, VMS)
   define their own PROCNAME() in their respective osdep.h that use the
   filter_match_case flag set to FALSE by the -ic option to enable
   case-insensitive archive entry mathing. */
#  define PROCNAME(n) procname(n, (action == ARCHIVE || action == DELETE || action == FRESHEN) && filter_match_case)

local void zipstdout()
    /* setup for writing zip file on stdout */
{
    mesg = stderr;
    if (isatty(1))
        ziperr(ZE_PARMS, "cannot write zip file to terminal");
    if ((zipfile = malloc(4)) == NULL)
        ziperr(ZE_MEM, "was processing arguments");
    strcpy(zipfile, "-");
    /*
       if ((r = readzipfile()) != ZE_OK)
       ziperr(r, zipfile);
       */
}

local int check_unzip_version(unzippath)
    char *unzippath;
{
    /* Here is where we need to check for the version of unzip the user
     * has.  If creating a Zip64 archive need UnZip 6 or may fail.
     */
    char cmd[4004];
    FILE *unzip_out = NULL;
    char buf[1001];
    float UnZip_Version = 0.0;

    cmd[0] = '\0';
    strncat(cmd, unzippath, 4000);
    strcat(cmd, " -v");

    if ((unzip_out = popen(cmd, "r")) == NULL) {
        perror("unzip pipe error");
    } else {
        if (fgets(buf, 1000, unzip_out) == NULL) {
            zipwarn("failed to get information from UnZip", "");
        } else {
            /* the first line should start with the version */
            if (sscanf(buf, "UnZip %f ", &UnZip_Version) < 1) {
                zipwarn("unexpected output of UnZip -v", "");
            } else {
                /* printf("UnZip %f\n", UnZip_Version); */

                while (fgets(buf, 1000, unzip_out)) {
                }
            }
        }
        pclose(unzip_out);
    }
    if (UnZip_Version < 6.0 && zip64_archive) {
        sprintf(buf, "Found UnZip version %4.2f", UnZip_Version);
        zipwarn(buf, "");
        zipwarn("Need UnZip 6.00 or later to test this Zip64 archive", "");
        return 0;
    }
    return 1;
}

local void check_zipfile(zipname, zippath)
    char *zipname;
    char *zippath;
    /* Invoke unzip -t on the given zip file */
{
    char *cmd;
    int result;

    /* Tell picky compilers to shut up about unused variables */
    zippath = zippath;

    if (unzip_path) {
        /* user gave us a path to some unzip (may not be UnZip) */
        char *here;
        int len;

        /* Replace first {} with archive name.  If no {} append name to string. */
        here = strstr(unzip_path, "{}");

        if ((cmd = malloc(strlen(unzip_path) + strlen(zipname) + 3)) == NULL) {
            ziperr(ZE_MEM, "building command string for testing archive");
        }

        if (here) {
            /* have {} so replace with temp name */
            len = here - unzip_path;
            strcpy(cmd, unzip_path);
            cmd[len] = '\0';
            strcat(cmd, " ");
            strcat(cmd, "'");    /* accept space or $ in name */
            strcat(cmd, zipname);
            strcat(cmd, "'");
            strcat(cmd, " ");
            strcat(cmd, here + 2);
        } else {
            /* No {} so append temp name to end */
            strcpy(cmd, unzip_path);
            strcat(cmd, " ");
            strcat(cmd, "'");    /* accept space or $ in name */
            strcat(cmd, zipname);
            strcat(cmd, "'");
        }
        free(unzip_path);
        unzip_path = NULL;

    } else {
        if ((cmd = malloc(20 + strlen(zipname))) == NULL) {
            ziperr(ZE_MEM, "building command string for testing archive");
        }

        strcpy(cmd, "unzip -t ");
        if (!verbose) strcat(cmd, "-qq ");
        if (check_unzip_version("unzip") == 0)
            ZIPERR(ZE_TEST, zipfile);

        strcat(cmd, "'");    /* accept space or $ in name */
        strcat(cmd, zipname);
        strcat(cmd, "'");
    }

    result = system(cmd);
    free(cmd);
    cmd = NULL;
    if (result) {

        fprintf(mesg, "test of %s FAILED\n", zipfile);
        ziperr(ZE_TEST, "original files unmodified");
    }
    if (noisy) {
        fprintf(mesg, "test of %s OK\n", zipfile);
        fflush(mesg);
    }
    if (logfile) {
        fprintf(logfile, "test of %s OK\n", zipfile);
        fflush(logfile);
    }
}

/* get_filters() is replaced by the following
   local int get_filters(argc, argv)
   */

/* The filter patterns for options -x, -i, and -R are
   returned by get_option() one at a time, so use a linked
   list to store until all args are processed.  Then convert
   to array for processing.
   */

/* convert list to patterns array */
local int filterlist_to_patterns()
{
    unsigned i;
    struct filterlist_struct *next = NULL;

    if (pcount == 0) {
        patterns = NULL;
        return 0;
    }
    if ((patterns = (struct plist *) malloc((pcount + 1) * sizeof(struct plist))) == NULL) {
        ZIPERR(ZE_MEM, "was creating pattern list");
    }

    for (i = 0; i < pcount && filterlist != NULL; i++) {
        switch (filterlist->flag) {
            case 'i':
                icount++;
                break;
            case 'R':
                Rcount++;
                break;
        }
        patterns[i].select = filterlist->flag;
        patterns[i].zname = filterlist->pattern;
        next = filterlist->next;
        free(filterlist);
        filterlist = next;
    }

    return pcount;
}


/* add a file argument to linked list */
local long add_name(filearg)
    char *filearg;
{
    char *name = NULL;
    struct filelist_struct *fileentry = NULL;

    if ((fileentry = (struct filelist_struct *) malloc(sizeof(struct filelist_struct))) == NULL) {
        ZIPERR(ZE_MEM, "adding file");
    }
    if ((name = malloc(strlen(filearg) + 1)) == NULL) {
        ZIPERR(ZE_MEM, "adding file");
    }
    strcpy(name, filearg);
    fileentry->next = NULL;
    fileentry->name = name;
    if (filelist == NULL) {
        /* first file argument */
        filelist = fileentry;         /* start of list */
        lastfile = fileentry;
    } else {
        lastfile->next = fileentry;   /* link to last filter in list */
        lastfile = fileentry;
    }
    filearg_count++;

    return filearg_count;
}


/* Running Stats
   10/30/04 */

local int DisplayRunningStats()
{
    char tempstrg[100];

    if (mesg_line_started) {
        fprintf(mesg, "\n");
        mesg_line_started = 0;
    }
    if (logfile_line_started) {
        fprintf(logfile, "\n");
        logfile_line_started = 0;
    }
    if (display_volume) {
        if (noisy) {
            fprintf(mesg, "%lu>%lu: ", current_in_disk + 1, current_disk + 1);
            mesg_line_started = 1;
        }
        if (logall) {
            fprintf(logfile, "%lu>%lu: ", current_in_disk + 1, current_disk + 1);
            logfile_line_started = 1;
        }
    }
    if (display_counts) {
        if (noisy) {
            fprintf(mesg, "%3ld/%3ld ", files_so_far, files_total - files_so_far);
            mesg_line_started = 1;
        }
        if (logall) {
            fprintf(logfile, "%3ld/%3ld ", files_so_far, files_total - files_so_far);
            logfile_line_started = 1;
        }
    }
    if (display_bytes) {
        /* since file sizes can change as we go, use bytes_so_far from
           initial scan so all adds up */
        WriteNumString(bytes_so_far, tempstrg);
        if (noisy) {
            fprintf(mesg, "[%4s", tempstrg);
            mesg_line_started = 1;
        }
        if (logall) {
            fprintf(logfile, "[%4s", tempstrg);
            logfile_line_started = 1;
        }
        if (bytes_total >= bytes_so_far) {
            WriteNumString(bytes_total - bytes_so_far, tempstrg);
            if (noisy)
                fprintf(mesg, "/%4s] ", tempstrg);
            if (logall)
                fprintf(logfile, "/%4s] ", tempstrg);
        } else {
            WriteNumString(bytes_so_far - bytes_total, tempstrg);
            if (noisy)
                fprintf(mesg, "-%4s] ", tempstrg);
            if (logall)
                fprintf(logfile, "-%4s] ", tempstrg);
        }
    }
    if (noisy)
        fflush(mesg);
    if (logall)
        fflush(logfile);

    return 0;
}

int encr_passwd(modeflag, pwbuf, size, zfn)
    int modeflag;
    char *pwbuf;
    int size;
    ZCONST char *zfn;
{
    char *prompt;

    /* Tell picky compilers to shut up about unused variables */
    zfn = zfn;

    prompt = (modeflag == ZP_PW_VERIFY) ?
        "Verify password: " : "Enter password: ";

    if (getp(prompt, pwbuf, size) == NULL) {
        ziperr(ZE_PARMS, "stderr is not a tty");
    }
    return IZ_PW_ENTERED;
}


/* rename a split
 * A split has a tempfile name until it is closed, then
 * here rename it as out_path the final name for the split.
 */
int rename_split(temp_name, out_path)
    char *temp_name;
    char *out_path;
{
    int r;
    /* Replace old zip file with new zip file, leaving only the new one */
    if ((r = replace(out_path, temp_name)) != ZE_OK)
    {
        zipwarn("new zip file left as: ", temp_name);
        free((zvoid *)tempzip);
        tempzip = NULL;
        ZIPERR(r, "was replacing split file");
    }
    if (zip_attributes) {
        setfileattr(out_path, zip_attributes);
    }
    return ZE_OK;
}


int set_filetype(char *out_path)
{
    return ZE_OK;
}


/*
   -------------------------------------------------------
   Command Line Options
   -------------------------------------------------------

   Valid command line options.

   The function get_option() uses this table to check if an
   option is valid and if it takes a value (also called an
   option argument).  To add an option to zip just add it
   to this table and add a case in the main switch to handle
   it.  If either shortopt or longopt not used set to "".

   The fields:
   shortopt     - short option name (1 or 2 chars)
   longopt      - long option name
   value_type   - see zip.h for constants
   negatable    - option is negatable with trailing -
   ID           - unsigned long int returned for option
   name         - short description of option which is
   returned on some errors and when options
   are listed with -so option, can be NULL
   */

/* Most option IDs are set to the shortopt char.  For
   multichar short options set to arbitrary unused constant. */
#define o_AC            0x101
#define o_AS            0x102
#define o_C2            0x103
#define o_C5            0x104
#define o_db            0x105
#define o_dc            0x106
#define o_dd            0x107
#define o_des           0x108
#define o_df            0x109
#define o_DF            0x110
#define o_dg            0x111
#define o_ds            0x112
#define o_du            0x113
#define o_dv            0x114
#define o_FF            0x115
#define o_FI            0x116
#define o_FS            0x117
#define o_h2            0x118
#define o_ic            0x119
#define o_jj            0x120
#define o_la            0x121
#define o_lf            0x122
#define o_li            0x123
#define o_ll            0x124
#define o_mm            0x125
#define o_MM            0x126
#define o_nw            0x127
#define o_RE            0x128
#define o_sb            0x129
#define o_sc            0x130
#define o_sd            0x131
#define o_sf            0x132
#define o_so            0x133
#define o_sp            0x134
#define o_su            0x135
#define o_sU            0x136
#define o_sv            0x137
#define o_tt            0x138
#define o_TT            0x139
#define o_UN            0x140
#define o_ve            0x141
#define o_VV            0x142
#define o_ws            0x143
#define o_ww            0x144
#define o_z64           0x145


/* the below is mainly from the old main command line
   switch with a few changes */
struct option_struct far options[] = {
    /* short longopt        value_type        negatable        ID    name */
    {"0",  "store",       o_NO_VALUE,       o_NOT_NEGATABLE, '0',  "store"},
    {"1",  "compress-1",  o_NO_VALUE,       o_NOT_NEGATABLE, '1',  "compress 1"},
    {"2",  "compress-2",  o_NO_VALUE,       o_NOT_NEGATABLE, '2',  "compress 2"},
    {"3",  "compress-3",  o_NO_VALUE,       o_NOT_NEGATABLE, '3',  "compress 3"},
    {"4",  "compress-4",  o_NO_VALUE,       o_NOT_NEGATABLE, '4',  "compress 4"},
    {"5",  "compress-5",  o_NO_VALUE,       o_NOT_NEGATABLE, '5',  "compress 5"},
    {"6",  "compress-6",  o_NO_VALUE,       o_NOT_NEGATABLE, '6',  "compress 6"},
    {"7",  "compress-7",  o_NO_VALUE,       o_NOT_NEGATABLE, '7',  "compress 7"},
    {"8",  "compress-8",  o_NO_VALUE,       o_NOT_NEGATABLE, '8',  "compress 8"},
    {"9",  "compress-9",  o_NO_VALUE,       o_NOT_NEGATABLE, '9',  "compress 9"},
    {"A",  "adjust-sfx",  o_NO_VALUE,       o_NOT_NEGATABLE, 'A',  "adjust self extractor offsets"},
    {"b",  "temp-path",   o_REQUIRED_VALUE, o_NOT_NEGATABLE, 'b',  "dir to use for temp archive"},
    {"c",  "entry-comments", o_NO_VALUE,    o_NOT_NEGATABLE, 'c',  "add comments for each entry"},
    {"d",  "delete",      o_NO_VALUE,       o_NOT_NEGATABLE, 'd',  "delete entries from archive"},
    {"db", "display-bytes", o_NO_VALUE,     o_NEGATABLE,     o_db, "display running bytes"},
    {"dc", "display-counts", o_NO_VALUE,    o_NEGATABLE,     o_dc, "display running file count"},
    {"dd", "display-dots", o_NO_VALUE,      o_NEGATABLE,     o_dd, "display dots as process each file"},
    {"dg", "display-globaldots",o_NO_VALUE, o_NEGATABLE,     o_dg, "display dots for archive instead of files"},
    {"ds", "dot-size",     o_REQUIRED_VALUE, o_NOT_NEGATABLE, o_ds, "set progress dot size - default 10M bytes"},
    {"du", "display-usize", o_NO_VALUE,     o_NEGATABLE,     o_du, "display uncompressed size in bytes"},
    {"dv", "display-volume", o_NO_VALUE,    o_NEGATABLE,     o_dv, "display volume (disk) number"},
    {"D",  "no-dir-entries", o_NO_VALUE,    o_NOT_NEGATABLE, 'D',  "no entries for dirs themselves (-x */)"},
    {"DF", "difference-archive",o_NO_VALUE, o_NOT_NEGATABLE, o_DF, "create diff archive with changed/new files"},
    {"e",  "encrypt",     o_NO_VALUE,       o_NOT_NEGATABLE, 'e',  "encrypt entries, ask for password"},
    {"F",  "fix",         o_NO_VALUE,       o_NOT_NEGATABLE, 'F',  "fix mostly intact archive (try first)"},
    {"FF", "fixfix",      o_NO_VALUE,       o_NOT_NEGATABLE, o_FF, "try harder to fix archive (not as reliable)"},
    {"FI", "fifo",        o_NO_VALUE,       o_NEGATABLE,     o_FI, "read Unix FIFO (zip will wait on open pipe)"},
    {"FS", "filesync",    o_NO_VALUE,       o_NOT_NEGATABLE, o_FS, "add/delete entries to make archive match OS"},
    {"f",  "freshen",     o_NO_VALUE,       o_NOT_NEGATABLE, 'f',  "freshen existing archive entries"},
    {"fd", "force-descriptors", o_NO_VALUE, o_NOT_NEGATABLE, o_des,"force data descriptors as if streaming"},
    {"fz", "force-zip64", o_NO_VALUE,       o_NEGATABLE,     o_z64,"force use of Zip64 format, negate prevents"},
    {"g",  "grow",        o_NO_VALUE,       o_NOT_NEGATABLE, 'g',  "grow existing archive instead of replace"},
    {"h",  "help",        o_NO_VALUE,       o_NOT_NEGATABLE, 'h',  "help"},
    {"H",  "",            o_NO_VALUE,       o_NOT_NEGATABLE, 'h',  "help"},
    {"?",  "",            o_NO_VALUE,       o_NOT_NEGATABLE, 'h',  "help"},
    {"h2", "more-help",   o_NO_VALUE,       o_NOT_NEGATABLE, o_h2, "extended help"},
    {"i",  "include",     o_VALUE_LIST,     o_NOT_NEGATABLE, 'i',  "include only files matching patterns"},
    {"j",  "junk-paths",  o_NO_VALUE,       o_NOT_NEGATABLE, 'j',  "strip paths and just store file names"},
    {"J",  "junk-sfx",    o_NO_VALUE,       o_NOT_NEGATABLE, 'J',  "strip self extractor from archive"},
    {"k",  "DOS-names",   o_NO_VALUE,       o_NOT_NEGATABLE, 'k',  "force use of 8.3 DOS names"},
    {"l",  "to-crlf",     o_NO_VALUE,       o_NOT_NEGATABLE, 'l',  "convert text file line ends - LF->CRLF"},
    {"ll", "from-crlf",   o_NO_VALUE,       o_NOT_NEGATABLE, o_ll, "convert text file line ends - CRLF->LF"},
    {"lf", "logfile-path",o_REQUIRED_VALUE, o_NOT_NEGATABLE, o_lf, "log to log file at path (default overwrite)"},
    {"la", "log-append",  o_NO_VALUE,       o_NEGATABLE,     o_la, "append to existing log file"},
    {"li", "log-info",    o_NO_VALUE,       o_NEGATABLE,     o_li, "include informational messages in log"},
    {"L",  "license",     o_NO_VALUE,       o_NOT_NEGATABLE, 'L',  "display license"},
    {"m",  "move",        o_NO_VALUE,       o_NOT_NEGATABLE, 'm',  "add files to archive then delete files"},
    {"mm", "",            o_NO_VALUE,       o_NOT_NEGATABLE, o_mm, "not used"},
    {"MM", "must-match",  o_NO_VALUE,       o_NOT_NEGATABLE, o_MM, "error if in file not matched/not readable"},
    {"n",  "suffixes",    o_REQUIRED_VALUE, o_NOT_NEGATABLE, 'n',  "suffixes to not compress: .gz:.zip"},
    {"nw", "no-wild",     o_NO_VALUE,       o_NOT_NEGATABLE, o_nw, "no wildcards during add or update"},
    {"o",  "latest-time", o_NO_VALUE,       o_NOT_NEGATABLE, 'o',  "use latest entry time as archive time"},
    {"O",  "output-file", o_REQUIRED_VALUE, o_NOT_NEGATABLE, 'O',  "set out zipfile different than in zipfile"},
    {"p",  "paths",       o_NO_VALUE,       o_NOT_NEGATABLE, 'p',  "store paths"},
    {"P",  "password",    o_REQUIRED_VALUE, o_NOT_NEGATABLE, 'P',  "encrypt entries, option value is password"},
    {"q",  "quiet",       o_NO_VALUE,       o_NOT_NEGATABLE, 'q',  "quiet"},
    {"r",  "recurse-paths", o_NO_VALUE,     o_NOT_NEGATABLE, 'r',  "recurse down listed paths"},
    {"R",  "recurse-patterns", o_NO_VALUE,  o_NOT_NEGATABLE, 'R',  "recurse current dir and match patterns"},
    {"RE", "regex",       o_NO_VALUE,       o_NOT_NEGATABLE, o_RE, "allow [list] matching (regex)"},
    {"s",  "split-size",  o_REQUIRED_VALUE, o_NOT_NEGATABLE, 's',  "do splits, set split size (-s=0 no splits)"},
    {"sp", "split-pause", o_NO_VALUE,       o_NOT_NEGATABLE, o_sp, "pause while splitting to select destination"},
    {"sv", "split-verbose", o_NO_VALUE,     o_NOT_NEGATABLE, o_sv, "be verbose about creating splits"},
    {"sb", "split-bell",  o_NO_VALUE,       o_NOT_NEGATABLE, o_sb, "when pause for next split ring bell"},
    {"sc", "show-command",o_NO_VALUE,       o_NOT_NEGATABLE, o_sc, "show command line"},
    {"sd", "show-debug",  o_NO_VALUE,       o_NOT_NEGATABLE, o_sd, "show debug"},
    {"sf", "show-files",  o_NO_VALUE,       o_NEGATABLE,     o_sf, "show files to operate on and exit"},
    {"so", "show-options",o_NO_VALUE,       o_NOT_NEGATABLE, o_so, "show options"},
    {"su", "show-unicode", o_NO_VALUE,      o_NEGATABLE,     o_su, "as -sf but also show escaped Unicode"},
    {"sU", "show-just-unicode", o_NO_VALUE, o_NEGATABLE,     o_sU, "as -sf but only show escaped Unicode"},
    {"t",  "from-date",   o_REQUIRED_VALUE, o_NOT_NEGATABLE, 't',  "exclude before date"},
    {"tt", "before-date", o_REQUIRED_VALUE, o_NOT_NEGATABLE, o_tt, "include before date"},
    {"T",  "test",        o_NO_VALUE,       o_NOT_NEGATABLE, 'T',  "test updates before replacing archive"},
    {"TT", "unzip-command", o_REQUIRED_VALUE,o_NOT_NEGATABLE,o_TT, "unzip command to use, name is added to end"},
    {"u",  "update",      o_NO_VALUE,       o_NOT_NEGATABLE, 'u',  "update existing entries and add new"},
    {"U",  "copy-entries", o_NO_VALUE,      o_NOT_NEGATABLE, 'U',  "select from archive instead of file system"},
    {"UN", "unicode",     o_REQUIRED_VALUE, o_NOT_NEGATABLE, o_UN, "UN=quit, warn, ignore, no, escape"},
    {"v",  "verbose",     o_NO_VALUE,       o_NOT_NEGATABLE, 'v',  "display additional information"},
    {"",   "version",     o_NO_VALUE,       o_NOT_NEGATABLE, o_ve, "(if no other args) show version information"},
    {"ws", "wild-stop-dirs", o_NO_VALUE,    o_NOT_NEGATABLE, o_ws,  "* stops at /, ** includes any /"},
    {"x",  "exclude",     o_VALUE_LIST,     o_NOT_NEGATABLE, 'x',  "exclude files matching patterns"},
    /*    {"X",  "no-extra",    o_NO_VALUE,       o_NOT_NEGATABLE, 'X',  "no extra"},
    */
    {"X",  "strip-extra", o_NO_VALUE,       o_NEGATABLE,     'X',  "-X- keep all ef, -X strip but critical ef"},
    {"y",  "symlinks",    o_NO_VALUE,       o_NOT_NEGATABLE, 'y',  "store symbolic links"},
    {"z",  "archive-comment", o_NO_VALUE,   o_NOT_NEGATABLE, 'z',  "ask for archive comment"},
    {"Z",  "compression-method", o_REQUIRED_VALUE, o_NOT_NEGATABLE, 'Z', "compression method"},
    {"@",  "names-stdin", o_NO_VALUE,       o_NOT_NEGATABLE, '@',  "get file names from stdin, one per line"},
    /* the end of the list */
    {NULL, NULL,          o_NO_VALUE,       o_NOT_NEGATABLE, 0,    NULL} /* end has option_ID = 0 */
};



int main(int argc, char **argv)
{
    int d;                /* true if just adding to a zip file */
    char *e;              /* malloc'd comment buffer */
    struct flist far *f;  /* steps through found linked list */
    int i;                /* arg counter, root directory flag */
    int kk;               /* next arg type (formerly another re-use of "k") */

    /* zip64 support 09/05/2003 R.Nausedat */
    uzoff_t c;            /* start of central directory */
    uzoff_t t;            /* length of central directory */
    zoff_t k;             /* marked counter, comment size, entry count */
    uzoff_t n;            /* total of entry len's */

    int o;                /* true if there were any ZE_OPEN errors */
    char *p;              /* steps through option arguments */
    char *pp;             /* temporary pointer */
    int r;                /* temporary variable */
    int s;                /* flag to read names from stdin */
    uzoff_t csize;        /* compressed file size for stats */
    uzoff_t usize;        /* uncompressed file size for stats */
    ulg tf;               /* file time */
    int first_listarg = 0;/* index of first arg of "process these files" list */
    struct zlist far *v;  /* temporary variable */
    struct zlist far * far *w;    /* pointer to last link in zfiles list */
    FILE *x /*, *y */;    /* input and output zip files (y global) */
    struct zlist far *z;  /* steps through zfiles linked list */
    int bad_open_is_error = 0; /* if read fails, 0=warning, 1=error */

    char *zipbuf;         /* stdio buffer for the zip file */
    FILE *comment_stream; /* set to stderr if anything is read from stdin */
    int all_current;      /* used by File Sync to determine if all entries are current */

    struct filelist_struct *filearg;

    /* used by get_option */
    unsigned long option; /* option ID returned by get_option */
    int argcnt = 0;       /* current argcnt in args */
    int argnum = 0;       /* arg number */
    int optchar = 0;      /* option state */
    char *value = NULL;   /* non-option arg, option value or NULL */
    int negated = 0;      /* 1 = option negated */
    int fna = 0;          /* current first non-opt arg */
    int optnum = 0;       /* index in table */

    int show_options = 0; /* show options */
    int show_what_doing = 0; /* show what doing */
    int show_args = 0;    /* show command line */
    int seen_doubledash = 0; /* seen -- argument */
    int key_needed = 0;   /* prompt for encryption key */
    int have_out = 0;     /* if set in_path and out_path different archive */

    char **args = NULL;  /* could be wide argv */


    SETLOCALE(LC_CTYPE, "");

    /* For Unix, set the locale to UTF-8.  Any UTF-8 locale is
       OK and they should all be the same.  This allows seeing,
       writing, and displaying (if the fonts are loaded) all
       characters in UTF-8. */
    {
        char *loc;

        loc = setlocale(LC_CTYPE, "en_US.UTF-8");

        if (loc != NULL) {
            /* using UTF-8 character set so can set UTF-8 GPBF bit 11 */
            using_utf8 = 1;
        } else {
        }
    }

    mesg = (FILE *) stdout; /* cannot be made at link time for VMS */
    comment_stream = (FILE *)stdin;

    init_upper();           /* build case map table */

    zipfile = tempzip = NULL;
    y = NULL;
    d = 0;                        /* disallow adding to a zip file */
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    signal(SIGABRT, handler);
    signal(SIGBUS, handler);
    signal(SIGILL, handler);
    signal(SIGSEGV, handler);

    /* make copy of args that can use with insert_arg() used by get_option() */
    args = copy_args(argv, 0);

    kk = 0;                       /* Next non-option argument type */

    argnum = 0;

    while ((option = get_option(&args, &argcnt, &argnum,
                    &optchar, &value, &negated,
                    &fna, &optnum, 0)))
    {
        switch (option)
        {
            case 'e':   /* Encrypt */
                if (key)
                    free(key);
                key_needed = 1;
                break;

            case o_NON_OPTION_ARG:
                if (recurse != 2 && kk == 0 && patterns == NULL) {
                    filterlist_to_patterns();
                }
                /* ignore only first -- */
                if (strcmp(value, "--") == 0 && seen_doubledash == 0) 
                {
                } 
                else if (kk == 6) 
                {
                } 
                else 
                    switch (kk)
                    {
                        case 0:
                            /* first non-option arg is zipfile name */
                            if (strcmp(value, "-") == 0) {  /* output zipfile is dash */
                            }
                            else
                            {
                                /* name of zipfile */
                                zipfile = ziptyp(value);
                                free(value);
                            }
                            /* if in_path not set, use zipfile path as usual for input */
                            /* in_path is used as the base path to find splits */
                            if (in_path == NULL) {
                                in_path = malloc(strlen(zipfile) + 1);
                                strcpy(in_path, zipfile);
                                printf("[Debug] in_path=[%s]\n", zipfile);
                            }
                            /* if out_path not set, use zipfile path as usual for output */
                            /* out_path is where the output archive is written */
                            if (out_path == NULL) {
                                out_path = malloc(strlen(zipfile) + 1);
                                strcpy(out_path, zipfile);
                                printf("[Debug] out_path=[%s]\n", zipfile);
                            }
                            kk = 3;
                            break;

                        case 3:  case 4:
                            /*循环把要压缩的文件，添加到链表filelist中，由filearg_count存储链表中文件的数量*/
                            /*kk=3只是为了记录第一个压缩文件在程序参数列表中的下标，下标由first_listarg*/
                            add_name(value);
                            //printf("[Debug] add_name(%s), argnum=[%d]\n", value, argnum);

                            if (kk == 3) {
                                first_listarg = argnum;
                                kk = 4;
                            }
                            break;

                    } /* switch kk */
                break;
        }  /* switch */
    }


    /* do processing of command line and one-time tasks */

    /* Key not yet specified.  If needed, get/verify it now. */
    /*这只是单纯的要求输入密码和确认密码信息，不涉及向加密文件中添加密码锁信息的部分*/
    if (key_needed) {
        key = malloc(IZ_PWLEN+1);
        r = encr_passwd(ZP_PW_ENTER, key, IZ_PWLEN+1, zipfile);
        if (r != IZ_PW_ENTERED) 
        {
            if (r < IZ_PW_ENTERED)
                r = ZE_PARMS;
            ZIPERR(r, "was getting encryption password");
        }
        //printf("encr_passwd=[%s]\n", key);
        if (*key == '\0') {
            ZIPERR(ZE_PARMS, "zero length password not allowed");
        }
        e = malloc(IZ_PWLEN+1);
        r = encr_passwd(ZP_PW_VERIFY, e, IZ_PWLEN+1, zipfile);
        if (r != IZ_PW_ENTERED && r != IZ_PW_SKIPVERIFY) 
        {
            free((zvoid *)e);
            if (r < ZE_OK) r = ZE_PARMS;
            ZIPERR(r, "was verifying encryption password");
        }
        //printf("encr_passwd=[%s]\n", e);
        r = ((r == IZ_PW_SKIPVERIFY) ? 0 : strcmp(key, e));
        free((zvoid *)e);
        if (r) {
            ZIPERR(ZE_PARMS, "password verification failed");
        }
    }


    /* only set logall if logfile open */
    logall = 0;

    /* so disk display starts at 1, will be updated when entries are read */
    current_in_disk = 0;

    struct filelist_struct *pstr = filelist;
    while( NULL != pstr )
    {
        //printf("pstr = [%s]\n", pstr->name);
        pstr = pstr->next;
    }

    /* Process file arguments from command line */
    if (filelist) 
    {
        if (action == ARCHIVE) 
        {
        } 
        else 
        {
            /* try find matching files on OS first then try find entries in archive */
            for (; filelist; ) 
            {
                r = PROCNAME(filelist->name);
                free(filelist->name);
                filearg = filelist;
                filelist = filelist->next;
                free(filearg);
            }
        }
    }
    pstr = filelist;
    while( NULL != pstr )
    {
        printf("pstr = [%s]\n", pstr->name);
        pstr = pstr->next;
    }


    k = 0;                        /* Initialize marked count */
    scan_started = 0;
    scan_count = 0;
    all_current = 1;

    scan_count = 0;
    scan_started = 0;
    for (f = found; f != NULL;) {
        tf = 0;
        if (action != DELETE && action != FRESHEN) {
            printf("---------------------------------\n");
            tf = filetime(f->name, (ulg *)NULL, (zoff_t *)&usize, NULL);
        }
        //printf("[Debug] f->name=[%s], size=[%d]\n", f->name, usize);
        /* ??? */
        files_total++;
        f->usize = 0;
        if (usize != (uzoff_t) -1 && usize != (uzoff_t) -2) {
            bytes_total += usize;
            f->usize = usize;
        }
        f = f->nxt;
    }

    d = (d && k == 0 && (zipbeg || zfiles != NULL)); /* d true if appending */

#if CRYPT
    /* Initialize the crc_32_tab pointer, when encryption was requested. */
    if (key != NULL) {
        crc_32_tab = get_crc_table();
    }
#endif /* CRYPT */

    x = NULL;
    tempzn = 0;
    {
        int yd;
        int i;

        /* create path by stripping name and appending template */
        if ((tempzip = malloc(strlen(zipfile) + 12)) == NULL) {
        }
        strcpy(tempzip, zipfile);
        //printf("[Debug] %d, tempzip=[%s]\n", __LINE__, tempzip);
        for(i = strlen(tempzip); i > 0; i--) {
            if (tempzip[i - 1] == '/')
                break;
        }
        tempzip[i] = '\0';
        strcat(tempzip, "aaXXXXXX");
        //printf("[Debug] %d, tempzip=[%s]\n", __LINE__, tempzip);

        yd = mkstemp(tempzip);
        y = fdopen(yd, FOPW_TMP);
    }

    zipbuf = (char *)malloc(ZBSZ);
    setvbuf(y, zipbuf, _IOFBF, ZBSZ);

    /* If not seekable set some flags 3/14/05 EG */
    output_seekable = 1;
    if (!is_seekable(y)) {
    }

    /* if archive exists, not streaming and not deleting or growing, copy
       any bytes at beginning */
    if (strcmp(zipfile, "-") != 0 && !d)  /* this must go *after* set[v]buf */
    {
        //printf("[Debug] %d, zipbeg=[%x]\n", __LINE__, zipbeg);
        /* copy anything before archive */
        if (in_file && zipbeg && (r = bfcopy(zipbeg)) != ZE_OK) {
        }
        tempzn = zipbeg;
    }

    o = 0;                                /* no ZE_OPEN errors yet */


    w = &zfiles;

    fprintf(stderr, "zip diagnostic: fcount=%u\n", (unsigned)fcount);
    for (f = found; f != NULL; f = fexpel(f))
    {
        uzoff_t len;
        /* add a new zfiles entry and set the name */
        z = (struct zlist far *)farmalloc(sizeof(struct zlist));
        z->nxt = NULL;
        z->name = f->name;
        f->name = NULL;
        z->uname = NULL;          /* UTF-8 name for extra field */
        z->zuname = NULL;         /* externalized UTF-8 name for matching */
        z->ouname = NULL;         /* display version of UTF-8 name with OEM */

        //printf("[Debug] [%d] z->name=[%s]\n", __LINE__, z->name);

        /* Only set z->uname if have a non-ASCII Unicode name */
        /* The Unicode path extra field is created if z->uname is not NULL,
           unless on a UTF-8 system, then instead of creating the extra field
           set bit 11 in the General Purpose Bit Flag */
        {
            int is_ascii = 0;

            is_ascii = is_ascii_string(f->uname);

            if (z->uname == NULL) {
                if (!is_ascii)
                    z->uname = f->uname;
                else
                    free(f->uname);
            }
        }
        f->uname = NULL;

        z->iname = f->iname;
        f->iname = NULL;
        z->zname = f->zname;
        f->zname = NULL;
        z->oname = f->oname;
        f->oname = NULL;
        z->ext = z->cext = z->com = 0;
        z->extra = z->cextra = NULL;
        z->mark = 1;
        z->dosflag = f->dosflag;
        //printf("[Debug] [%d] z->iname=[%s]\n", __LINE__, z->iname);
        //printf("[Debug] [%d] z->zname=[%s]\n", __LINE__, z->zname);
        //printf("[Debug] [%d] z->oname=[%s]\n", __LINE__, z->oname);

        /* zip it up */
        DisplayRunningStats();
        if (noisy)
        {
            fprintf(mesg, "  adding: %s", z->oname);
            mesg_line_started = 1;
            fflush(mesg);
        }
        /* initial scan */
        len = f->usize;
        if ((r = zipup(z)) != ZE_OK  && r != ZE_OPEN && r != ZE_MISS)
        {
        }
        if (r == ZE_OPEN || r == ZE_MISS)
        {
        }
        else
        {
            files_so_far++;
            /* current size of file (just before reading) */
            good_bytes_so_far += z->len;
            /* size of file on initial scan */
            bytes_so_far += len;
            *w = z;
            w = &z->nxt;
            zcount++;
        }
    }
    if (key != NULL)
    {
        free((zvoid *)key);
        key = NULL;
    }

    if (comadd)
    {
        e = malloc(MAXCOM + 1);
        for (z = zfiles; z != NULL; z = z->nxt)
            if (z->mark)
            {
                if (noisy)
                    fprintf(mesg, "Enter comment for %s:\n", z->oname);
                if (fgets(e, MAXCOM+1, comment_stream) != NULL)
                {
                    if ((p = malloc((extent)(k = strlen(e))+1)) == NULL)
                    {
                        free((zvoid *)e);
                        ZIPERR(ZE_MEM, "was reading comment lines");
                    }
                    strcpy(p, e);
                    if (p[k-1] == '\n')
                        p[--k] = 0;
                    z->comment = p;
                    /* zip64 support 09/05/2003 R.Nausedat */
                    z->com = (extent)k;
                }
            }
    }

    diag("writing central directory");
    k = 0;                        /* keep count for end header */
    c = tempzn;                   /* get start of central */
    n = t = 0;
    for (z = zfiles; z != NULL; z = z->nxt)
    {
        if (z->mark || !(diff_mode || filesync)) {
            if ((r = putcentral(z)) != ZE_OK) {
                ZIPERR(r, tempzip);
            }
            tempzn += 4 + CENHEAD + z->nam + z->cext + z->com;
            n += z->len;
            t += z->siz;
            k++;
        }
    }

    if (k == 0)
        zipwarn("zip file empty", "");
    t = tempzn - c;               /* compute length of central */
    diag("writing end of central directory");

    if ((r = putend(k, t, c, zcomlen, zcomment)) != ZE_OK) {
        ZIPERR(r, tempzip);
    }

    if (fclose(y)) {
        ZIPERR(d ? ZE_WRITE : ZE_TEMP, tempzip);
    }
    y = NULL;
    /* Free some memory before spawning unzip */
    lm_free();
    /* Test new zip file before overwriting old one or removing input files */
    if (test)
        check_zipfile(tempzip, argv[0]);
    /* Replace old zip file with new zip file, leaving only the new one */
    if (strcmp(zipfile, "-") && !d)
    {
        diag("replacing old zip file with new zip file");
        if ((r = replace(out_path, tempzip)) != ZE_OK)
        {
            zipwarn("new zip file left as: ", tempzip);
            free((zvoid *)tempzip);
            tempzip = NULL;
            ZIPERR(r, "was replacing the original zip file");
        }
        free((zvoid *)tempzip);
    }
    tempzip = NULL;
    if (zip_attributes && strcmp(zipfile, "-")) {
        setfileattr(out_path, zip_attributes);
    }
    if (strcmp(zipfile, "-")) {
        set_filetype(out_path);
    }


    free((zvoid *) zipbuf);
    RETURN(finish(o ? ZE_OPEN : ZE_OK));
}
