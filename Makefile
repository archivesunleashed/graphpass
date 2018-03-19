UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  IGRAPH_PATH = /usr/local/
endif
ifeq ($(UNAME), Darwin)
  IGRAPH_PATH = /usr/local/Cellar/igraph/0.7.1_6/
endif

CC = gcc
OUTPUTS = lib_graphpass.o analyze.o filter.o gexf.o io.o quickrun.o reports.o rnd.o viz.o
HELPERFILES = analyze.c filter.c gexf.c graphpass.c io.c lib_graphpass.c quickrun.c \
  reports.c rnd.c viz.c
IGRAPH_INCLUDE = $(IGRAPH_PATH)include/igraph
IGRAPH_LIB = $(IGRAPH_PATH)lib


TEST_INCLUDE = ./tests/
TEST_RUNNER_PATH = ./tests/
UNITY_INCLUDE = ./unity
INCLUDE = ./headers
DEPS = -I$(INCLUDE) -I$(IGRAPH_INCLUDE) -I$(UNITY_INCLUDE)
BUILD = build/

install:  release

release: src/graphpass.c
	gcc src/graphpass.c src/analyze.c src/filter.c src/gexf.c src/io.c src/quickrun.c src/reports.c src/rnd.c src/viz.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm  -o graphpass
	- ./graphpass -qg

debug: ./src/graphpass.c
	gcc -g src/graphpass.c src/analyze.c src/filter.c src/gexf.c src/io.c src/quickrun.c src/reports.c src/rnd.c src/viz.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm  -o graphpass

test: $(TEST_INCLUDE)all_test.c
	gcc $(UNITY_INCLUDE)/unity.c src/quickrun.c src/analyze.c src/filter.c src/rnd.c src/reports.c  src/viz.c src/io.c src/gexf.c $(TEST_INCLUDE)*.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -o graphpass_test
	- ./graphpass_test


.PHONY : clean
clean:
	rm -f graphpass
