UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  BASE_PATH = /usr/local/
  IGRAPH_PATH = $(BASE_PATH)
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

all: clean test install

install: src/main/graphpass.c
	gcc src/main/*.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm -o graphpass -fprofile-arcs -ftest-coverage
	- ./graphpass -qnv

release: src/main/graphpass.c
	gcc src/main/*.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm  -o graphpass -fprofile-arcs -ftest-coverage
	- ./graphpass -qgnv

debug: ./src/main/graphpass.c
	gcc -g -Wall src/main/*.c $(DEPS) -L$(IGRAPH_LIB) -ligraph -lm  -o graphpass -fprofile-arcs -ftest-coverage

test: qp ana io gexf run clean

qp: $(TEST_INCLUDE)runner_test_qp.c
	gcc $(UNITY_INCLUDE)/unity.c $(TEST_INCLUDE)runner_test_qp.c $(DEPS) $(TEST_INCLUDE)quickrun_test.c $(HELPER_FILES) -L$(IGRAPH_LIB) -ligraph -lm -o qp

ana: $(TEST_INCLUDE)runner_test_ana.c
	gcc $(UNITY_INCLUDE)/unity.c $(TEST_INCLUDE)runner_test_ana.c $(DEPS) $(TEST_INCLUDE)analyze_test.c $(HELPER_FILES) -L$(IGRAPH_LIB) -ligraph -lm -o ana

io: $(TEST_INCLUDE)runner_test_io.c
	gcc $(UNITY_INCLUDE)/unity.c $(TEST_INCLUDE)runner_test_io.c $(DEPS) $(TEST_INCLUDE)io_test.c $(HELPER_FILES) -L$(IGRAPH_LIB) -ligraph -lm -o io

gexf: $(TEST_INCLUDE)runner_test_gexf.c
	gcc $(UNITY_INCLUDE)/unity.c $(TEST_INCLUDE)runner_test_gexf.c $(DEPS) $(TEST_INCLUDE)gexf_test.c $(HELPER_FILES) -L$(IGRAPH_LIB) -ligraph -lm -o gexf

run:
	- ./ana
	./qp
	./io
	./gexf

.PHONY : clean
clean:
	rm -f qp
	rm -f ana
	rm -f io
	rm -f gexf
	rm -rf TEST_OUT_FOLDER
	rm -rf $(BUILD)
	rm -f graphpass
	rm -f *.gcno
	rm -f *.gcda
	rm -f *.c.gcov
	rm -f *.h.gcov
