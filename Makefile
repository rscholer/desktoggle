CFLAGS ?= -Wall -O2
LDFLAGS ?= -Wall -O2

HELP2MAN = help2man --no-info
INSTALL = install -D
INSTALL_PROGRAM = $(INSTALL) -m 755
INSTALL_DATA = $(INSTALL) -m 644

LDLIBS = -lX11
OBJS = desktoggle.o
PROGS = desktoggle

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
docdir = $(datarootdir)/doc/desktoggle
mandir = $(datarootdir)/man
man1dir = $(datarootdir)/man1

man1ext = .1

.PHONY: all clean distclean install

all: desktoggle man1
distclean: clean

desktoggle: $(OBJS)
	$(CC) $(LDLIBS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

man1: $(PROGS)
	$(HELP2MAN) --output=$<$(man1ext) ./$<

clean:
	$(RM) desktoggle
	$(RM) $(OBJS)
	$(RM) *$(man1ext)

install:
	$(INSTALL_PROGRAM) desktoggle $(DESTDIR)$(bindir)/desktoggle
	$(INSTALL_DATA) COPYING $(DESTDIR)$(docdir)/COPYING
	$(INSTALL_DATA) README $(DESTDIR)$(docdir)/README
	$(INSTALL_DATA) desktoggle$(man1ext) $(DESTDIR)$(man1dir)/desktoggle$(man1ext)
