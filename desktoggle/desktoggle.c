/* Copyright (C) 2011 by Raphael Scholer

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

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
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
