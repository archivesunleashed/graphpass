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
#include <math.h>

/** @file analyze.c
    @brief Provides basic network analysis and adds them to the graph output.
 
    Analyze.h contains the main functions for analysing network graphs.  These
    functions are used by other files (filter, or viz, for instance) to 
    determine such things as what to filter, how to produce colors, and 
    what size nodes should be.
 */

igraph_real_t mean_vector (igraph_vector_t *v1) {
  return (igraph_vector_sum(v1)/igraph_vector_size(v1));
}

igraph_real_t variance_vector (igraph_vector_t *v1) {
  igraph_real_t mean = mean_vector(v1);
  igraph_vector_t squared_residuals;
  igraph_vector_init(&squared_residuals, igraph_vector_size(v1));
  igraph_real_t variance;
  for (long int i=0; i<igraph_vector_size(v1); i++) {
    VECTOR(squared_residuals)[i] = pow((igraph_vector_e(v1, i) - mean), 2);
  }
  variance = (igraph_vector_sum(&squared_residuals) / (igraph_vector_size(&squared_residuals) -1));
  // igraph_vector_destroy(&squared_residuals);
  return variance;
}

igraph_real_t std_vector(igraph_vector_t *v1) {
  return sqrt(variance_vector(v1));
}

igraph_real_t stderror_vector(igraph_vector_t *v1) {
  return (std_vector(v1) / sqrt(igraph_vector_size(v1)));
}
          
igraph_real_t t_stat_vector(igraph_vector_t *v1) {
  return (mean_vector(v1)/stderror_vector(v1));
}

igraph_real_t t_test_vector(igraph_vector_t *v1, igraph_real_t df) {
  igraph_real_t t_stat;
  igraph_real_t pvalue;
  t_stat = t_stat_vector(v1);
  pvalue = (df / (t_stat * t_stat + df));
  if ((isinf(pvalue) != 0) || (isnan(pvalue) != 0)) {
    return 1.0;
  }
  if ((isinf(pvalue) != 0) || (isnan(pvalue) != 0)) {
    return 1.0;
  }
  
  const double daf = df/2;
  /*  Purpose:
   
   BETAIN computes the incomplete Beta function ratio.
   
   Licensing:
   
   This code is distributed under the GNU LGPL license.
   
   Modified:
   
   05 November 2010
   
   Author:
   
   Original FORTRAN77 version by KL Majumder, GP Bhattacharjee.
   C version by John Burkardt.
   
   Reference:
   
   KL Majumder, GP Bhattacharjee,
   Algorithm AS 63:
   The incomplete Beta Integral,
   Applied Statistics,
   Volume 22, Number 3, 1973, pages 409-411.
   
   Parameters:
   https://www.jstor.org/stable/2346797?seq=1#page_scan_tab_contents
   Input, double X, the argument, between 0 and 1.
   
   Input, double P, Q, the parameters, which
   must be positive.
   
   Input, double BETA, the logarithm of the complete
   beta function.
   
   Output, int *IFAULT, error flag.
   0, no error.
   nonzero, an error occurred.
   
   Output, double BETAIN, the value of the incomplete
   Beta function ratio.
   */

  const double beta = lgammal(daf)+0.57236494292470009-lgammal(daf+0.5);
  const double acu = 0.1E-14;
  double ai;
  double cx;
  int indx;
  int ns;
  double pp;
  double psq;
  double qq;
  double rx;
  double temp;
  double term;
  double xx;
  
  //  if ault = 0;
  //Check the input arguments.
  if ( (daf<= 0.0)) {// || (0.5 <= 0.0 )){
    //    *ifault = 1;
    //    return pvalue;
  }
  if ( pvalue < 0.0 || 1.0 < pvalue )
  {
    //    *ifault = 2;
    return pvalue;
  }
  /*
   Special cases.
   */
  if ( pvalue == 0.0 || pvalue == 1.0 )   {
    return pvalue;
  }
  psq = daf+ 0.5;
  cx = 1.0 - pvalue;
  
  if ( daf< psq * pvalue )
  {
    xx = cx;
    cx = pvalue;
    pp = 0.5;
    qq = daf;
    indx = 1;
  }
  else
  {
    xx = pvalue;
    pp = daf;
    qq = 0.5;
    indx = 0;
  }
  
  term = 1.0;
  ai = 1.0;
  pvalue = 1.0;
  ns = ( int ) ( qq + cx * psq );
  /*
   Use the Soper reduction formula.
   */
  rx = xx / cx;
  temp = qq - ai;
  if ( ns == 0 ) {
    rx = xx;
  }
  for ( ; ; ) {
    term = term * temp * rx / ( pp + ai );
    pvalue = pvalue + term;;
    temp = fabs ( term );
    if ( temp <= acu && temp <= acu * pvalue ) {
      pvalue = pvalue * exp ( pp * log ( xx )
                           + ( qq - 1.0 ) * log ( cx ) - beta ) / pp;
      if ( indx ) {
        pvalue = 1.0 - pvalue;
      }
      break;
    }
    ai = ai + 1.0;
    ns = ns - 1;
    if ( 0 <= ns ) {
      temp = qq - ai;
      if ( ns == 0 )
      {
        rx = xx;
      }
    } else {
      temp = psq;
      psq = psq + 1.0;
    }
  }
  return pvalue;
}

