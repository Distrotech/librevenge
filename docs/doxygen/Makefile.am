EXTRA_DIST =	doxygen.cfg \
		footer.html.in \
		header.html.in

if WITH_LIBWPD_DOCS

install-data-am:
	mkdir -p $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html
	$(INSTALL) html/*.html $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html/
	$(INSTALL) html/*.png $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html/
	$(INSTALL) html/*.css $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html/

uninstall-am:
	-rm -rf $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/html

all:
	test -f header.html.in || $(LN_S) $(srcdir)/header.html.in ./
	test -f footer.html.in || $(LN_S) $(srcdir)/footer.html.in ./
	doxygen $(srcdir)/doxygen.cfg

else

all:

endif

distclean-local: clean-local

clean-local:
	rm -rf html
