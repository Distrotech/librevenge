ACLOCAL_AMFLAGS = -I m4

pkgconfdir = $(libdir)/pkgconfig

if WITH_LIBREVENGE_STREAM
LIBREVENGE_STREAM_PC = librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.pc
endif

pkgconf_DATA = librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.pc $(LIBREVENGE_STREAM_PC)

SUBDIRS = build inc src docs

EXTRA_DIST = \
	autogen.sh \
	librevenge.spec \
	librevenge.spec.in \
	librevenge.pc.in \
	librevenge-stream.pc.in \
	librevenge-zip.in \
	COPYING.LGPL \
	COPYING.MPL \
	CREDITS \
	HACKING \
	INSTALL \
	NEWS \
	README \
	TODO

distclean-local:
	rm -rf *.cache *~ *.pc

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

zip: install
	sh librevenge-zip 

dist-hook:
	git log --date=short --pretty="format:@%cd  %an  <%ae>  [%H]%n%n%s%n%n%e%b" | sed -e "s|^\([^@]\)|\t\1|" -e "s|^@||" >$(distdir)/ChangeLog

