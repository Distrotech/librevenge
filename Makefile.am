pkgconfdir = $(libdir)/pkgconfig

if WITH_LIBWPD_STREAM
LIBWPD_STREAM_PC = libwpd-stream-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.pc
endif

pkgconf_DATA = libwpd-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.pc $(LIBWPD_STREAM_PC)

SUBDIRS = src build docs autopackage

EXTRA_DIST = \
	autogen.sh	\
	libwpd.spec	\
	libwpd.spec.in	\
	libwpd-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.pc.in	\
	libwpd-zip.in	\
	CHANGES		\
	COPYING		\
	CREDITS		\
	HACKING		\
	INSTALL		\
	README		\
	TODO

distclean-local:
	rm -rf *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

zip: all install
	sh libwpd-zip 

check: all
	cd src/test && make check

if WITH_LIBWPD_DOCS
install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@
	$(INSTALL) $(srcdir)/CHANGES $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
	$(INSTALL) $(srcdir)/COPYING $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
	$(INSTALL) $(srcdir)/CREDITS $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
	$(INSTALL) $(srcdir)/HACKING $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
	$(INSTALL) $(srcdir)/INSTALL $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
	$(INSTALL) $(srcdir)/README $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
	$(INSTALL) $(srcdir)/TODO $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
uninstall-local:
	-rm -rf $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@
endif
