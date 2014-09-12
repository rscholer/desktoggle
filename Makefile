# Files to process
OBJS = $(patsubst %.c, %.o, $(wildcard *.c))
PKGS = x11

# Set compiler/linker options
CFLAGS ?= -Wall -Wextra -pedantic -Wmissing-prototypes -O2 -g
CFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS ?= $(CFLAGS)
LDLIBS := $(shell pkg-config --libs $(PKGS))

# Set programs
CC := $(CC) -std=c99
INSTALL = install -pD
INSTALL_PROGRAM = $(INSTALL) -m 755
INSTALL_DATA = $(INSTALL) -m 644

# Set directories
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
docdir = $(datarootdir)/doc/desktoggle
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1

man1ext = .1

# Targets
.PHONY: all clean install rebuild uninstall

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo

all: desktoggle

desktoggle: $(OBJS)
	$(CC) $(LDLIBS) $(LDFLAGS) -o $@ $^
	@echo

clean:
	$(RM) desktoggle
	$(RM) $(OBJS)

install:
	$(INSTALL_PROGRAM) desktoggle $(DESTDIR)$(bindir)/desktoggle
	$(INSTALL_DATA) COPYING $(DESTDIR)$(docdir)/COPYING
	$(INSTALL_DATA) README $(DESTDIR)$(docdir)/README
	$(INSTALL_DATA) desktoggle$(man1ext) $(DESTDIR)$(man1dir)/desktoggle$(man1ext)

rebuild: clean all

uninstall:
	$(RM) $(DESTDIR)$(bindir)/desktoggle
	$(RM) $(DESTDIR)$(docdir)/COPYING
	$(RM) $(DESTDIR)$(docdir)/README
	$(RM) $(DESTDIR)$(man1dir)/desktoggle$(man1ext)
