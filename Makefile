all: clean src

src:
	gcc -Wall -fstack-protector-all -lxosd -lX11 -lXrandr sublim.c -o sublim

clean:
	rm -f sublim

install: src
	cp sublim /usr/bin/asublim
	cp sublimWrap /usr/bin/

