/** @file analyze_test.h 
 @brief Tests for analyze.h
 */

#include "graphpass.h"
#include "unity.h"
#include <math.h>
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
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(aut)[10])/100, 0.00);
  igraph_vector_destroy(&aut);
}

void TEST_RANKORDER() {
  igraph_vector_t test, test2, ranks;
  igraph_vector_init(&test, 10);
  igraph_vector_init(&test2, 10);
  igraph_vector_init(&ranks, 10);
  VECTOR(test)[0] = 10; VECTOR(test)[1] = 2;
  VECTOR(test)[2] = 4;  VECTOR(test)[3] = 7;
  VECTOR(test)[4] = 10; VECTOR(test)[5] = 100;
  VECTOR(test)[6] = 11; VECTOR(test)[7] = 99;
  VECTOR(test)[8] = 2;  VECTOR(test)[9] = 10;
  VECTOR(ranks)[0] = 4; VECTOR(ranks)[1] = 9;
  VECTOR(ranks)[2] = 8;  VECTOR(ranks)[3] = 7;
  VECTOR(ranks)[4] = 4; VECTOR(ranks)[5] = 1;
  VECTOR(ranks)[6] = 3; VECTOR(ranks)[7] = 2;
  VECTOR(ranks)[8] = 9;  VECTOR(ranks)[9] = 4;
  produceRank(&test, &test2);
  TEST_ASSERT_TRUE(igraph_vector_all_e(&test2, &ranks));
  igraph_vector_destroy(&test);
  igraph_vector_destroy(&test2);
  igraph_vector_destroy(&ranks);
}


