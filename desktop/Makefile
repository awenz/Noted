CFLAGS := -Wall -Wextra -pedantic -O2 -g -D_REENTRANT $(CFLAGS)
LDFLAGS := -lsqlite3 -lm $(LDFLAGS)

noted: noted.o

install:noted 
	mkdir -p $(DESTDIR)/usr/bin
	install -m755 noted $(DESTDIR)/usr/bin/noted

clean:
	$(RM) noted noted.o 
