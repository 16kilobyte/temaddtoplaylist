## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(TOTEMADDTOPLAYLIST_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libtotemaddtoplaylist.la


libtotemaddtoplaylist_la_SOURCES = \
	main.c

libtotemaddtoplaylist_la_LDFLAGS = 

libtotemaddtoplaylist_la_LIBADD = $(TOTEMADDTOPLAYLIST_LIBS)

include_HEADERS = \
	totemaddtoplaylist.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libtotemaddtoplaylist-0.1.pc

EXTRA_DIST = \
	libtotemaddtoplaylist-0.1.pc.in
