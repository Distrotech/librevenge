## Process this file with automake to produce Makefile.in

SUBDIRS = auto-load

pythondir = $(datadir)/librevenge/python
nobase_dist_python_DATA = \
	librevenge/__init__.py \
	librevenge/util/__init__.py \
	librevenge/util/compatibility.py \
	librevenge/util/printing.py \
	librevenge/v0_0/__init__.py \
	librevenge/v0_0/streams.py \
	librevenge/v0_0/types.py

clean-local:
	-rm -f \
		librevenge/*.py[co] \
		librevenge/util/*.py[co] \
		librevenge/v0_0/*.py[co]

## vim: set shiftwidth=4 tabstop=4 noexpandtab:
