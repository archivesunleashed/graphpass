#include "igraph.h"
#include "sample_graphs_test.h"
#include "analyze.h"
#include "unity.h"

void TEST_BETWEENNESS_ALGORITHM_GRAPH() {
  igraph_vector_t bet;
  calc_betweenness(&star_graph);
  VANV(&star_graph, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0] == 1.0);
}

void TEST_BETWEENNESS_ALGORITHM_DIGRAPH() {
  igraph_vector_t bet;
  calc_betweenness(&star_digraph);
  VANV(&star_graph, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(bet)[0] == 1.0);
}

main () {
  UNITY_BEGIN();
  RUN_TEST(TEST_BETWEENNESS_ALGORITHM_GRAPH);
  RUN_TEST(TEST_BETWEENNESS_ALGORITHM_DIGRAPH);
  return UNITY_END();
}
