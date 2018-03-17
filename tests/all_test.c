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
#include "sample_graphs_test.c"
#include "analyze_test.c"
#include "filter_test.c"
#include "quickrun_test.c"

extern void setUp(void);
extern void tearDown(void);
extern void TEST_QUICKRUN(void);

void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}


int main () {
  UnityBegin("tests/quickrun_test.h");
  RUN_TEST(TEST_QUICKRUN, 15);
  return UNITY_END();
}
