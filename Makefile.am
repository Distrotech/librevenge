pkgconfdir = $(libdir)/pkgconfig

if WITH_LIBWPD_STREAM
LIBWPD_STREAM_PC = libwpd-stream-0.8.pc
endif

pkgconf_DATA = libwpd-0.8.pc $(LIBWPD_STREAM_PC)

SUBDIRS = src build

EXTRA_DIST = \
	autogen.sh		\
	libwpd.spec		\
	libwpd.spec.in		\
	libwpd-*.pc.in	\
	libwpd-zip.in		\
	CHANGES

distclean-local:
	rm -rf *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

zip: all install
	sh libwpd-zip

