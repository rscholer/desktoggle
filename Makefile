CFLAGS ?= -Wall -O2

LDLIBS = -lX11
PREFIX = /usr/local
OBJS = desktoggle.o

.PHONY: all clean distclean install rebuild

all: desktoggle
distclean: clean


desktoggle: $(OBJS)
	$(CC) $(LDLIBS) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) desktoggle
	$(RM) $(OBJS)

install:
	install -D -m 755 desktoggle $(DESTDIR)/$(PREFIX)/bin/desktoggle
