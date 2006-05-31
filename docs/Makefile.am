SUBDIRS = doxygen

EXTRA_DIST = libwpd.dia libwpd.png

if WITH_LIBWPD_DOCS

install-data-am:
	mkdir -p $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@
	$(INSTALL) $(srcdir)/libwpd.dia $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
	$(INSTALL) $(srcdir)/libwpd.png $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/

uninstall-am:
	-rm -f $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/libwpd.dia
	-rm -f $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/libwpd.png

endif
