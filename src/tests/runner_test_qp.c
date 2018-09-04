//
//  all_test.c
//  graphpass
//
//  Created by Ryan Deschamps on 2018-03-13.
//  Copyright © 2018 Ryan Deschamps. All rights reserved.
//
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
Unity.CurrentTestName = #TestFunc; \
Unity.CurrentTestLineNumber = TestLineNum; \
Unity.NumberOfTests++; \
if (TEST_PROTECT()) \
{ \
setUp(); \
TestFunc(); \
} \
if (TEST_PROTECT()) \
{ \
tearDown(); \
} \
UnityConcludeTest(); \
}

#include "igraph.h"
#include "unity.h"
#include "unity_internals.h"
#include "graphpass.h"
#include "stdio.h"

extern void setUp(void);
extern void tearDown(void);
extern void TEST_QUICKRUN_SIZE(void);
extern void TEST_QUICKRUN_COLOR(void);
extern void TEST_QUICKRUN_NOSAVE(void);
extern void TEST_QUICKRUN_DEGREE(void);
extern void TEST_QUICKRUN_GEXF(void);
extern void TEST_QUICKRUN_GRAPHML(void);

void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}

int main (void) {
  
  load_graph("src/resources/cpp2.graphml");
  UnityBegin("src/tests/quickrun_test.c");
  RUN_TEST(TEST_QUICKRUN_NOSAVE, 22);
  RUN_TEST(TEST_QUICKRUN_DEGREE, 29);
  RUN_TEST(TEST_QUICKRUN_COLOR, 51);
  RUN_TEST(TEST_QUICKRUN_SIZE, 40);
  RUN_TEST(TEST_QUICKRUN_GEXF, 72);
  RUN_TEST(TEST_QUICKRUN_GRAPHML, 80);
  igraph_destroy(&g);
  return (UNITY_END());
}
