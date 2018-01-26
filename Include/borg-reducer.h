#include "igraph.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#ifndef BORG_REDUCER
#define BORG_REDUCER
#define MAX_PERCENTILES 3
#define MAX_METHODS 9
#define ALL_METHODS 'abcdehioprw'
#define PAGERANK_DAMPING 0.85

igraph_t g;
igraph_attribute_table_t att;
char* filename;
char* method;
char* output;
long int graphsize;
double percent;

extern int init(char* file, char* meth, char* out) {
  filename = file;
  method = meth;
  output = out;
  return 0;
}

int write_graph(igraph_t *graph, char* output, char* method, char* filename) {
  FILE *fp;
  struct stat st = {0};
  if (stat(output, &st) == -1) {
    mkdir(output, 0700);
  }
  char path[150];
  char perc_as_string[3];
  int perc = (int)percent;
  snprintf(perc_as_string, 3, "%d", perc);
  strcpy(path, output);
  strcat(path, filename);
  strcat(path, perc_as_string);
  strcat(path, method);
  strcat(path, ".graphml");
  fp = fopen(path, "w");
  igraph_write_graph_graphml(graph, fp, 1);
  fclose(fp);
  return 0;
}

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

static int rand_int(int n) {
  int limit = RAND_MAX - RAND_MAX % n;
  int rnd;
  do {
      rnd = rand();
  }
  while (rnd >= limit);
  return rnd % n;
}

void shuffle(int *array, int n) {
  int i, j, tmp;
  for (i = n - 1; i > 0; i--) {
      j = rand_int(i + 1);
      tmp = array[j];
      array[j] = array[i];
      array[i] = tmp;
   }
}

extern int load_graph (char* filename) {
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
  char *attr = "Betweenness";
  printf("%li", graphsize);
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);
  igraph_betweenness(graph, &v, igraph_vss_all(), 1, 0, 1);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return 0;
}

extern int calc_pagerank(igraph_t *graph){
  char *attr = "PageRank";
  printf("%li", graphsize);
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);
  igraph_pagerank(graph, IGRAPH_PAGERANK_ALGO_PRPACK, &v, 0,
    igraph_vss_all(), 1, 0.85, 0, 0);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return 0;
}

extern int calc_eigenvector(igraph_t *graph){
  char *attr = "Eigenvector";
  igraph_arpack_options_t options;
  igraph_arpack_options_init(&options);
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);
  igraph_eigenvector_centrality(graph, &v, 0,
    1, 1, 0, &options);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
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
  char *attr;
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
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);
  igraph_degree(graph, &v, igraph_vss_all(), filtertype, IGRAPH_NO_LOOPS);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return (0);
}

int calc_modularity(igraph_t *graph) {
  char* attr = "WalkTrapModularity";
  igraph_vector_t v;
  igraph_vector_t classes;
  igraph_matrix_t merges;
  igraph_vector_init(&classes, 0);
  igraph_vector_init(&v, 0);
  igraph_matrix_init(&merges, 0, 0);
  igraph_community_walktrap(graph, 0 /* no weights */, 4 /* steps */, &merges,
      &v, &classes);
  SETVANV(graph, attr, &classes);
  igraph_vector_destroy(&v);
  igraph_matrix_destroy(&merges);
  igraph_vector_destroy(&classes);
  return 0;
}

int layout_graph(igraph_t *graph, char layout) {
  igraph_matrix_t matrix;
  long int gsize = (long int)igraph_vcount(graph);
  igraph_vector_t x, y, min, max;
  igraph_vector_init(&x, 0);
  igraph_vector_init(&y, 0);
  igraph_vector_init(&min, gsize);
  igraph_vector_init(&max, gsize);
  igraph_vector_fill(&min, -1000);
  igraph_vector_fill(&max, 1000);
  igraph_matrix_init(&matrix, gsize, 2);
  switch (layout) {
    case 'k' : igraph_layout_kamada_kawai(graph, &matrix,
			       1000, gsize/0.5,
			       10, 0.98,
			       gsize, 0,
			       &min,
			       &max,
			       &min,
			       &max);
    break;
    case 'f' : igraph_layout_fruchterman_reingold(graph, &matrix,
             500, gsize,
             gsize^2,
             1.5,
             gsize^3,
             0,
             NULL,
           &min, &max, &min, &max);
    break;
    default: igraph_layout_lgl(graph, &matrix,
    150, gsize, gsize, 1.5, gsize^3, sqrt(gsize), -1);
           }
  igraph_matrix_get_col(&matrix, &x, 0);
  igraph_matrix_get_col(&matrix, &y, 1);
  igraph_vector_scale(&x, 100);
  igraph_vector_scale(&y, 100);
  SETVANV(graph, "x", &x);
  SETVANV(graph, "y", &y);
  igraph_vector_destroy(&x);
  igraph_vector_destroy(&y);
  igraph_matrix_destroy(&matrix);
  return 0;
}

int set_size(igraph_t *graph, igraph_vector_t *v, int max) {
  long int gsize = (long int)igraph_vcount(graph);
  igraph_vector_t v2;
  igraph_vector_t min;

  double scale;
  igraph_vector_copy(&v2, v);
  igraph_vector_init(&min, gsize);
  igraph_vector_fill(&min, igraph_vector_min(&v2));
  igraph_vector_sub(&v2, &min);
  scale = gsize / (igraph_vector_max(&v2) - igraph_vector_min(&v2));
  igraph_vector_scale(&v2, scale);
  SETVANV(graph, "size", &v2);
  return 0;
}

extern int output_to_gexf (igraph_t *graph, FILE *outstream) {
  return (0);
}

int create_filtered_graph(igraph_t *graph, double cutoff, int cutsize,
  char* output, char* attr) {
  srand(time(NULL));
  int check = 0;
  int check2 = 0;
  igraph_vector_t grands;
  igraph_vs_t selector;
  for (long int i=0; i<graphsize; i++) {
    if (VAN(graph, attr, i) < cutoff) {
      ++check;
    } else if (VAN(graph, attr, i) == cutoff){
      ++check2;
    }
  }
  printf("CHECK %d -- CHECK2 %d", check, check2);
  double cut[cutsize];
  for (long int i=0; i<cutsize; i++) {
    cut[i] = (double)cutsize+100;
  }
  int equal[check2];
  igraph_t g2;
  igraph_copy(&g2, graph);
  if (check == 0) {
    printf ("WARNING :  Percentage resulted in ambiguous filtering\n \
because no values were lower than cutoff point %f (only equal to) This means\n \
that all values at cutoff point will be selected randomly.\n", cutoff);
    int rands = 0;
    for (long int i=0; i<graphsize; i++) {
      if (VAN(graph, attr, i) == cutoff) {
        equal[rands] = i;
        ++rands;
      }
    }
    shuffle(equal, rands);
    for (long int i=0; i<cutsize; i++) {
      cut[i] = equal[i];
    }
  } else if (check == cutsize) {
  /* if number of equals and less thans are all needed then just do the filter */
    int index = 0;
    for (long int i=0; i<graphsize; i++) {
      if (VAN(graph, attr, i) < cutoff) {
        cut[index] = (double)i;
        ++index;
      }
    }
  } else {
   /* means we have to randomize selection for val == cutoff */
    int randoms = (cutsize - check);
    printf ("WARNING :  Percentage resulted in ambiguous filtering.\n This means \
      that %i values at cutoff point %f will be selected randomly.\n", randoms, cutoff);
    int index = 0;
    int rands = 0;
    for (long int i=0; i<graphsize; i++) {
      if (VAN(graph, attr, i) < cutoff) {
        cut[index] = (double)i;
        ++index;
      } else if (VAN(graph, attr, i) == cutoff) {
        equal[rands] = (double)i;
        ++rands;
      }
    }
    /* second pass add items that equal the cutoff */
    int ind = 0;
    int randind = 0;
    shuffle(equal, rands);
    for (long int i=0; i<cutsize; i++) {
      if (cut[ind] == (double)(cutsize + 100)) {
        cut[ind] = (double)equal[randind];
        ++randind;
        ++ind;
      } else {
        ++ind;
      }
    }
  }
  igraph_vector_view(&grands, cut, cutsize);
  igraph_vs_vector(&selector, &grands);
  igraph_delete_vertices(&g2, selector);
  layout_graph(&g2, 'f');
  calc_degree(&g2, 'd');
  calc_degree(&g2, 'i');
  calc_degree(&g2, 'o');
  calc_betweenness (&g2);
  calc_eigenvector (&g2);
  calc_pagerank (&g2);
  calc_modularity(&g2);

  igraph_vector_t size;
  igraph_vector_init(&size, cutsize);
  VANV(&g2, "Degree", &size);
  set_size(&g2, &size, 100);

  write_graph(&g2, output, attr, filename);
  igraph_vector_destroy(&size);
  igraph_vs_destroy(&selector);
  igraph_destroy(&g2);
  return 0;
}

int shrink (igraph_t *graph, int cutsize, char* attr) {
  igraph_vector_t v;
  igraph_vector_init(&v, graphsize);
  double cut;
  for (long i=0; i<graphsize; i++) {
    VECTOR(v)[i] = VAN(graph, attr, i);
  }
  igraph_vector_sort(&v);
  create_filtered_graph(graph, VECTOR(v)[cutsize], cutsize, "OUT/", attr);
  igraph_vector_destroy(&v);
  return 0;
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
  int p;
  int cutsize;
  graphsize = (long int)igraph_vcount(&g);
  percentile = (percentile > 0.99) ? fix_percentile(percentile) : percentile;
  percent = percentile *100;
  if (percent > graphsize) {
    printf ("The percentage you provided (%f) is greater than the number \n\
    of nodes (%li) in the graph.  Select another percentage.", percent, graphsize);
    return 0;
  }
  printf("PERCENTILE :  %f, PERCENT : %f", percentile, percent);

  cutsize = round((double)graphsize * percentile);
  /* Methods are */
  /* TODO:  MOVE THIS TO A THE ALGORITHM FUNCTION */
  int flag = 0;
  while (flag > -1) {
    switch (method[flag]) {
      case 'b' : calc_betweenness(&g);
      shrink(&g, cutsize, "Betweenness");
      break;
      case 'c' : calc_clustering(&g);
      break;
      case 'd' : calc_degree(&g, 'd');
      shrink(&g, cutsize, "Degree");
      break;
      case 'i' : calc_degree(&g, 'i');
      shrink(&g, cutsize, "Indegree");
      break;
      case 'o' : calc_degree(&g, 'o');
      shrink(&g, cutsize, "Outdegree");
      break;
      case 'e' : calc_eigenvector(&g);
      shrink(&g, cutsize, "Eigenvector");
      break;
      case 'p' : calc_pagerank (&g);
      shrink(&g, cutsize, "PageRank");
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
