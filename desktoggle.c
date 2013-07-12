/* Copyright (C) 2013 by Raphael Scholer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define BUG_TRACKER "https://github.com/rscholer/desktoggle/issues\0"
#define MAINTAINER "Raphael Scholer\0"
#define PROJECT "desktoggle\0"
#define SHORT_DESC "Hide all windows and show the desktop.\0"
#define VERSION "1.2\0"

void output_help(char name[]);
void output_version(void);

int main(int argc, char *argv[]) {
	/**************************************************************************
	 * Declare and Initialize Variables
	 *************************************************************************/
	Display *d;
	Window root;
	Atom _NET_SHOWING_DESKTOP;
	Atom actual_type;
	int actual_format;
	int error;
	int current = False;
	unsigned long nitems;
	unsigned long after;
	unsigned char *data = NULL;

	/* Variables for parsing arguments */
	int current_option = 0;
	int option_index = 0;

	const struct option long_options[] = {
		{"help",	no_argument,	0,		'h'},
		{"version",	no_argument,	0,		'v'},
		{0,			0,				0,		0}
	};

	/**************************************************************************
	 * Parse arguments
	 *************************************************************************/
	while (1) {
		current_option = getopt_long(argc, argv, "", long_options,
										&option_index);

		if (current_option == -1) {
			break;
		}
		switch (current_option) {
			case 'h':
				output_help(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				output_version();
				exit(EXIT_SUCCESS);
				break;
			case '?':
				printf("Try \'%s --help\' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
			default:
				abort();
				break;
		}
	}

	/* List all unknown non-option arguments and exit*/
	if (optind < argc) {
		if (argc - optind == 1) {
			printf("Unknown non-option argument:\n");
		}
		else {
			printf("Unknown non-option arguments:\n");
		}
		while (optind < argc) {
			printf("%s\n", argv[optind++]);
		}
		printf("Try \'%s --help\' for more information.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/**************************************************************************
	 *  Main program
	 *************************************************************************/

	/* Open the default display */
	if (!(d = XOpenDisplay(NULL))) {
		fprintf(stderr, "Cannot open display \"%s\".\n", XDisplayName(NULL));
		exit(EXIT_FAILURE);
	}

	/* This is the default root window */
	root = DefaultRootWindow(d);

	/* find the Atom for _NET_SHOWING_DESKTOP */
	_NET_SHOWING_DESKTOP = XInternAtom(d, "_NET_SHOWING_DESKTOP", False);

	/* Obtain the current state of _NET_SHOWING_DESKTOP  */
	error = XGetWindowProperty(d, root, _NET_SHOWING_DESKTOP, 0, 1, False,
								XA_CARDINAL, &actual_type, &actual_format,
								&nitems, &after, &data);

	/* Check for errors */
	if (error != Success) {
		fprintf(stderr, "Received error %d!\n", error);
		XCloseDisplay(d);
		exit(EXIT_FAILURE);
	}

	/* Set current state of desktop */
	if (nitems == 0) {
		/* current is alredy set to False */
		fprintf(stderr, "Unexpected result.\n");
		fprintf(stderr, "Assuming unshown desktop!\n");
	}
	else if (data != NULL) {
		current = data[0];
		XFree(data);
		data = NULL;
	}

	/* Initialize Xevent struct */
	XEvent xev = {
		.xclient = {
			.type = ClientMessage,
			.send_event = True,
			.display = d,
			.window = root,
			.message_type = _NET_SHOWING_DESKTOP,
			.format = 32,
			.data.l[0] = !current /* <-- Toggles state */
		}
	};

	/* Send the event to the window manager */
	XSendEvent(d, root, False,
				SubstructureRedirectMask | SubstructureNotifyMask, &xev);

	XCloseDisplay(d);
	exit(EXIT_SUCCESS);
}

void output_help(char name[]) {
	printf("Usage: %s [OPTION]...\n", name);
	printf("\n%s\n", SHORT_DESC);
	printf("\nOptions:\n");
	printf("  --help\t\tshow this help message and exit\n");
	printf("  --version\t\tshow version information and exit\n");
	printf("\nPlease report %s bugs at <%s>.\n",PROJECT ,BUG_TRACKER);
}

void output_version(void) {
	printf("%s %s\n", PROJECT, VERSION);
	printf("Copyright (C) 2013 %s\n", MAINTAINER);
	printf("\nPermission is hereby granted, free of charge, to any person ");
	printf("obtaining a copy\n");
	printf("of this software and associated documentation files ");
	printf("(the \"Software\"), to deal\n");
	printf("in the Software without restriction, including without ");
	printf("limitation the rights\n");
	printf("to use, copy, modify, merge, publish, distribute, sublicense, ");
	printf("and/or sell\n");
	printf("copies of the Software, and to permit persons to whom the ");
	printf("Software is\n");
	printf("furnished to do so, subject to the following conditions:\n");
	printf("\nThe above copyright notice and this permission notice shall be ");
	printf("included in\n");
	printf("all copies or substantial portions of the Software.\n");
	printf("\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ");
	printf("ANY KIND, EXPRESS OR\n");
	printf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES ");
	printf("OF MERCHANTABILITY,\n");
	printf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. ");
	printf("IN NO EVENT SHALL THE\n");
	printf("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,");
	printf("DAMAGES OR OTHER\n");
	printf("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR ");
	printf("OTHERWISE, ARISING FROM,\n");
	printf("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR ");
	printf("OTHER DEALINGS IN\n");
	printf("THE SOFTWARE.\n");
}
