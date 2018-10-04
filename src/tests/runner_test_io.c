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
#include "string.h"

extern void setUp(void);
extern void tearDown(void);
extern void TEST_GET_DIRECTORY(void);
extern void TEST_GET_FILE(void);
extern void TEST_STRIP_EXT(void);
extern void TEST_LOAD_GRAPH(void);
extern void TEST_WRITE_GRAPH(void);

void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}

int main (void) {
  ug_TEST = true;
  UnityBegin("src/tests/io_test.c");
  RUN_TEST(TEST_GET_DIRECTORY, 43);
  RUN_TEST(TEST_GET_FILE, 55);
  RUN_TEST(TEST_STRIP_EXT, 68);
  RUN_TEST(TEST_LOAD_GRAPH, 74);
  RUN_TEST(TEST_WRITE_GRAPH, 85);
  return (UNITY_END());
}
