EXTRA_DIST =	doxygen.cfg \
		footer.html.in \
		header.html.in

if WITH_LIBWPD_DOCS

install-data-am:
	mkdir -p $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html
	$(INSTALL) html/* $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html/

uninstall-am:
	-rm -rf $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html
	
endif

all:
	doxygen doxygen.cfg

distclean-local: clean-local

clean-local:
	rm -rf html
