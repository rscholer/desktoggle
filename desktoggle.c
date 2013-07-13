/* Copyright (C) 2013 by Raphael Scholer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTHOR "Raphael Scholer"
#define CDATE "2013"
#define PACKAGE_BUGREPORT "https://github.com/rscholer/desktoggle/issues/"
#define PACKAGE_NAME "desktoggle"
#define PACKAGE_NAME_S "Desktoggle"
#define PACKAGE_URL "https://github.com/rscholer/desktoggle/"
#define VERSION "1.2"

const char *PROGRAM_NAME = NULL;

void output_help(void);
void output_try_help(void);
void output_version(void);
void set_program_name(const char *name);

int main(int argc, char *argv[]) {
	set_program_name(argv[0]);
	/**************************************************************************
	 * Declare and Initialize Variables
	 *************************************************************************/
	Display *display;
	Window root;
	Atom _NET_SHOWING_DESKTOP;
	Atom actual_type;
	int actual_format;
	int error;
	int current = False;
	unsigned long nitems;
	unsigned long after;
	unsigned char *data = NULL;
	XEvent xev = {
		.xclient = {
			.type = ClientMessage,
			.send_event = True,
			.display = NULL,
			.window = 0,
			.message_type = 0,
			.format = 32,
			.data.l[0] = 0
		}
	};
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
				output_help();
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				output_version();
				exit(EXIT_SUCCESS);
				break;
			case '?':
				output_try_help();
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
			fputs("Unknown non-option argument:\n", stderr);
		}
		else {
			fputs("Unknown non-option arguments:\n", stderr);
		}
		while (optind < argc) {
			fprintf(stderr, "  %s\n", argv[optind++]);
		}
		output_try_help();
		exit(EXIT_FAILURE);
	}

	/**************************************************************************
	 *  Main program
	 *************************************************************************/

	/* Open the default display */
	if (!(display = XOpenDisplay(NULL))) {
		fprintf(stderr, "Cannot open display \"%s\".\n", XDisplayName(NULL));
		exit(EXIT_FAILURE);
	}

	/* This is the default root window */
	root = DefaultRootWindow(display);

	/* find the Atom for _NET_SHOWING_DESKTOP */
	_NET_SHOWING_DESKTOP = XInternAtom(display, "_NET_SHOWING_DESKTOP", False);

	/* Obtain the current state of _NET_SHOWING_DESKTOP  */
	error = XGetWindowProperty(display, root, _NET_SHOWING_DESKTOP, 0, 1, False,
								XA_CARDINAL, &actual_type, &actual_format,
								&nitems, &after, &data);

	/* Check for errors */
	if (error != Success) {
		fprintf(stderr, "Received error %d!\n", error);
		XCloseDisplay(display);
		exit(EXIT_FAILURE);
	}

	/* Set current state of desktop */
	if (nitems == 0) {
		/* current is alredy set to False */
		fputs("Unexpected result.\n", stderr);
		fputs("Assuming unshown desktop!\n", stderr);
	}
	else if (data != NULL) {
		current = data[0];
		XFree(data);
		data = NULL;
	}

	/* Populate xev struct with current data */
	xev.xclient.data.l[0] = !current;
	xev.xclient.display = display;
	xev.xclient.message_type = _NET_SHOWING_DESKTOP;
	xev.xclient.window = root;

	/* Send the event to the window manager */
	XSendEvent(display, root, False,
				SubstructureRedirectMask | SubstructureNotifyMask, &xev);

	XCloseDisplay(display);
	exit(EXIT_SUCCESS);
}

void output_help(void) {
	printf("Usage: %s [OPTION]...\n", PROGRAM_NAME);
	puts("Hide all windows and show the desktop.\n");
	puts("      --help     display this help and exit");
	puts("      --version  output version information and exit");
	printf("\nReport %s bugs at <%s>\n", PACKAGE_NAME, PACKAGE_BUGREPORT);
	printf("%s home page: <%s>\n", PACKAGE_NAME_S, PACKAGE_URL);
}

void output_try_help(void) {
	fprintf(stderr, "Try \'%s --help\' for more information.\n", PROGRAM_NAME);
}

void output_version(void) {
	printf("%s %s\n", PACKAGE_NAME, VERSION);
	printf("Copyright (C) %s %s\n", CDATE, AUTHOR);
	puts("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
	puts("This is free software: you are free to change and redistribute it.");
	puts("There is NO WARRANTY, to the extent permitted by law.");
	printf("\nWritten by %s.\n", AUTHOR);
}

void set_program_name(const char *name) {
	PROGRAM_NAME = strdup(name);
}
