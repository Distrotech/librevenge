all:
	g++ WPXParser.cpp WPXHeader.cpp WP6Parser.cpp WP6Header.cpp `glib-config --cflags` -shared -o libwpd2.so
	cp -f ./libwpd2.so /usr/lib
	cp -f {libwpd2.h,WPXParser.h,WPXHeader.h,WP6Parser.h,WP6Header.h} /usr/include/libwpd2

clean:
	rm -rf ./libwpd2.so
