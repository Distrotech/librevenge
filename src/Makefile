PREFIX=/opt

all:
	g++ WPXParser.cpp WPXHeader.cpp WP6Parser.cpp WP6Header.cpp WPXPart.cpp WP6Part.cpp WP6VariableLengthGroup.cpp `glib-config --cflags` -shared -o libwpd2.so
	

install:	
	mkdir -p $(PREFIX)/include/libwpd2
	cp -f ./libwpd2.so $(PREFIX)/lib
	cp -f {libwpd2.h,WPXParser.h,WPXHeader.h,WP6Parser.h,WP6Header.h,WPXLLListener.h,WPXHLListener.h} $(PREFIX)/include/libwpd2

clean:
	rm -rf ./libwpd2.so
