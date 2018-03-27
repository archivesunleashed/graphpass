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
extern void TEST_DEGREE_ALGORITHM(void);
extern void TEST_BETWEENNESS_ALGORITHM(void);
extern void TEST_AUTHORITY_ALGORITHM(void);
extern void TEST_RANKORDER(void);
extern void TEST_HUB_ALGORITHM(void);
extern void TEST_EIGENVECTOR_ALGORITHM(void);
extern void TEST_PAGERANK_ALGORITHM(void);
extern void TEST_INDEGREE_ALGORITHM(void);
extern void TEST_OUTDEGREE_ALGORITHM(void);
extern void TEST_MODULARITY(void);
extern void TEST_MEAN(void);
extern void TEST_VARIANCE(void);
extern void TEST_STD(void);
extern void TEST_STDERROR(void);
extern void TEST_TTEST(void);
extern void TEST_TPVALUE(void);

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
  SAVE = true;
  load_graph("assets/cpp2.graphml");
  UnityBegin("tests/analyze_test.c");
  RUN_TEST(TEST_DEGREE_ALGORITHM, 14);
  RUN_TEST(TEST_INDEGREE_ALGORITHM, 25);
  RUN_TEST(TEST_OUTDEGREE_ALGORITHM, 36);
  RUN_TEST(TEST_BETWEENNESS_ALGORITHM, 47);
  RUN_TEST(TEST_AUTHORITY_ALGORITHM, 58);
  RUN_TEST(TEST_HUB_ALGORITHM, 69);
  RUN_TEST(TEST_EIGENVECTOR_ALGORITHM, 80);
  RUN_TEST(TEST_PAGERANK_ALGORITHM, 91);
  RUN_TEST(TEST_MODULARITY, 102);
  RUN_TEST(TEST_RANKORDER, 113);
  RUN_TEST(TEST_MEAN, 138);
  RUN_TEST(TEST_VARIANCE, 151);
  RUN_TEST(TEST_STD,164);
  RUN_TEST(TEST_STDERROR, 177);
  RUN_TEST(TEST_TTEST, 190);
  RUN_TEST(TEST_TPVALUE, 203)
  igraph_destroy(&g);
  return (UNITY_END());
}
