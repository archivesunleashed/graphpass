/* -*- Graphpass -- Network Graph Utilities mode: C -*-  */
/*
 Copyright [2018] [Ryan Deschamps]
 
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

/** @file analyze.c
    @brief Provides basic network analysis and adds them to the graph output.
 
    Analyze.h contains the main functions for analysing network graphs.  These
    functions are used by other files (filter, or viz, for instance) to 
    determine such things as what to filter, how to produce colors, and 
    what size nodes should be.
 */

/** Calculates betweenness scores for the individual nodes in a graph
 
 Betweenness is measured
 
 @param graph - the graph for which to record the scores.
 @return 0 unless error occurs.
 */

extern int calc_betweenness(igraph_t *graph){
  char *attr = "Betweenness";
  igraph_vector_t v;
  igraph_vector_init(&v, igraph_vcount(graph));
  igraph_betweenness(graph, &v, igraph_vss_all(), igraph_is_directed(graph), NULL, 1);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return 0;
}

/** Calculates authority scores for the individual nodes in a graph
 
 Authority is measured
 
 @param graph - the graph for which to record the scores.
 */

extern int calc_authority(igraph_t *graph){
  char *attr = "Authority";
  igraph_arpack_options_t options;
  igraph_arpack_options_init(&options);
  igraph_vector_t v;
  igraph_vector_init(&v, igraph_vcount(graph));
  igraph_authority_score(graph, &v, NULL, 1, 0, &options);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return 0;
}

/** Calculates hub scores for the individual nodes in a graph
 
 Hub is measured
 
 @param graph - the graph for which to record the scores.
 */

extern int calc_hub(igraph_t *graph){
  char *attr = "Hub";
  igraph_arpack_options_t options;
  igraph_arpack_options_init(&options);
  igraph_vector_t v;
  igraph_vector_init(&v, igraph_vcount(graph));
  igraph_hub_score(graph, &v, NULL, 1, 0, &options);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return 0;
}

/** Calculates pagerank scores for the individual nodes in a graph
 
 Pagerank is measured
 
 @param graph - the graph for which to record the scores.
 */
extern int calc_pagerank(igraph_t *graph){
  char *attr = "PageRank";
  igraph_vector_t v;
  igraph_vector_init(&v, igraph_vcount(graph));
  igraph_pagerank(graph, IGRAPH_PAGERANK_ALGO_PRPACK, &v, 0,
                  igraph_vss_all(), 1, 0.85, 0, 0);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return 0;
}

/** Calculates eigenvector centrality scores for the individual nodes in a graph
 
 Authority is measured
 
 @param graph - the graph for which to record the scores.
 */
extern int calc_eigenvector(igraph_t *graph){
  char *attr = "Eigenvector";
  igraph_arpack_options_t options;
  igraph_arpack_options_init(&options);
  igraph_vector_t v;
  igraph_vector_init(&v, igraph_vcount(graph));
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

/** Calculates degree scores for the individual nodes in a graph
 
 Authority is measured
 
 @param graph - the graph for which to record the scores.
 @param type - 'i' for indegree, 'o' for outdegree, 'd' for degree
 */
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
  igraph_vector_init(&v, igraph_vcount(graph));
  igraph_degree(graph, &v, igraph_vss_all(), filtertype, IGRAPH_NO_LOOPS);
  SETVANV(graph, attr, &v);
  igraph_vector_destroy(&v);
  return (0);
}

/** Calculates walktrap modularity scores for the individual nodes in a graph
 and sets group values for each.
 
 Walktrap modularity is measured
 
 @param graph - the graph for which to record the scores.
 */
