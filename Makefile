CC = gcc
IGRAPH_PATH = /usr/local/Cellar/igraph/0.7.1_6/
IGRAPH_INCLUDE = $(IGRAPH_PATH)include/igraph
IGRAPH_LIB = $(IGRAPH_PATH)lib
INCLUDE = ./Include/*.h
DEPS = INCLUDE IGRAPH_INCLUDE

install: borg-reducer.c
	gcc borg-reducer.c -I$(IGRAPH_INCLUDE) -L$(IGRAPH_LIB) -ligraph -o borgreducer

debug: borg-reducer.c
	gcc borg-reducer.c -g -I$(IGRAPH_INCLUDE) -L$(IGRAPH_LIB) -ligraph -o borgreducer

.PHONY : clean
clean:
	rm -f borgreducer