int paired_t_stat (igraph_vector_t *v1, igraph_vector_t *v2, igraph_real_t *pv, igraph_real_t *ts) {
  igraph_vector_t diff;
  igraph_vector_init(&diff, igraph_vector_size(v2));
  igraph_real_t pvalue;
  
  for (long int i=0; i<igraph_vector_size(v2); i++) {
    VECTOR(diff)[i] = igraph_vector_e(v1,i) - igraph_vector_e(v2,i);
  }
  *pv = t_test_vector(&diff, igraph_vector_size(&diff)-1);
  *ts = t_stat_vector(&diff);
  // calculate t-statistic and p-value;
  igraph_vector_destroy(&diff);
  return 0;
}

  
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
    for (long int j=0; j < source_size; j++) {
      if (igraph_vector_e(source, i) == igraph_vector_e(&source_cpy,j)) {
        //igraph_vector_set(v, i, VECTOR(rank_vals)[j]);
        VECTOR(*v)[i] = VECTOR(rank_vals)[j];
        break;
      }
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
  igraph_vector_t deg, rank;
  igraph_vector_init(&deg, igraph_vcount(graph));
  igraph_vector_init(&rank, igraph_vcount(graph));
  VANV(graph, "Degree", &deg);
  produceRank(&deg, &rank);
  SETVANV(graph, "DegreeRank", &rank);
  igraph_vector_destroy(&deg);
  igraph_vector_destroy(&rank);
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
  igraph_vector_destroy(&rank);
  return 0;
}

int create_graph_csv(char* filepath, int start, int perc) {
  struct stat st = {0};
  if (stat("GRAPH/", &st) == -1) {
    mkdir("GRAPH/", 0700);
  }
  FILE *fs;
  fs = fopen("GRAPH/graph_report.csv", "a");
  fprintf(fs, "| perc       | Authority  | Betweenness | Degree      | Eigenvector | Hub         | Indegree    | OutDegree   | PageRank    |  Random      |\n");
  
  for (int i=start; i<perc; i++) {
    ug_report = false;
    ug_save = false;
    ug_percent = i;
    ug_methods = ALL_METHODS;
    ug_OUTPUT = "GRAPH/";
    load_graph(filepath);
    filter_graph();
    fprintf(fs, "|%-5i|", i);
    while (pv != NULL) {
      fprintf(fs, "%-13f|", pv->val);
      pv = pv->next;
    }
    fprintf(fs, "\n");
    pv = EmptyNode;
  }
  fclose(fs);
  return 0;
}

