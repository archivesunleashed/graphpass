/** @file analyze_test.h 
 @brief Tests for analyze.h
 */

#include "../headers/graphpass.h"
#include "../headers/graphpass_test.h"
#include "unity.h"


void TEST_BETWEENNESS_ALGORITHM_GRAPH() {
  igraph_vector_t bet;
  createUndirectedGraph(&g);
  calc_betweenness(&g);
  VANV(&g, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}

void TEST_BETWEENNESS_ALGORITHM_DIGRAPH() {
  createDirectedGraph(&g);
  igraph_vector_t bet;
  calc_betweenness(&g);
  VANV(&g, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}

void TEST_AUTHORITY_ALGORITHM_GRAPH() {
  igraph_vector_t bet;
  createUndirectedGraph(&g);
  calc_betweenness(&g);
  VANV(&g, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}

void TEST_AUTHORITY_ALGORITHM_DIGRAPH() {
  createDirectedGraph(&g);
  igraph_vector_t bet;
  calc_betweenness(&g);
  VANV(&g, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 1.0);
}


