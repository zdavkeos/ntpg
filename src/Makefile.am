# ntpg src/Makefile.am

#AUTOMAKE_OPTIONS = subdir-objects
#ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CPPFLAGS = $(gtkmm_CFLAGS) $(glibtop_CFLAGS)
ntpg_LDADD = $(gtkmm_LIBS) $(glibtop_LIBS)

bin_PROGRAMS = ntpg
ntpg_SOURCES = main.cc \
	ui.cc ui.h \
	peers.cc peers.h \
	conf_view.cc conf_view.h \
	ntp_interface.cc ntp_interface.h \
	summary.cc summary.h \
	statistics.cc statistics.h \
	parse.cc parse.h \
	util.cc util.h
