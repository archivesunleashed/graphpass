/*
 * GraphPass:
 * A utility to filter networks and provide a default visualization output
 * for Gephi or SigmaJS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
