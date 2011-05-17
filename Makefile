all: clean src

src:
	gcc -Wall -lxosd sublim.c -o sublim

clean:
	rm -f sublim

install: src
	cp sublim /usr/bin/asublim

