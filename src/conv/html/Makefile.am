bin_PROGRAMS = wpd2html

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I../../lib/

wpd2html_LDFLAGS = -L../../lib/
wpd2html_LDADD = -lwpd2 $(LIBWPD_LIBS)

wpd2html_SOURCES =			\
	HtmlListenerImpl.cpp	 	\
	main.cpp
