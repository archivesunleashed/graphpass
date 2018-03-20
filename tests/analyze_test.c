/** @file analyze_test.h 
 @brief Tests for analyze.h
 */

#include "graphpass.h"
#include "unity.h"
void setUp(void) {
  calc_degree(&g, 'd');
  
  
  
  calc_betweenness(&g);
  
  
  
  calc_authority(&g);
  
}
void tearDown(void) {
  
  
  
}

void TEST_DEGREE_ALGORITHM() {
  igraph_vector_t deg;
  igraph_vector_init(&deg, 0);
  VANV(&g, "Degree", &deg);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(deg)[0], 1.0);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(deg)[10], 1.0);
  igraph_vector_destroy(&deg);
}

void TEST_BETWEENNESS_ALGORITHM() {
  igraph_vector_t bet;
  igraph_vector_init(&bet, 0);
  VANV(&g, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0], 0.0);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[10], 0.0);
  igraph_vector_destroy(&bet);
}

void TEST_AUTHORITY_ALGORITHM() {
  igraph_vector_t aut;
  igraph_vector_init(&aut, 0);
  VANV(&g, "Authority", &aut);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(aut)[0], 0.0768518);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(aut)[10], 0.0);
  igraph_vector_destroy(&aut);
}



