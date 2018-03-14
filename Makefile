UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  IGRAPH_PATH = /usr/local/
endif
ifeq ($(UNAME), Darwin)
  IGRAPH_PATH = /usr/local/Cellar/igraph/0.7.1_6/
endif

CC = gcc

IGRAPH_INCLUDE = $(IGRAPH_PATH)include/igraph
IGRAPH_LIB = $(IGRAPH_PATH)lib
TEST_INCLUDE = ./tests/
UNITY_INCLUDE = -I./unity/
INCLUDE = -I./include/
DEPS = -I$(INCLUDE) -I$(IGRAPH_INCLUDE) -I$(UNITY_INCLUDE)
BUILD = build/

install: graphpass.c
	gcc graphpass.c -g $(DEPS) -L$(IGRAPH_LIB) -ligraph -o graphpass -lm
	./graphpass --file cpp2.graphml --output OUT/ --percent 10 --method b

debug: graphpass.c
	gcc graphpass.c -g -I$(IGRAPH_INCLUDE) -L$(IGRAPH_LIB) -ligraph -o graphpass

test: graphpass_test.c
	gcc graphpass_test.c iI$(DEPS) -L$(IGRAPH_LIB)


.PHONY : clean
clean:
	rm -f graphpass
