bin_PROGRAMS = wpd2raw

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I../../lib/

wpd2raw_LDFLAGS = -L../../lib/
wpd2raw_LDADD = -lwpd-1 $(LIBWPD_LIBS)

wpd2raw_SOURCES =			\
	RawListener.cpp	 	\
	RawListener.h	 	\
	main.cpp
