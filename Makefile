UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  IGRAPH_PATH = /usr/local/
endif
ifeq ($(UNAME), Darwin)
  IGRAPH_PATH = /usr/local/Cellar/igraph/0.7.1_6/
endif

CC = gcc
OUTPUTS = lib_graphpass.o analyze.o filter.o gexf.o io.o quickrun.o reports.o rnd.o viz.o
HELPER_FILES = src/main/analyze.c src/main/filter.c src/main/gexf.c src/main/io.c src/main/quickrun.c src/main/reports.c src/main/rnd.c src/main/viz.c
IGRAPH_INCLUDE = $(IGRAPH_PATH)include/igraph
IGRAPH_LIB = $(IGRAPH_PATH)lib


TEST_INCLUDE = ./src/tests/
TEST_RUNNER_PATH = ./src/tests/
UNITY_INCLUDE = ./vendor/unity
INCLUDE = ./src/headers
DEPS = -I$(INCLUDE) -I$(IGRAPH_INCLUDE) -I$(UNITY_INCLUDE)
BUILD = build/

all: test install

install: src/main/graphpass.c
	gcc src/main/*.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm  -o graphpass
	- ./graphpass -qn

release: src/main/graphpass.c
	gcc src/main/*.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm  -o graphpass
	- ./graphpass -qg

debug: ./src/main/graphpass.c
	gcc -g src/main/*.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm  -o graphpass

test: qp ana run clean

qp: $(TEST_INCLUDE)runner_test_qp.c
	gcc $(UNITY_INCLUDE)/unity.c $(TEST_INCLUDE)runner_test_qp.c $(DEPS) $(TEST_INCLUDE)quickrun_test.c $(HELPER_FILES) -L$(IGRAPH_LIB) -ligraph -lm -o qp

ana: $(TEST_INCLUDE)runner_test_ana.c
	gcc $(UNITY_INCLUDE)/unity.c $(TEST_INCLUDE)runner_test_ana.c $(DEPS) $(TEST_INCLUDE)analyze_test.c $(HELPER_FILES) -L$(IGRAPH_LIB) -ligraph -lm -o ana

run:
	- ./ana
	./qp

.PHONY : clean
clean:
	rm -f qp
	rm -f ana
	rm -rf TEST_OUT_FOLDER
	rm -f graphpass
