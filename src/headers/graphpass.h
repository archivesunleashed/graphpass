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

#ifndef LIB_GRAPHPASS_H
#define LIB_GRAPHPASS_H

#include <igraph.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

typedef enum { false, true } bool;
typedef enum { FAIL, WARN, COMM } broadcast;

igraph_t g;
igraph_attribute_table_t att;
const char* ug_FILENAME; /**< The filename from -f flag. */
const char* ug_DIRECTORY; /**< Directory to access FILENAME */
char* ug_methods;  /**< METHODS to filter */
char* ug_OUTPUT;  /**< Folder to output new graphs */
char* OUTPATH; /**< Path to output folder (DIRECTORY + OUTPUT) */
igraph_integer_t NODESIZE; /**< Number of Nodes in original graph */
float ug_percent; /**< Filtering percentage 0.0 by default */
bool ug_report; /**< Include a report? */
bool ug_gformat; /**< Graph format - true is "GEXF" false is "GRAPHML" */
bool ug_quickrun; /**< Lightweight visualization run */
bool ug_save; /**< If false, does not save graphs at all (for reports) */
bool ug_verbose; //**< Verbose mode (default off). */
bool CALC_WEIGHTS;
igraph_vector_t WEIGHTED; /**< If greater than 0, conducts weighted analysis */

/* Required External libraries */

#define PROGRAM_NAME "GraphPass"
#define BUG_REPORT "https://www.github.com/archivesunleashed/graphpass/issues"
#define GIT_REPOSITORY "https://www.github.com/archivesunleashed/graphpass"

/* Color Presets */

#define COLOUR_SET_PASTEL "pastel.h"
#define COLOUR_SET_PRIMARY "primary.h"
#define COLOUR_SET_DAMPENED "dampened.h"

/* Visualization Presets */

#define VIZ_SET_SPACIOUS "viz_spacious.h"
#define VIZ_SET_LARGE "viz_large.h"
#define VIZ_SET_SMALL "vis_small.h"

/* Default Settings */
#define MAX_METHODS 9
#define ALL_METHODS "abdehiopr"
#define SIZE_DEFAULT "Degree"
#define SIZE_DEFAULT_CHAR 'd'
#define COLOR_BASE "WalkTrapModularity"
#define PAGERANK_DAMPING 0.85 /**< chance random walk will not restart */
#define LAYOUT_DEFAULT_CHAR 'f'
#define MAX_NODES 50000 /**< number of nodes in graph before shut down */

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

struct Node {
  char* abbrev;
  igraph_real_t val;
  struct Node *next;
};

/** @struct RankNode
 @brief unimplemented struct for holding the top 20 rankids for the graph.
 */
struct RankNode {
  int rankids[20];
  struct RankNode *next;
};

static struct Node* EmptyNode;

struct stat;
struct Node* asshead;
struct Node* edges;
struct Node* density;
struct Node* betcent;
struct Node* reciprocity;
struct Node* degcent;
struct Node* idegcent;
struct Node* odegcent;
struct Node* eigcent;
struct Node* pagecent;
struct Node* diameter;
struct Node* pathlength;
struct Node* clustering;
struct Node* pv;
struct Node* ts;
struct RankNode* ranks;

int shuffle(int *array, int n);
/** adds a new value to a Node **/
int push(struct Node** head_ref, igraph_real_t value, char* attr);

/** adds a new value to a RankNode **/
int pushRank (struct RankNode** head_ref, int rankids[20]);
int igraph_i_xml_escape(char* src, char** dest);

int igraph_write_graph_gexf(const igraph_t *graph, FILE *outstream,
                            igraph_bool_t prefixattr);
igraph_real_t mean_vector (igraph_vector_t *v1);
igraph_real_t variance_vector (igraph_vector_t *v1);
igraph_real_t std_vector(igraph_vector_t *v1);
igraph_real_t stderror_vector(igraph_vector_t *v1);
igraph_real_t t_stat_vector(igraph_vector_t *v1);
igraph_real_t t_test_vector(igraph_vector_t *v1, igraph_real_t df);

int rankCompare(igraph_t *g1, igraph_t *g2, char* attr, igraph_real_t* result_pv, igraph_real_t* result_ts );
/** Writes the report **/
int write_report(igraph_t *graph);
int colors (igraph_t *graph);
int layout_graph(igraph_t *graph, char layout);
int set_size(igraph_t *graph, igraph_vector_t *v, int max);

int strip_ext(char *fname);
int load_graph (char* filename);
int write_graph(igraph_t *graph, char *attr);
int produceRank(igraph_vector_t *source, igraph_vector_t *vector);
int create_graph_csv(char* filepath, int start, int perc);
int paired_t_stat (igraph_vector_t *v1, igraph_vector_t *v2, igraph_real_t *pv, igraph_real_t *ts);
int calc_betweenness(igraph_t *graph);
int calc_authority(igraph_t *graph);
int calc_hub(igraph_t *graph);
int calc_pagerank(igraph_t *graph);
int calc_eigenvector(igraph_t *graph);
/* extern int calc_clustering(igraph_t *graph); */
/* extern int calc_random(igraph_t *graph); */
int calc_degree(igraph_t *graph, char type);
int calc_modularity(igraph_t *graph);
int centralization(igraph_t *graph, char* attr);
int analysis_all (igraph_t *graph);
int quickrunGraph();

float fix_percentile();
int create_filtered_graph(igraph_t *graph, double cutoff, int cutsize, char* attr);
int shrink (igraph_t *graph, int cutsize, char* attr);
int runFilters (igraph_t *graph, int cutsize);
int filter_graph();

#endif
