# gntp configure.ac

AC_INIT([ntpg], [0.1], [zdavkeos -at- gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])

# Use AM_SILENT_RULES if present
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

PKG_CHECK_MODULES([gtkmm], [gtkmm-3.0 >= 2.90.0])
#PKG_CHECK_MODULES([gtkmm], [gtkmm-2.4 >= 2.4.0])

#PKG_CHECK_MODULES([glibtop], [libgtop-2.0 >= 2.28.0])

# Custom flags
#CXX="clang"
CXXFLAGS="-Wall -g"

AC_OUTPUT
