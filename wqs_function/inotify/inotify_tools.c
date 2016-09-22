#include <stdlib.h>
#include <stdio.h>

#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <sys/inotify.h>

enum {
    ACTION_NULL_WD,
    ACTION_ADD_WD,
    ACTION_DEL_WD,
};

int main(int argc, const char **argv)
{
    int err = 0;

    if (!inotifytools_initialize()) {
        printf("inotifytools_initialize failedn");
        goto error;
    }

    inotifytools_initialize_stats();

    const char *monitor_path = ".";
    if (argc > 1) {
        monitor_path = argv[1];
    }
    printf("Monitor dir(%s)n", monitor_path);

    if (!inotifytools_watch_recursively(monitor_path, IN_ALL_EVENTS)) {
        printf("inotifytools_watch_recursively failedn");
        goto error;
    }

    inotifytools_set_printf_timefmt("%F %T");

    struct inotify_event * event = inotifytools_next_event(-1);
    char path[256];
    while (event) {
        inotifytools_printf( event, "%T %w%f %en" );

        if (IN_ISDIR&event->mask) {
            int action = ACTION_NULL_WD;

            if ((IN_DELETE|IN_DELETE_SELF|IN_MOVED_FROM)&event->mask) {
                action = ACTION_DEL_WD;
                snprintf(path, sizeof(path), "%s%s",
                        inotifytools_filename_from_wd(event->wd),
                        event->name);
                printf("Remove path(%s) from wdn", path);
            } else if (((IN_CREATE|IN_MOVED_TO)&event->mask) && (IN_ISDIR&event->mask)) {
                action = ACTION_ADD_WD;
                snprintf(path, sizeof(path), "%s%s",
                        inotifytools_filename_from_wd(event->wd),
                        event->name);
                printf("Add path(%s) into wdn", path);
            }

            if (ACTION_ADD_WD == action) {
                if (!inotifytools_watch_recursively(path, IN_ALL_EVENTS)) {
                    printf("inotifytools_watch_recursively failedn");
                    goto error;
                }
            } else if (ACTION_DEL_WD == action) {
                if (!inotifytools_remove_watch_by_wd(event->wd)) {
                    printf("inotifytools_remove_watch_by_wd failed. event->wd(%d)n", event->wd);
                    goto error;
                }
            }
        }
        event = inotifytools_next_event(-1);
    }

    printf("Exitn");

    return 0;

error:
    err = inotifytools_error();
    printf("Error(%d)n", err);
    return -1;
}

