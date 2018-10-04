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

/** @file analyze_test.h
 @brief Tests for analyze.h
 */

#include "graphpass.h"
#include "unity.h"
#include <math.h>

// Constants
char samp[TEST_ARRAY_LENGTH][TEST_MAX_STRING_SIZE] = {
  "/path/to/a/directory/",
  "/path/to/a/file.ext",
  "file.ext"}, path[TEST_MAX_STRING_SIZE];

void setUp(void) {
}

void tearDown(void) {
}

void TEST_GET_DIRECTORY() {
  char expected[TEST_ARRAY_LENGTH][TEST_MAX_STRING_SIZE] = {
    "/path/to/a/directory/",
    "/path/to/a/",
    "./"}, path[TEST_MAX_STRING_SIZE];
  for (int i=0; i < TEST_ARRAY_LENGTH; i++) {
    char *expect;
    get_directory(samp[i], &expect);
    TEST_ASSERT_EQUAL_STRING(expect, expected[i]);
  }
}

void TEST_GET_FILE() {
  for (int i=TEST_ARRAY_LENGTH-1; i < 0; i--) {
    char expect[TEST_FILENAME_SIZE] = "file.ext";
    char *result;
    get_filename(samp[i], &result);
    if (i > 0) {
      TEST_ASSERT_EQUAL_STRING(result, expect);
    } else { // last value should be null
      TEST_ASSERT_NULL(result);
    }
  }
}

void TEST_STRIP_EXT() {
  char expect[TEST_FILENAME_SIZE] = "file.ext";
  char result[5] = "file";
  strip_ext(expect);
  TEST_ASSERT_EQUAL_STRING(&expect, result);
}

void TEST_LOAD_GRAPH() {
  load_graph("src/resources/cpp2.graphml");
  int fail = load_graph("fake/filepath.cpp2.graphml");
  int success = load_graph("src/resources/cpp2.graphml");
  TEST_ASSERT_EQUAL_INT(NODESIZE, 218);
  TEST_ASSERT_EQUAL_INT(EDGESIZE, 220);
  TEST_ASSERT_EQUAL_INT(fail, -1);
  TEST_ASSERT_EQUAL_INT(success, 0);
  igraph_destroy(&g);
}

void TEST_WRITE_GRAPH() {
  struct stat st = {0};
  ug_save = true;
  ug_FILENAME = "cpp2.graphml";
  ug_OUTPATH = "TEST_OUT_FOLDER/";
  ug_percent = 0.0;
  ug_DIRECTORY = "src/resources/";
  ug_OUTFILE = ug_FILENAME;
  ug_gformat = false;
  if (stat(ug_OUTPATH, &st) == -1) {
    mkdir(ug_OUTPATH, 0700);
  }
  igraph_t graph;
  igraph_vector_t v;
  igraph_real_t edges[] = {0,1,0,2,0,3};
  igraph_vector_view(&v, edges, sizeof(edges)/sizeof(double));
  igraph_create(&graph, &v, 0, IGRAPH_DIRECTED);
  int success = write_graph(&graph, "hey");
  ug_OUTPATH = "FAKE_TEST_OUT_FOLDER/";
  int fail = write_graph(&graph, "hey");
  TEST_ASSERT_EQUAL_INT(fail, -1);
  TEST_ASSERT_EQUAL_INT(success, 0);
  igraph_destroy(&graph);
}
