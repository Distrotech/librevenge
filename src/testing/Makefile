PREFIX=/opt

all:
	g++ main.cpp TestListener.cpp -I$(PREFIX)/include -L$(PREFIX)/lib `glib-config --cflags` -lwpd2 -o libwpd2test
