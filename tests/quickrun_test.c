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

#include "lib_graphpass.h"
#include "quickrun.h"

void TEST_QUICKRUN() {
  igraph_i_set_attribute_table(&igraph_cattribute_table);
  createUndirectedGraph(&g);
  NODESIZE = igraph_vcount(&g);
  quickrunGraph();
  TEST_ASSERT_EQUAL_INT(1, 1);
}
