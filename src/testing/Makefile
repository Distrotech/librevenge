PREFIX=/opt

all:
	g++ main.cpp TestListener.cpp -I$(PREFIX)/include -L$(PREFIX)/lib `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -lwpd2 -o libwpd2test
