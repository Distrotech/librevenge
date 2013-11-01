ACLOCAL_AMFLAGS = -I m4

pkgconfdir = $(libdir)/pkgconfig

pkgconf_DATA = \
	librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.pc \
	librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.pc

SUBDIRS = build inc src docs

EXTRA_DIST = \
	autogen.sh \
	librevenge.pc.in \
	librevenge-stream.pc.in \
	COPYING.LGPL \
	COPYING.MPL \
	HACKING \
	INSTALL \
	NEWS \
	README

distclean-local:
	rm -rf *.cache *~ *.pc

dist-hook:
	git log --date=short --pretty="format:@%cd  %an  <%ae>  [%H]%n%n%s%n%n%e%b" | sed -e "s|^\([^@]\)|\t\1|" -e "s|^@||" >$(distdir)/ChangeLog

