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
extern void TEST_QUICKRUN_DEGREE(void);

void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}

int main (void) {
  FILENAME = "cpp2.graphml";
  OUTPUT = "TEST_OUT_FOLDER/";
  PERCENT = 0.0;
  DIRECTORY = "assets/";
  GFORMAT = false;
  SAVE = false;
  load_graph("assets/cpp2.graphml");
  UnityBegin("tests/quickrun_test.c");
  RUN_TEST(TEST_QUICKRUN_DEGREE, 20)
  igraph_destroy(&g);
  return (UNITY_END());
}
