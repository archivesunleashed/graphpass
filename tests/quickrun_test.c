//
//  quickrun_test.c
//  graphpass
//
//  Created by Ryan Deschamps on 2018-03-15.
//  Copyright © 2018 Ryan Deschamps. All rights reserved.
//

/** @file quickrun_test.h
 @brief Tests for quickrun.h
 */

#include "unity.h"
#include "graphpass.h"

void TEST_QUICKRUN_DEGREE() {
  QUICKRUN = true;
  quickrunGraph();
  igraph_vector_t deg;
  igraph_vector_init(&deg, 0);
  VANV(&g, "Degree", &deg);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(deg)[0], 1.0);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(deg)[10], 1.0);
  igraph_vector_destroy(&deg);
}
