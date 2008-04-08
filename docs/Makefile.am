SUBDIRS = doxygen

EXTRA_DIST = libwpd.dia libwpd.png

if WITH_LIBWPD_DOCS

install-data-am:
	mkdir -p $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $(srcdir)/libwpd.dia $(DESTDIR)$(docdir)/libwpd.dia
	$(INSTALL_DATA) $(srcdir)/libwpd.png $(DESTDIR)$(docdir)/libwpd.png

uninstall-am:
	-rm -f $(DESTDIR)$(docdir)/libwpd.dia
	-rm -f $(DESTDIR)$(docdir)/libwpd.png

endif
