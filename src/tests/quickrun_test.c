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

#include <unistd.h>
#include "unity.h"
#include "graphpass.h"

void setUp() {
  ug_quickrun = true;
  ug_save = false;
  ug_FILENAME = "cpp2.graphml";
  ug_OUTPUT = "TEST_OUT_FOLDER/";
  ug_percent = 0.0;
  ug_DIRECTORY = "src/resources/";
  ug_gformat = false;
}

void TEST_QUICKRUN_NOSAVE() {
  quickrunGraph();
  ug_save = false;
  TEST_ASSERT_TRUE(access("TEST_OUTPUT_FOLDER/cpp2.gexf", F_OK) == -1);
  TEST_ASSERT_TRUE(access("TEST_OUTPUT_FOLDER/cpp2.graphml", F_OK) == -1);
}

void TEST_QUICKRUN_DEGREE() {
  quickrunGraph();
  igraph_vector_t deg;
  igraph_vector_init(&deg, 0);
  VANV(&g, "Degree", &deg);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(deg)[0], 1.0);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(deg)[10], 1.0);
  igraph_vector_destroy(&deg);
}

void TEST_QUICKRUN_COLOR() {
  quickrunGraph();
  igraph_vector_t rd,gr,bl;
  igraph_vector_init(&rd, 0);
  igraph_vector_init(&gr, 0);
  igraph_vector_init(&bl, 0);
  VANV(&g, "r", &rd);
  VANV(&g, "g", &gr);
  VANV(&g, "b", &bl);
  TEST_ASSERT_EQUAL_INT((int)VECTOR(rd)[0], 59);
  TEST_ASSERT_EQUAL_INT((int)VECTOR(rd)[10], 217);
  TEST_ASSERT_EQUAL_INT((int)VECTOR(gr)[0], 217);
  TEST_ASSERT_EQUAL_INT((int)VECTOR(gr)[10], 120);
  TEST_ASSERT_EQUAL_INT((int)VECTOR(bl)[0], 209);
  TEST_ASSERT_EQUAL_INT((int)VECTOR(bl)[10], 189);
  igraph_vector_destroy(&rd);
  igraph_vector_destroy(&gr);
  igraph_vector_destroy(&bl);
}

void TEST_QUICKRUN_SIZE() {
  quickrunGraph();
  igraph_vector_t size;
  igraph_vector_init(&size, 0.0);
  VANV(&g, "size", &size);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(size)[0], 0.0);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(size)[10], 0.0);
  igraph_vector_destroy(&size);
}


void TEST_QUICKRUN_GEXF () {
  ug_save = !ug_save;
  ug_gformat = true;
  quickrunGraph();
  TEST_ASSERT_TRUE(access("../TEST_OUT_FOLDER/cpp2.gexf", F_OK ));
  TEST_ASSERT_TRUE(access("../TEST_OUT_FOLDER/cpp2.gexf", R_OK ));
}

void TEST_QUICKRUN_GRAPHML() {
  ug_save = true;
  ug_gformat = false;
  quickrunGraph();
  TEST_ASSERT_TRUE(access("../TEST_OUT_FOLDER/cpp2.graphml", F_OK ));
  TEST_ASSERT_TRUE(access("../TEST_OUT_FOLDER/cpp2.graphml", R_OK ));
}
