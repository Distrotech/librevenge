pkgconfdir = $(libdir)/pkgconfig

if WITH_LIBWPD_STREAM
LIBWPD_STREAM_PC = libwpd-stream-1.pc
endif


pkgconf_DATA = libwpd-1.pc $(LIBWPD_STREAM_PC)

SUBDIRS = src build

EXTRA_DIST = \
	autogen.sh		\
	libwpd.spec		\
	libwpd.spec.in		\
	libwpd-1.pc.in		\
	libwpd-stream-1.pc.in	\
	CHANGES
