CFLAGS ?= -Wall -O2
LDFLAGS ?= -Wall -O2

INSTALL = install -D
INSTALL_PROGRAM = $(INSTALL) -m 755

LDLIBS = -lX11
OBJS = desktoggle.o

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

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
