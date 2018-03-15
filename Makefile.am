AUTOMAKE_OPTIONS = foreign

CFLAGS=-Wall -I/usr/local/Cellar/ -DVERSION=\"$(VERSION)\" -DPROG="\"$(PACKAGE)\""
LIBADD=-L/usr/local/Cellar/igraph/0.7.1_6/include/igraph -l/usr/local/Cellar/igraph/0.7.1_6/lib

bin_PROGRAMS = graphpass
graphpass_SOURCES = graphpass.c
