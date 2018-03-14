/* -*- Graphpass -- Network Graph Utilities mode: C -*-  */
/* Copyright [2018] [Ryan Deschamps]
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License. */


/** @file graphpass.h
  @brief The base library for nutil
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/* Required External libraries */
#include "igraph.h"


#ifndef GRAPHPASS
#define GRAPHPASS
#define MAX_METHODS 9
#define ALL_METHODS 'abcdehioprw'
#define SIZE_DEFAULT "Degree"
#define SIZE_DEFAULT_CHAR 'd'
#define COLOR_BASE "WalkTrapModularity"
#define PAGERANK_DAMPING 0.85 /**< chance random walk will not restart */
#define LAYOUT_DEFAULT_CHAR = 'f'

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/* Constants */

igraph_t g;
igraph_attribute_table_t att;
char* FILENAME; /**< The filename from -f flag. */
char* DIRECTORY; /**< Directory to access FILENAME and put REPORT */
char* FILEPATH; /**< The filepath (DIRECTORY + FILENAME) */
char* METHODS;  /**< METHODS to filter */
char* OUTPUT;  /**< Folder to output new graphs */
char* OUTPATH; /**< Path to output folder (DIRECTORY + OUTPUT) */
long int NODESIZE = 0; /**< Number of Nodes in original graph */
float PERCENT; /**< Filtering percentage 0.0 by default */
bool REPORT = false; /**< Include a report? */
bool GFORMAT = false; /**< Graph format - true is "GEXF" false is "GRAPHML" */
bool QUICKRUN = false; /**< Lightweight visualization run */
bool SAVE = true; /**< If false, does not save graphs at all (for reports) */
bool CALC_WEIGHTS = false;
igraph_vector_t WEIGHTED; /**< If greater than 0, conducts weighted analysis */

/* Required Internal Libraries */
#include "gexf.h"
#include "rnd.h"
#include "reports.h"
#include "io.h"
#include "viz.h"
#include "analyze.h"
#include "quickrun.h"
#include "filter.h"

int print_graph_attrs (igraph_t *graph) {
  igraph_vector_t gtypes, vtypes, etypes;
  igraph_strvector_t gnames, vnames, enames;
  igraph_vector_init(&gtypes, 0);
  igraph_vector_init(&vtypes, 0);
  igraph_vector_init(&etypes, 0);
  igraph_strvector_init(&gnames, 0);
  igraph_strvector_init(&vnames, 0);
  igraph_strvector_init(&enames, 0);
  igraph_cattribute_list(graph, &gnames, &gtypes, &vnames, &vtypes,
			 &enames, &etypes);
  /* vvvvv print graph attributes -- REMOVE LATER vvvvvvv */
  printf("Graph attributes: ");
    for (int i=0; i<igraph_strvector_size(&gnames); i++) {
      printf("%s (%i) ", STR(gnames, i), (int)VECTOR(gtypes)[i]);
    }
  printf("\n");
    printf("Vertex attributes: ");
    for (int i=0; i<igraph_strvector_size(&vnames); i++) {
      printf("%s (%i) ", STR(vnames, i), (int)VECTOR(vtypes)[i]);
    }
  printf("\n");
  printf("Edge attributes: ");
  for (int i=0; i<igraph_strvector_size(&enames); i++) {
    printf("%s (%i) ", STR(enames, i), (int)VECTOR(etypes)[i]);
    }
  printf("\n");
  /* ^^^^print graph attributes -- REMOVE LATER ^^^^^*/
  igraph_strvector_destroy(&enames);
  igraph_strvector_destroy(&vnames);
  igraph_strvector_destroy(&gnames);
  igraph_vector_destroy(&etypes);
  igraph_vector_destroy(&vtypes);
  igraph_vector_destroy(&gtypes);
  return 0;
}
#endif
