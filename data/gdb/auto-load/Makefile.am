## Process this file with automake to produce Makefile.in

autoloaddir = $(GDBDATADIR)/auto-load/$(libdir)

dist_autoload_DATA = \
	librevenge-$(RVNG_MAJOR_VERSION).$(RVNG_MINOR_VERSION).py \
	librevenge-stream-$(RVNG_MAJOR_VERSION).$(RVNG_MINOR_VERSION).py

EXTRA_DIST = \
	librevenge.py.in \
	librevenge-stream.py.in

clean-local:
	-rm -f *.py[co]

distclean-local:
	-rm -f *.py

## vim: set shiftwidth=4 tabstop=4 noexpandtab:
