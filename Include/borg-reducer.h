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

int print_graph_attrs () {
  igraph_vector_t gtypes, vtypes, etypes;
  igraph_strvector_t gnames, vnames, enames;
  igraph_vector_init(&gtypes, 0);
  igraph_vector_init(&vtypes, 0);
  igraph_vector_init(&etypes, 0);
  igraph_strvector_init(&gnames, 0);
  igraph_strvector_init(&vnames, 0);
  igraph_strvector_init(&enames, 0);
  igraph_cattribute_list(&g, &gnames, &gtypes, &vnames, &vtypes,
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

extern int filter_betweenness(int expectedsize){
  return 0;
}

extern int filter_pagerank(int expectedsize){
  return 0;
}

extern int filter_eigenvector(int expectedsize){
  return 0;
}

extern int filter_clustering(int expectedsize){
  return 0;
}

extern int filter_random(int expectedsize) {
  return 0;
}

extern int filter_degree(int expectedsize, char type) {
  char filtertype;
  char inorout;
  char *attr;
  int graphsize;

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

  /* new graph */
  igraph_t graph;
  igraph_copy(&graph, &g);
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);

  /* calculate degree */
  igraph_degree(&g, &v, igraph_vss_all(), filtertype, IGRAPH_NO_LOOPS);
  SETVANV(&g, attr, &v);
  print_graph_attrs();

  /* create igraph iterator */
  igraph_vit_t vit;
  igraph_vs_t vs;
  igraph_vs_all(&vs);
  igraph_vit_create(&graph, vs, &vit);

  for (long i=0; i<igraph_vcount(&g); i++) {
    printf("Vertex %li: ", i);
	  igraph_real_printf(VAN(&g, attr, i));
	  putchar(' ');
    printf("\n");
  }

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
  int flag = 0;
  while (flag > -1) {
    switch (method[flag]) {
      case 'b' : filter_betweenness(expectedsize);
      printf("BETWEENNESS \n");
      break;
      case 'c' : filter_clustering(expectedsize);
      printf("CLUSTERING \n");
      break;
      case 'd' : printf("DEGREE \n");
      case 'i' : printf("IN-DEGREE \n");
      case 'o' : filter_degree(expectedsize, method[flag]);
        printf("OUT-DEGREE \n");
      break;
      case 'e' : filter_eigenvector(expectedsize);
      printf("EIGENVECTOR \n");
      break;
      case 'p' : filter_pagerank (expectedsize);
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
