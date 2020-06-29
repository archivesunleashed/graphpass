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
  ug_OUTPATH = "TEST_OUT_FOLDER/";
  ug_percent = 0.0;
  ug_DIRECTORY = "src/resources/";
  ug_OUTFILE = "file";
  ug_gformat = true;
}

void tearDown(void) {
}

void TEST_WRITE_GEXF() {
  load_graph("src/resources/cpp2.graphml");
  filter_graph();
  TEST_ASSERT_TRUE(access("../TEST_OUT_FOLDER/file.gexf", F_OK ));
  TEST_ASSERT_TRUE(access("../TEST_OUT_FOLDER/file.gexf", R_OK ));
}
