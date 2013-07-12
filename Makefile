CFLAGS ?= -Wall -O2
LDFLAGS ?= -Wall -O2

INSTALL = install -D
INSTALL_PROGRAM = $(INSTALL) -m 755
INSTALL_DATA = $(INSTALL) -m 644

LDLIBS = -lX11
OBJS = desktoggle.o

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
docdir = $(datarootdir)/doc/desktoggle

.PHONY: all clean distclean install rebuild

all: desktoggle
distclean: clean
rebuild: clean all

desktoggle: $(OBJS)
	$(CC) $(LDLIBS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) desktoggle
	$(RM) $(OBJS)

install:
	$(INSTALL_PROGRAM) desktoggle $(DESTDIR)$(bindir)/desktoggle
	$(INSTALL_DATA) COPYING $(DESTDIR)$(docdir)/COPYING
	$(INSTALL_DATA) README $(DESTDIR)$(docdir)/README
