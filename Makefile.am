ACLOCAL_AMFLAGS = -I m4

pkgconfdir = $(libdir)/pkgconfig

if WITH_LIBWPD_STREAM
LIBWPD_STREAM_PC = libwpd-stream-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.pc
endif

pkgconf_DATA = libwpd-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.pc $(LIBWPD_STREAM_PC)

SUBDIRS = src build docs autopackage

EXTRA_DIST = \
	autogen.sh \
	libwpd.spec \
	libwpd.spec.in \
	libwpd-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.pc.in \
	libwpd-stream-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.pc.in \
	libwpd-zip.in \
	COPYING \
	CREDITS \
	HACKING \
	INSTALL \
	NEWS \
	README \
	TODO
distclean-local:
	rm -rf *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

zip: install
	sh libwpd-zip 

dist-hook:
	git log --date=short --pretty="format:@%cd  %an  <%ae>  [%H]%n%n%s%n%n%e%b" | sed -e "s|^\([^@]\)|\t\1|" -e "s|^@||" >$(distdir)/ChangeLog

