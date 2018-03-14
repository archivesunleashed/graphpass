#include "analyze.h"
#include "unity.h"

/** @file analyze_test.h 
 @brief Tests for analyze.h
 */

void TEST_BETWEENNESS_ALGORITHM_GRAPH() {
  igraph_vector_t bet;
  createUndirectedGraph();
  calc_betweenness(&star_graph);
  VANV(&star_graph, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}

void TEST_BETWEENNESS_ALGORITHM_DIGRAPH() {
  createDirectedGraph();
  igraph_vector_t bet;
  calc_betweenness(&star_digraph);
  VANV(&star_graph, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}

void TEST_AUTHORITY_ALGORITHM_GRAPH() {
  igraph_vector_t bet;
  createUndirectedGraph();
  calc_betweenness(&star_graph);
  VANV(&star_graph, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}

void TEST_AUTHORITY_ALGORITHM_DIGRAPH() {
  createDirectedGraph();
  igraph_vector_t bet;
  calc_betweenness(&star_digraph);
  VANV(&star_graph, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}


