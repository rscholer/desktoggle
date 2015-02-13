# Files to process
MAN1S = $(patsubst %.1.asciidoc, %.1, $(wildcard *.1.asciidoc))
OBJS = $(patsubst %.c, %.o, $(wildcard *.c))
PKGS = x11

# Set compiler/linker options
CFLAGS ?= -std=c99 -Werror -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wshadow \
          -Wstrict-prototypes -Wwrite-strings -Wswitch-default -Wconversion \
          -Wunreachable-code -Wcast-qual -Winit-self -Wuninitialized -O2 -g
CFLAGS += $(shell pkg-config --cflags $(PKGS))

LDFLAGS ?= $(CFLAGS)
LDLIBS := $(shell pkg-config --libs $(PKGS))

# Set programs
INSTALL = install -p
INSTALL_DIR = $(INSTALL) -m 755
INSTALL_PROGRAM = $(INSTALL) -m 755
INSTALL_DATA = $(INSTALL) -m 644

# Set directories
prefix ?= /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1

man1ext = .1

# Targets
.PHONY: all clean install manpages rebuild uninstall

%.1: %.1.asciidoc
	a2x --doctype manpage --format manpage $<
	sed -i '/\.\\\".*$$/d' $@
	@echo

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo

all: desktoggle manpages


desktoggle: $(OBJS)
	$(CC) $(LDLIBS) $(LDFLAGS) -o $@ $^
	@echo

clean:
	$(RM) desktoggle
	$(RM) $(OBJS)
	$(RM) $(MAN1S)

install:
	$(INSTALL_DIR) -d $(DESTDIR)$(bindir)
	$(INSTALL_DIR) -d $(DESTDIR)$(man1dir)
	$(INSTALL_PROGRAM) -t $(DESTDIR)$(bindir) desktoggle
	$(INSTALL_DATA) -t $(DESTDIR)$(man1dir) desktoggle$(man1ext)

manpages: $(MAN1S)

rebuild: clean all

uninstall:
	$(RM) $(DESTDIR)$(bindir)/desktoggle
	$(RM) $(DESTDIR)$(man1ext)/desktoggle$(man1ext)
