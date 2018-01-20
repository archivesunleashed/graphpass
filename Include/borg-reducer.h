#include "igraph.h"
#include <stdio.h>
#include <math.h>
#ifndef BORG_REDUCER
#define BORG_REDUCER
#define MAX_PERCENTILES 3
#define MAX_METHODS 9
#define ALL_METHODS 'bcdeioprw'

const igraph_t g;
igraph_attribute_table_t att;

extern int load_graph (char* filename) {
  printf("%s", filename);
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == 0) {
    return (1);
  }
  igraph_read_graph_graphml(&g, fp, 0);
  fclose(fp);
  return (0);
}

double fix_percentile(double percentile) {
  double perc;
  perc = (percentile > 99.0 || (percentile <= 1.0 && percentile > 0.99)) ? 0.99 : (percentile / 100.0);
  printf ("PERC: %f", perc);
  return perc;
}

extern int filter_betweenness(percentiles, filename){
  return 0;
}

extern int filter_pagerank(percentiles, filename){
  return 0;
}

extern int filter_eigenvector(percentiles, filename){
  return 0;
}

extern int filter_clustering(percentiles, filename){
  return 0;
}

extern int filter_random(percentiles, filename) {
  return 0;
}

extern int filter_degree(int expectedsize, char* filename, char type) {
  printf("FILTER DEGREE %i", expectedsize);
  char filtertype;
  char inorout;
  int graphsize;
  inorout = type == 'i' ? IGRAPH_IN : IGRAPH_OUT;
  filtertype = type == 'd' ? IGRAPH_ALL : inorout;
  printf("FILTERTYPE %c", filtertype);
  /* find the size of the graph and calculate expected size*/
  int i;
  for (i=0;i<3;i++) {

  }
  /* new graph */
  igraph_t graph;
  igraph_copy(&graph, &g);
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);
  /* calculate degree */
  igraph_degree(&g, &v, igraph_vss_all(), filtertype, IGRAPH_NO_LOOPS);

  /* create igraph iterator */
  igraph_vit_t vit;
  igraph_vs_t vs;
  igraph_vs_all(&vs);
  igraph_vit_create(&graph, vs, &vit);
  while (!IGRAPH_VIT_END(vit)) {
    printf("GRAPH ITERATOR: %li", (long int) IGRAPH_VIT_GET(vit));
  IGRAPH_VIT_NEXT(vit);
  }
  igraph_destroy(&graph);
  return (0);
}

extern int output_to_gexf (igraph_t *graph, FILE *outstream) {
  return (0);
}

/**
 * @brief Filters an igraph using one or more methods, and outputs graphs
 *   as derivatives of filename.
 * @param igraph_t graph - an igraph object
 * @param float[] percentiles - an array of up to 3 percentile values from 0.1 to 99
 *  representing the desired amount filtration.  If
 *  a percentile is greater than 0, it will divide by 100.
 * @param char [] method.  A string of up to 6 characters, one for each of
 * the following:
 *    b: betweenness algorithm
 *    c: clustering coefficients
 *    d: degree (plain)
 *    e: eigenvector
 *    i: indegree
 *    o: outdegree
 *    p: page-rank
 *    r: random filtration
 *    w: weighted degree
 * @param char[] filename  A filename to use for filtered graphs.  The values
 *   for percentiles and methods will be add to the filename per graph using
 *   (FILENAME)_(METHOD)_(PERCENTAGE)
 *
 * @return 0 unless an error is discovered
 */
extern int filter_graph(double percentile,
    char *method, char *filename) {
  printf("FILTER %s", filename);
  int p;
  int expectedsize;
  double graphsize;
  graphsize = (double)igraph_vcount(&g);
  percentile = (percentile > 0.99) ? fix_percentile(percentile) : percentile;
  expectedsize = round((double)graphsize * (1.0 - percentile));
  /* Methods are */
  int len = (int)strlen(method);
  for (int i=0; i < len; i++) {
    printf("METHOD: %s", method);
    switch (method[i]) {
      case 'b' : filter_betweenness(expectedsize, filename);
      printf("BETWEENNESS");
      break;
      case 'c' : filter_clustering(expectedsize, filename);
      break;
      case 'd' :
      case 'i' :
      case 'o' : filter_degree(expectedsize, filename, method);
      break;
      case 'e' : filter_eigenvector(expectedsize, filename);
      break;
      case 'p' : filter_pagerank (expectedsize, filename);
      break;
      case '\0' :
      break;
      default: printf("WARNING: Invalid parameter for method sent, ignoring.\n \
        This may affect your outputs.");
    }
  }
  return 0;
}
#endif
