/* Copyright (c) 2011-2018 by Raphael Scholer

This file is part of desktoggle.

desktoggle is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

desktoggle is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with desktoggle.  If not, see <https://www.gnu.org/licenses/>. */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "desktoggle.h"


static void output_help(const char *name);
static void output_try_help(const char *name);
static void output_version(void);


int main(int argc, char *argv[]) {
    /* Variables for X handling */
    Atom _NET_SHOWING_DESKTOP;
    Atom actual_type;
    Display *display;
    Window root;
    XEvent xev = {
        .xclient = {
            .type = ClientMessage,
            .send_event = True,
            .display = NULL,
            .window = 0,
            .message_type = 0,
            .format = 32,
            .data.l[0] = False
        }
    };
    int actual_format;
    int error;
    unsigned char *data = NULL;
    unsigned long after;
    unsigned long nitems;

    /* Variables for parsing of arguments */
    int current_option = 0;
    const struct option long_options[] = {
        {"help",    no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0,         0,           0, 0}
    };

    /* Parse arguments */
    while (current_option != -1) {
        current_option = getopt_long(argc, argv, "", long_options,
                                     &optind);

        switch (current_option) {
            case 'h':
                output_help(argv[0]);
                exit(EXIT_SUCCESS);
            case 'v':
                output_version();
                exit(EXIT_SUCCESS);
            case '?':
                output_try_help(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    /* List all unknown non-option arguments and exit */
    if (optind < argc) {
        fprintf(stderr, "%s: unrecognized argument '%s'\n",
                argv[0], argv[optind]);

        output_try_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Open the default display */
    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "%s: Cannot open display \"%s\".\n", argv[0], XDisplayName(NULL));
        exit(EXIT_FAILURE);
    }

    /* This is the default root window */
    root = DefaultRootWindow(display);

    /* Find the Atom for _NET_SHOWING_DESKTOP */
    _NET_SHOWING_DESKTOP = XInternAtom(display, "_NET_SHOWING_DESKTOP", False);

    /* Obtain the current state of _NET_SHOWING_DESKTOP  */
    error = XGetWindowProperty(display, root, _NET_SHOWING_DESKTOP, 0, 1,
                               False, XA_CARDINAL, &actual_type,
                               &actual_format, &nitems, &after, &data);

    if (error != Success) {
        fprintf(stderr, "%s: Received error %d!\n", argv[0], error);
    }
    else {
        /* Set current state of desktop */
        if (nitems == 0) {
            /* State is alredy set to False */
            fputs("Unexpected result.\n", stderr);
            fputs("Assuming unshown desktop!\n", stderr);
        }
        else if (data != NULL) {
            xev.xclient.data.l[0] = !data[0];
            xev.xclient.display = display;
            xev.xclient.message_type = _NET_SHOWING_DESKTOP;
            xev.xclient.window = root;

            /* Send the event to the window manager */
            XSendEvent(display, root, False,
                       SubstructureRedirectMask | SubstructureNotifyMask, &xev);

            XFree(data);
            data = NULL;
        }
    }

    XCloseDisplay(display);
    exit((!error) ? EXIT_SUCCESS : EXIT_FAILURE);
}


static void output_help(const char *name) {
    printf("usage: %s [OPTION]...\n\n", name);
    puts("Hide all windows and show the desktop.\n");
    puts("optional arguments:");
    puts("  --help     display this help and exit");
    puts("  --version  output version information and exit");
}


static void output_try_help(const char *name) {
    fprintf(stderr, "Try \'%s --help\' for more information.\n", name);
}


static void output_version(void) {
    printf("desktoggle %s\n", PROJECT_VERSION);
    puts("Copyright (c) 2011-2018 Raphael Scholer");
    puts("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
    puts("This is free software: you are free to change and redistribute it.");
    puts("There is NO WARRANTY, to the extent permitted by law.");
}

/* vim: set et sts=4 sw=4 ts=4 : */
