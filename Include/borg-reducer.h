#include "igraph.h"
#include <stdio.h>
#include <math.h>
#ifndef BORG_REDUCER
#define BORG_REDUCER
#define MAX_PERCENTILES 3
#define MAX_METHODS 9
#define ALL_METHODS 'bcdeioprw'

igraph_t g;
igraph_attribute_table_t att;

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
  igraph_strvector_destroy(&enames);
  igraph_strvector_destroy(&vnames);
  igraph_strvector_destroy(&gnames);
  igraph_vector_destroy(&etypes);
  igraph_vector_destroy(&vtypes);
  igraph_vector_destroy(&gtypes);
  return 0;
}

extern int load_graph (char* filename) {
  printf("%s", filename);
  igraph_i_set_attribute_table(&igraph_cattribute_table);
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
  return perc;
}

extern int calc_betweenness(igraph_t *graph){
  printf("BETWEENNESS");
  char *attr = "Betweenness";
  int graphsize;
  graphsize = igraph_vcount(graph);
  printf("%i", graphsize);
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);
  igraph_betweenness(graph, &v, igraph_vss_all(), 1, 0, 1);
  SETVANV(graph, attr, &v);
  print_graph_attrs(graph);
  for (long i=0; i<graphsize; i++) {
    printf("Vertex %li: ", i);
    igraph_real_printf(VAN(graph, attr, i));
    putchar(' ');
    printf("\n");
  }
  return 0;
}

extern int calc_pagerank(igraph_t *graph){
  return 0;
}

extern int calc_eigenvector(igraph_t *graph){
  return 0;
}

extern int calc_clustering(igraph_t *graph){
  return 0;
}

extern int calc_random(igraph_t *graph) {
  return 0;
}

extern int calc_degree(igraph_t *graph, char type) {
  char filtertype;
  char inorout;
  char *attr;
  int graphsize;
  graphsize = igraph_vcount(graph);
  switch (type) {
    case 'i' :
      filtertype = IGRAPH_IN;
      attr = "Indegree";
      break;
    case 'o' :
      filtertype = IGRAPH_OUT;
      attr = "Outdegree";
      break;
    default :
      filtertype = IGRAPH_ALL;
      attr = "Degree";
  }

  printf ("%c ", filtertype);

  /* new graph */
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);

  /* calculate degree */
  igraph_degree(graph, &v, igraph_vss_all(), filtertype, IGRAPH_NO_LOOPS);
  SETVANV(graph, attr, &v);
  print_graph_attrs(graph);

  for (long i=0; i<graphsize; i++) {
    printf("Vertex %li: ", i);
	  igraph_real_printf(VAN(graph, attr, i));
	  putchar(' ');
    printf("\n");
  }
  igraph_vector_destroy(&v);
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
  int flag = 0;
  while (flag > -1) {
    switch (method[flag]) {
      case 'b' : calc_betweenness(&g);
      printf("BETWEENNESS \n");
      break;
      case 'c' : calc_clustering(&g);
      printf("CLUSTERING \n");
      break;
      case 'd' :
      case 'i' :
      case 'o' : calc_degree(&g, method[flag]);
      break;
      case 'e' : calc_eigenvector(&g);
      printf("EIGENVECTOR \n");
      break;
      case 'p' : calc_pagerank (&g);
      printf("PAGE RANK \n");
      break;
      case '\0' :
      flag = -2;
      break;
      default: printf("WARNING: Invalid parameter for method sent, ignoring.\n \
        This may affect your outputs.");
    }
    ++flag;
  }
  igraph_destroy(&g);
  return 0;
}

#endif
