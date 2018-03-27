/* -*- Nutil -- Network Graph Utilities mode: C -*-  */
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

#include <graphpass.h>

/** @file filter.c
 @brief Basic filtering utilities
 */



/** Automatically converts percentage to decimal values to fit filter model.
 
 If the --percent or -p flag is greater than 1 and less than 99, fix_percentile 
 will divide by 100, otherwise it will return 0.99 (99 percent).
 
 @return 0.99 >= A float value >= 0.01
 */

float fix_percentile() {
  if (PERCENT == 0.0) {return 0.0;}
  float perc;
  perc = (PERCENT > 99.0 || (PERCENT < 1.0 && PERCENT > 0.99)) ? 0.99 : (PERCENT / 100.0);
  return perc;
}

/** Create a graph from an original graph with a number of nodes equal to
   cutsize.
 
  @param graph - the graph to filter
  @param cutoff - the value to use as a cutoff value.
  @param cutsize - the size of the requested graph.
  @param attr - the method used to shorten the graph
 
  @return 0 unless an error occurs.
 */
int create_filtered_graph(igraph_t *graph, double cutoff, int cutsize, char* attr) {
  srand(time(NULL));
  int checkFewer = 0;
  int checkEqual = 0;
  /* a View containing the ids to keep */
  igraph_vector_t grands;
  igraph_vs_t selector;
  igraph_t g2;
  igraph_copy(&g2, graph);
  /* the ids to cut */
  double cut[cutsize];
  /** Random filtering is most basic */
  if (strcmp(attr, "Random") == 0) {
    int precut[NODESIZE];
    /* remove cutsize based on shuffle */
    for (long int i=0; i<NODESIZE; i++) {
      precut[i] = i;
    }
    shuffle(precut, NODESIZE);
    for (long int j=0; j<cutsize; j++) {
      cut[j] = precut[j];
    }
  } else {
    /* check the number of values less than (checkFewer) or equal (checkEqual) to
     the assigned cutoff value */
    for (long int i=0; i<NODESIZE; i++) {
      if (VAN(graph, attr, i) < cutoff) {
        ++checkFewer;  // number of nodes fewer than cutoff
      } else if (VAN(graph, attr, i) == cutoff){
        ++checkEqual; // number of nodes equal to cutoff
      }
    }
    for (long int i=0; i<cutsize; i++) {
      cut[i] = -1.0;
    }
    int equal[checkEqual];
    if (checkFewer == cutsize) {
      /* if number of equals and less thans are all needed then just do the filter */
      int index = 0;
      for (long int i=0; i<NODESIZE; i++) {
        if (VAN(graph, attr, i) < cutoff) {
          cut[index] = (double)i;
          ++index;
        }
      }
    }
    else if (checkFewer == 0) {
      /* The only values to cut are equal to the cutoff */
      printf("  ---WARNING--- :  Percentage resulted in ambiguous filtering \n");
      printf("    because no values were lower than cutoff point %f \n", cutoff);
      printf("    This means that all values that equal the cutoff point will be selected randomly.\n");
      int rands = 0;
      for (long int i=0; i<NODESIZE; i++) {
        if (VAN(graph, attr, i) == cutoff) {
          equal[rands] = i;
          ++rands;
        }
      }
      shuffle(equal, rands);
      for (long int i=0; i<cutsize; i++) {
        cut[i] = equal[i];
      }
    } else {
      /* means we have to randomize selection for val == cutoff */
      int randoms = (cutsize - checkFewer);
      printf ("  ---WARNING--- :  Percentage resulted in ambiguous filtering.\n");
      printf( "    This means that %i values at cutoff point %f \n will be selected randomly.\n",
        randoms, cutoff);
      int index = 0;
      int rands = 0;
      for (long int i=0; i<NODESIZE; i++) {
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
        if (cut[ind] == -1) {
          cut[ind] = (double)equal[randind];
          ++randind;
          ++ind;
        } else {
          ++ind;
        }
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
  
  colors(&g2);
  igraph_vector_t size;
  igraph_vector_init(&size, cutsize);
  igraph_vector_t rank;
  igraph_vector_init(&rank, igraph_vcount(&g2));
  
  igraph_vector_t ideg;
  igraph_vector_t odeg;
  igraph_vector_init(&ideg, cutsize);
  igraph_vector_init(&odeg, cutsize);
  
  VANV(&g2, "Degree", &size);
  VANV(&g2, "Indegree", &ideg);
  VANV(&g2, "Outdegree", &odeg);
  // print size here.
  produceRank(&size, &rank);
  printf("finishProduce Rank");
  SETVANV(&g2, "DegreeRank", &rank);
  set_size(&g2, &size, 100);
  centralization(&g2, "Betweenness");
  centralization(&g2, "PageRank");
  centralization(&g2, "Degree");
  centralization(&g2, "Eigenvector");
  igraph_real_t pathl, cluster, assort, dens, recip, pvals, tsco;
  igraph_integer_t dia;
  igraph_average_path_length(graph, &pathl, 1, 1);
  igraph_diameter(&g2, &dia, NULL, NULL, NULL ,1, 1);
  /* get Rankings
   int ranks[20];
   igraph_vector_t eids, sorted;
   igraph_vector_init(&eids, 0);
   igraph_vector_init(&sorted, 0);
   VANV(&g2, "id", &eids);
   check = -1;
   Need to develop sort id function here
   while (check <0) {
   ++check;
   } */
  
  igraph_transitivity_undirected(&g, &cluster, IGRAPH_TRANSITIVITY_ZERO);
  igraph_assortativity(&g2, &ideg, &odeg, &assort, 1);
  igraph_density(&g2, &dens, 0);
  igraph_reciprocity(&g2, &recip, 1, IGRAPH_RECIPROCITY_DEFAULT);
  SETGAN(&g2, "NODES", igraph_vcount(&g2));
  SETGAN(&g2, "EDGES", igraph_ecount(&g2));
  SETGAN(&g2, "AVG_PATH_LENGTH", pathl);
  SETGAN(&g2, "DIAMETER", dia);
  SETGAN(&g2, "OVERALL_CLUSTERING", cluster);
  SETGAN(&g2, "ASSORTATIVITY", assort);
  SETGAN(&g2, "DENSITY", dens);
  SETGAN(&g2, "RECIPROCITY", recip);
  if (SAVE == true) {
    write_graph(&g2, attr);
  }
  push(&asshead, assort, attr);
  push(&edges, GAN(&g2, "EDGES"), attr);
  push(&density, dens, attr);
  push(&diameter, dia, attr);
  push(&pathlength, pathl, attr);
  push(&clustering, cluster, attr);
  push(&betcent, GAN(&g2, "centralizationBetweenness"), attr);
  push(&degcent, GAN(&g2, "centralizationDegree"), attr);
  push(&idegcent, GAN(&g2, "centralizationIndegree"), attr);
  push(&odegcent, GAN(&g2, "centralizationOutdegree"), attr);
  push(&eigcent, GAN(&g2, "centralizationEigenvector"), attr);
  push(&pagecent, GAN(&g2, "centralizationPageRank"), attr);
  push(&reciprocity, recip, attr);
  rankCompare(&g, &g2, "Degree", &pvals, &tsco);
  push(&pv, pvals, attr);
  push(&ts, tsco, attr);
  igraph_vector_destroy(&size);
  igraph_vector_destroy(&rank);
  igraph_vector_destroy(&ideg);
  igraph_vector_destroy(&odeg);
  igraph_vs_destroy(&selector);
  igraph_destroy(&g2);
  return 0;
}

int shrink (igraph_t *graph, int cutsize, char* attr) {
  igraph_vector_t v;
  if (strcmp(attr, "Random")==0) {
    create_filtered_graph(graph, 0.0, cutsize, attr);
  } else {
    igraph_vector_init(&v, NODESIZE);
    for (long i=0; i<NODESIZE; i++) {
      VECTOR(v)[i] = VAN(graph, attr, i);
    }
    igraph_vector_sort(&v);
    create_filtered_graph(graph, VECTOR(v)[cutsize], cutsize, attr);
  }
  igraph_vector_destroy(&v);
  return 0;
}

int runFilters (igraph_t *graph, int cutsize) {
  int flag = 0;
  while (flag > -1) {
    switch (METHODS[flag]) {
      case 'a' : shrink(graph, cutsize, "Authority");
        break;
      case 'b' : shrink(graph, cutsize, "Betweenness");
        break;
      case 'c' : shrink(graph, cutsize, "Clustering");
        break;
      case 'd' : shrink(graph, cutsize, "Degree");
        break;
      case 'h' : shrink(graph, cutsize, "Hub");
        break;
      case 'i' : shrink(graph, cutsize, "Indegree");
        break;
      case 'o' : shrink(graph, cutsize, "Outdegree");
        break;
      case 'e' : shrink(graph, cutsize, "Eigenvector");
        break;
      case 'p' : shrink(graph, cutsize, "PageRank");
        break;
      case 'r' : shrink(graph, cutsize, "Random");
        break;
      case '\0' :
        flag = -2;
        break;
      default: printf("---WARNING--- : Invalid parameter for method sent, ignoring.\n \
                      This may affect your outputs.\n\n");
    }
    ++flag;
  }
  
  return 0;
}

/** Filters an igraph using one or more methods based on global "METHODS", and outputs graphs as derivatives of filename.
 
  @return 0 unless an error is discovered
 */

int filter_graph() {
  igraph_vector_t idRef;
  igraph_vector_init_seq(&idRef, 0, igraph_vcount(&g)-1);
  float percentile;
  int cutsize;
  if (QUICKRUN == true) {
    printf("\n\nQuickrun requested.\n\n");
    printf("Quickrun does no filtering, and provides layout information\n");
    printf("based on Degree (nodesize), Walktrap Modularity (color), and\n");
    printf("the Fructerman-Rheingold algorithm to maximize space between\n");
    printf("nodes.\n\n");
    printf("Quickrun is quicker, but less informative in terms of output.\n");
    quickrunGraph();
    igraph_destroy(&g);
    return(0);
  }
  /* if (CALC_WEIGHTS == false) {igraph_vector_init(&WEIGHTED, NODESIZE);}*/
  percentile = (PERCENT > 0.99) ? fix_percentile() : PERCENT;
  cutsize = round((double)NODESIZE * percentile);
  printf("Filtering the graphs by %f will reduce the graph size by %d \n", PERCENT, cutsize);
  printf("This will produce a graph with %d nodes.\n", (NODESIZE - cutsize));
  SETVANV(&g, "idRef", &idRef);
  analysis_all(&g);
  runFilters(&g, cutsize);
  if (REPORT == true) {
    write_report(&g);
  }
  igraph_destroy(&g);
  igraph_vector_destroy(&idRef);
  return 0;
}




