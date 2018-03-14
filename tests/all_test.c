//
//  all_test.c
//  graphpass
//
//  Created by Ryan Deschamps on 2018-03-13.
//  Copyright © 2018 Ryan Deschamps. All rights reserved.
//
#include "unity.h"
#include "igraph.h"
#include "sample_graphs_test.h"
#include "analyze_test.h"
#include "filter_test.h"

int main () {
  UNITY_BEGIN();
  RUN_TEST(TEST_BETWEENNESS_ALGORITHM_GRAPH);
  RUN_TEST(TEST_BETWEENNESS_ALGORITHM_DIGRAPH);
  return UNITY_END();
}
