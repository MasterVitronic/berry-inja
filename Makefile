PREFIX  = /usr/local
LIBDIR  = $(PREFIX)/lib/berry/packages

CPPFLAGS= -fPIC -Wall -I/usr/include/ -shared -std=c++11 -O2

inja.so:
	g++ be-inja.cpp $(CPPFLAGS) -o inja.so

install:
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp inja.so $(DESTDIR)$(LIBDIR)

docs: inja.so docs/config.ld
	ldoc -c docs/config.ld -d html -a .
	mkdir -p docs/html/manual/docs
	cp docs/berry.svg docs/html/manual/docs

clean:
	rm -rf *.o *.so

.PHONY: inja.so
