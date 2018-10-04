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

/** @file graphpass.c
 @brief The main file for GraphPass.

 Graphpass accepts a file, a percentage and a series of characters that represent
 methods of filtering a network graph, and outputs new graph files with the filtered
 graphs and optionally, a report showing how those filters affected the graph.
 */

#define _GNU_SOURCE
#define ASSETS_PATH "src/resources"
#define MAX_METHODS 9
#define MAX_FILESIZE 100

#include "igraph.h"
#include <stdlib.h>
#include "graphpass.h"

char* FILEPATH; /**< The filepath (ug_DIRECTORY + ug_FILENAME). */

/** Whether to save the graph. **/
bool ug_save = true;
/** Graph format true for GEXF; false for GRAPHML. **/
bool ug_gformat = false;
/** Produce a report analyzing effect of filtering on graph. **/
bool ug_report = false;
/** Provide a quickrun with simple sizing, positioning and coloring. **/
bool ug_quickrun = false;
/** Print out helper messages. **/
bool ug_verbose = false;
/** Not a test file. */
bool ug_TEST = false;
/** Concluding error msg. */
int conclude;

const char hyphen = '-';

int main (int argc, char *argv[]) {
  int c;
  while (1)
    {
      static struct option long_options[] =
        {
          /* These options have no required argument. */
          {"gexf",    no_argument,       0, 'g'},
          {"no-save", no_argument,       0, 'n'},
          {"quick",   no_argument,       0, 'q'},
          {"report",  no_argument,       0, 'r'},
          {"verbose", no_argument,       0, 'v'},

          /* These options require an argument. */
          {"input", required_argument, 0, 'i'},
          {"methods", required_argument, 0, 'm'},
          {"output",  required_argument, 0, 'o'},
          {"percent", required_argument, 0, 'p'},
          {"max-nodes", required_argument, 0, 'x'},
          {"max-edges", required_argument, 0, 'y'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;
      c = getopt_long (argc, argv, "gnvqri:m:o:p:x:y:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;
      switch (c)
        {
        case 0:
          /* If this option sets a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
        case 'n':
          ug_save = !ug_save;
          break;
        case 'g':
          ug_gformat = !ug_gformat;
          break;
        case 'i':
          ug_INPUT = optarg ? optarg : "./";
          break;
        case 'r':
          ug_report = !ug_report;
          break;
        case 'm':
          ug_methods = optarg ? optarg : "d";
          break;
        case 'o':
          ug_OUT = optarg ? optarg : "./";
          break;
        case 'p':
          ug_percent = optarg ? atof(optarg) : 0.0;
          break;
        case 'q':
          ug_quickrun = !ug_quickrun;
          break;
        case 'w':
          CALC_WEIGHTS = !CALC_WEIGHTS;
          break;
        case 'v':
          ug_verbose = true;
          break;
        case 'x':
          ug_maxnodes = optarg ? (long)strtol(optarg, (char**)NULL, 10) : MAX_NODES;
          break;
        case 'y':
          ug_maxedges = optarg ? (long)strtol(optarg, (char**)NULL, 10) : MAX_EDGES;
          break;
        case '?':
          /* getopt_long already printed an error message. */
          break;
        default:
          abort ();
        }
    }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      while (optind < argc)
        pushArg(&ug_args, argv[optind++]);
    }

  /** Set default values if not included in flags. **/
  if (ug_args) {
    ug_OUTARG = ug_args->val;
    if (ug_args->next) {
      ug_PATH = ug_args->next->val;
    } else {
      ug_PATH = ug_OUTARG;
      ug_OUTARG = NULL;
    }
  }
  ug_maxnodes = ug_maxnodes ? ug_maxnodes : MAX_NODES;
  ug_maxedges = ug_maxedges ? ug_maxedges : MAX_EDGES;
  ug_percent = ug_percent ? ug_percent : 0.00;
  ug_methods = ug_methods ? ug_methods : "d";
  /** Setup directory path and filenames. **/
  FILEPATH = ug_INPUT ? ug_INPUT : ug_PATH;
  FILEPATH = FILEPATH ? FILEPATH : "src/resources/cpp2.graphml";
  get_filename(FILEPATH, &ug_FILENAME);
  ug_FILENAME = ug_FILENAME ? ug_FILENAME : "FILE";
  get_directory(FILEPATH, &ug_DIRECTORY);
  ug_DIRECTORY = ug_DIRECTORY ? ug_DIRECTORY : "./";
  if (ug_OUTARG) {
    get_filename(ug_OUTARG, &ug_OUTFILE);
    // If OUTPATH and FILEPATH are the same, this could overwrite original file.
    // Checks follow below.
    ug_OUTFILE = ug_OUTFILE ? ug_OUTFILE : ug_FILENAME;
    get_directory(ug_OUTARG, &ug_OUTPATH);
    ug_OUTPATH = ug_OUTPATH ? ug_OUTPATH : "./";
  } else {
    ug_OUTPATH = "./";
    ug_OUTFILE = ug_FILENAME;
  }
  // Prevent output from destroying original file.
  if(strcmp(ug_OUTFILE, ug_FILENAME) == 0
    && strcmp(ug_OUTPATH, ug_DIRECTORY) == 0) {
    fprintf(stderr, "FAIL >>> Input and output locations cannot be the same.\n");
    fprintf(stderr, "FAIL >>> Exiting...\n");
    exit(EXIT_FAILURE);
  }

  /** Start output description. **/
  if (ug_verbose == true) {
    printf(">>>>>>>  GRAPHPASSING >>>>>>>> \n");
    printf("FILEPATH: %s\n", FILEPATH);
    printf("OUTPUT DIRECTORY: %s\nPERCENTAGE: %f\n", ug_OUTPATH, ug_percent);
    printf("FILE: %s\nMETHODS STRING: %s\n", ug_FILENAME, ug_methods);
    printf("QUICKRUN: %i\nREPORT: %i\nSAVE: %i\n", ug_quickrun, ug_report, ug_save);
  }

  /** Set up FILEPATH to access graphml file. **/
  if (ug_verbose == true) {
    printf("Running graphpass on file: %s\n", FILEPATH);
  }
  int load = load_graph(FILEPATH);
  if (load != 0) {
    fprintf(stderr, "FAIL >>> Graphpass could not load the graph.");
    exit(EXIT_FAILURE);
  }
  if (igraph_vcount(&g) > ug_maxnodes || igraph_ecount(&g) > ug_maxedges){
    fprintf(stderr, "FAIL >>> Graphpass can only conduct analysis on graphs with \
fewer than %li nodes and %li edges.\n", ug_maxnodes, ug_maxedges);
    fprintf(stderr, "FAIL >>> Exiting...\n");
    exit(EXIT_FAILURE);
  }

  /** Start the filtering based on values and methods. **/
  conclude = filter_graph();
  if (conclude == 0) {
    printf("\n\n>>>>  SUCCESS!");
  } else {
    fprintf(stderr, ">>>>  FAIL - an unknown error occurred during run.");
  }
  if (ug_save) {
    printf("- Files output to %s\n", ug_OUTPATH);
  }
  else {
    printf("- NO_SAVE requested, so no output.\n\n\n");
  }
  return 0;
}