extern int calc_modularity(igraph_t *graph) {
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

/** Calculates betweenness scores for the individual nodes in a graph
 
 Centralization is a graph-level score created by comparing the centrality
 values (betweenness, degree, eigenvector etc.) of the nodes in a graph 
 against the node with the highest centrality.  A "star" graph has 
 a centralization of 1, whereas a "full" graph (everyone is connected to
 everyone) has a centralization of 0.
 
 In general it identifies the influence of the most central nodes on the 
 entire graph.
 
 @param graph - the graph for which to record the scores.
 @param attr - the graph attribute for which the centralization score is based
 on.
 */
extern int centralization(igraph_t *graph, char* attr) {
  long int gsize = (long int)igraph_vcount(graph);
  int directed = igraph_is_directed(graph) ? 1 : 2;
  int maximum = (int) (gsize * (gsize -1))/directed;
  igraph_vector_t scores;
  char gattr[150];
  strcpy(gattr, "centralization");
  strcat(gattr, attr);
  igraph_vector_init (&scores, NODESIZE);
  VANV(graph, attr, &scores);
  igraph_real_t results = 0.0;
  results = igraph_centralization(&scores, maximum, 1);
  SETGAN(graph, gattr, results);
  igraph_vector_destroy(&scores);
  return 0;
}

/** Creates a rank-order from a vector of values 
 
 produceRank taks a source vector and produces ranks for the values in the 
 that they occur in source.
 @param source - An igraph vector containing the source data to rank.
 @param vector - An initialized igraph_vector_t to contain the ranks.
 **/

int produceRank(igraph_vector_t *source, igraph_vector_t *v) {
  long int source_size;
  source_size = igraph_vector_size(source);
  igraph_vector_t source_cpy, rank_vals;
  igraph_vector_init(&source_cpy, source_size);
  igraph_vector_init(&rank_vals, source_size);
  igraph_vector_copy(&source_cpy, source);
  igraph_vector_sort(&source_cpy);
  igraph_vector_reverse(&source_cpy);
  for (long int i=0; i < source_size; i++){
    if (i == 0) {
      VECTOR(rank_vals)[0] = 1;
    } else if (VECTOR(source_cpy)[i] == VECTOR(source_cpy)[i-1]) {
      VECTOR(rank_vals)[i] = VECTOR(rank_vals)[i-1];
    } else {
      VECTOR(rank_vals)[i] = i+1;
    }
  }
  for (long int i=0; i < source_size; i++) {
    long int j = 0;
    while (j< source_size) {
      if (igraph_vector_e(source, i) == VECTOR(source_cpy)[j]) {
        igraph_vector_set(v,i,VECTOR(rank_vals)[j]);
        break;
      }
      j++;
    }
  }
  igraph_vector_destroy(&rank_vals);
  igraph_vector_destroy(&source_cpy);
  return 0;
}


/** Calculates the main analysis scores for the graph

 analysis_all conducts all available graph, node and edge scores available
 in the network utility. It is very expensive in terms of processing time, 
 so should be used only for smaller graphs.
 
 @param graph - the graph for which to record the scores.
 */
extern int analysis_all (igraph_t *graph) {
  calc_authority(graph);
  calc_betweenness(graph);
  calc_degree(graph, 'd');
  calc_hub(graph);
  calc_degree(graph, 'i');
  calc_degree(graph, 'o');
  calc_eigenvector(graph);
  calc_pagerank (graph);
  igraph_real_t pathl, cluster, assort, dens, recip;
  igraph_integer_t dia;
  igraph_vector_t mod;
  igraph_vector_init (&mod, igraph_vcount(graph));
  igraph_average_path_length(graph, &pathl, 1, 1);
  igraph_diameter(graph, &dia, NULL, NULL, NULL ,1, 1);
  igraph_transitivity_undirected(graph, &cluster, IGRAPH_TRANSITIVITY_ZERO);
  calc_modularity(graph);
  VANV(graph, "WalkTrapModularity", &mod);
  igraph_assortativity_nominal(graph, &mod, &assort, 1);
  igraph_density(graph, &dens, 0);
  igraph_reciprocity(graph, &recip, 1, IGRAPH_RECIPROCITY_DEFAULT);
  SETGAN(graph, "NODES", igraph_vcount(graph));
  SETGAN(graph, "EDGES", igraph_ecount(graph));
  SETGAN(graph, "AVG_PATH_LENGTH", pathl);
  SETGAN(graph, "DIAMETER", dia);
  SETGAN(graph, "OVERALL_CLUSTERING", cluster);
  SETGAN(graph, "ASSORTATIVITY", assort);
  SETGAN(graph, "DENSITY", dens);
  SETGAN(graph, "RECIPROCITY", recip);
  centralization(graph, "Authority");
  centralization(graph, "Betweenness");
  centralization(graph, "Degree");
  centralization(graph, "Hub");
  centralization(graph, "Indegree");
  centralization(graph, "Outdegree");
  centralization(graph, "Eigenvector");
  centralization(graph, "PageRank");
  igraph_vector_destroy(&mod);
  return 0;
}

