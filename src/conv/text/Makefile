PREFIX=/usr

all:
	g++ main.cpp TextListener.cpp -I$(PREFIX)/include -L$(PREFIX)/lib `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -lwpd2 -o wpd2text

clean:
	rm -rf wpd2text