PREFIX=/usr
VERSION=0.1.0

all:
	g++ WPXParser.cpp \
	    WPXHeader.cpp \
		WP6Parser.cpp \
		WP6Header.cpp \
		WPXPart.cpp \
		WP6Part.cpp \
		WP6FixedLengthGroup.cpp \
		WP6FixedSpaceGroup.cpp \
		WP6FixedEOLGroup.cpp \
		WP6ExtendedCharacterGroup.cpp \
		WP6UndoGroup.cpp \
		WP6AttributeOnGroup.cpp \
		WP6AttributeOffGroup.cpp \
		WP6VariableLengthGroup.cpp \
		WP6EOLGroup.cpp \
		`glib-config --cflags` -shared -o libwpd2.so.$(VERSION)
	

install:	
	mkdir -p $(PREFIX)/include/libwpd2
	cp -f ./libwpd2.so.$(VERSION) $(PREFIX)/lib
	ln -f -s $(PREFIX)/lib/libwpd2.so.$(VERSION) $(PREFIX)/lib/libwpd2.so	
	cp -f {libwpd2.h,WPXParser.h,WPXHeader.h,WP6Parser.h,WP6Header.h,WPXLLListener.h,WPXHLListener.h} $(PREFIX)/include/libwpd2

clean:
	rm -rf ./libwpd2.so.$(VERSION)
