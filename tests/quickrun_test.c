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

#include "../headers/graphpass.h"
#include "../headers/graphpass_test.h"
#include "../unity/unity.h"

void TEST_QUICKRUN() {
  QUICKRUN = true;
  printf("q: %i %s", QUICKRUN, SIZE_DEFAULT);
  load_graph("assets/cpp2.graphml");
  printf("Is there an igraph available to us? %i", igraph_vcount(&g));
  printf("FINISHED MAKING GRAPH");
  quickrunGraph();
}
