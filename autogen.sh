#!/bin/sh
#srcdir=`basename $0`
#[ -z "$srcdir" ] && srcdir=.

PKG_NAME=ntpg
REQUIRED_AUTOMAKE_VERSION=1.7

if [ ! -f "src/ntpg_ui.cc" ]; then
 echo "$srcdir doesn't look like source directory for $PKG_NAME" >&2
 exit 1
fi

. gnome-autogen.sh
