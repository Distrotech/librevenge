bin_PROGRAMS = wpd2text

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I../../lib/

wpd2text_LDFLAGS = -L../../lib/
wpd2text_LDADD = -lwpd-1 $(LIBWPD_LIBS)

wpd2text_SOURCES =			\
	TextListener.cpp	 	\
	main.cpp
