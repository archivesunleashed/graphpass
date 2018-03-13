/* -*- Nutil -- Network Graph Utilities mode: C -*-  */
/*
 Copyright <2018> <Ryan Deschamps> <ryan.deschamps@gmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

/** @file analyze.h
    \brief Provides basic network analysis and adds them to the graph output.
 
    Analyze.h contains the main functions for analysing network graphs.  These
    functions are used by other files (filter, or viz, for instance) to 
    determine such things as what to filter, how to produce colors, and 
    what size nodes should be.
 */

/*! \fn int calc_betweenness 
    \brief Calculates betweenness scores for the individual nodes in a graph
 
    Betweenness is measured 
 
    @param graph - the graph for which to record the scores.
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

/*! \fn int calc_betweenness
 \brief Calculates betweenness scores for the individual nodes in a graph
 
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

/*! \fn int calc_betweenness
 \brief Calculates betweenness scores for the individual nodes in a graph
 
 Authority is measured
 
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

/*! \fn int calc_betweenness
 \brief Calculates betweenness scores for the individual nodes in a graph
 
 Authority is measured
 
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

/*! \fn int calc_betweenness
 \brief Calculates betweenness scores for the individual nodes in a graph
 
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

/*! \fn int calc_betweenness
 \brief Calculates betweenness scores for the individual nodes in a graph
 
 Authority is measured
 
 @param graph - the graph for which to record the scores.
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

/*! \fn int calc_betweenness
 \brief Calculates betweenness scores for the individual nodes in a graph
 
 Authority is measured
 
 @param graph - the graph for which to record the scores.
 */
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

/*! \fn int centraliztion
 \brief Calculates betweenness scores for the individual nodes in a graph
 
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
int centralization(igraph_t *graph, char* attr) {
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


/** \fn int analysis_all(igraph_t *graph)
 \brief Calculates the main analysis scores for the graph

 analysis_all conducts all available graph, node and edge scores available
 in the network utility. It is very expensive in terms of processing time, 
 so should be used only for smaller graphs.
 
 @param graph - the graph for which to record the scores.
 */
int analysis_all (igraph_t *graph) {
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


