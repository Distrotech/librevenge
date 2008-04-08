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
	CHANGES \
	COPYING \
	CREDITS \
	HACKING \
	INSTALL \
	README \
	TODO
distclean-local:
	rm -rf *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

zip: install
	sh libwpd-zip 

check: all
	cd src/test && make check

if WITH_LIBWPD_DOCS
install-data-local:
	mkdir -p $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $(srcdir)/CHANGES $(DESTDIR)$(docdir)/
	$(INSTALL_DATA) $(srcdir)/COPYING $(DESTDIR)$(docdir)/
	$(INSTALL_DATA) $(srcdir)/CREDITS $(DESTDIR)$(docdir)/
	$(INSTALL_DATA) $(srcdir)/HACKING $(DESTDIR)$(docdir)/
	$(INSTALL_DATA) $(srcdir)/INSTALL $(DESTDIR)$(docdir)/
	$(INSTALL_DATA) $(srcdir)/README $(DESTDIR)$(docdir)/
	$(INSTALL_DATA) $(srcdir)/TODO $(DESTDIR)$(docdir)/
uninstall-local:
	-rm -rf $(DESTDIR)$(docdir)
endif
