AUTOMAKE_OPTIONS = gnu 1.4

pkgconfigdir = $(libdir)/pkgconfig

SUBDIRS = lib conv

distclean-local:
	cd test && make distclean

clean-local:
	cd test && make clean

EXTRA_DIST = test/Makefile.am test/Makefile.in test/test.cpp
