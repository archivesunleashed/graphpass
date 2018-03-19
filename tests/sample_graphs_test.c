#include <igraph.h>


int createUndirectedGraph (igraph_t *star_graph) {
  igraph_vector_t star_graph_vector;
  
  igraph_vector_init(&star_graph_vector, 18);
  printf ("initialized starting\n");
  VECTOR(star_graph_vector)[0]=0; VECTOR(star_graph_vector)[1]=8;
  VECTOR(star_graph_vector)[2]=0; VECTOR(star_graph_vector)[3]=6;
  VECTOR(star_graph_vector)[4]=0; VECTOR(star_graph_vector)[5]=7;
  VECTOR(star_graph_vector)[6]=0; VECTOR(star_graph_vector)[7]=5;
  VECTOR(star_graph_vector)[8]=1; VECTOR(star_graph_vector)[9]=0;
  VECTOR(star_graph_vector)[10]=1; VECTOR(star_graph_vector)[11]=2;
  VECTOR(star_graph_vector)[12]=2; VECTOR(star_graph_vector)[13]=3;
  VECTOR(star_graph_vector)[14]=3; VECTOR(star_graph_vector)[15]=4;
  VECTOR(star_graph_vector)[16]=4; VECTOR(star_graph_vector)[17]=2;
  printf("VECTORS created\n");
  igraph_create(star_graph, &star_graph_vector, 9, 0);
  igraph_vector_destroy(&star_graph_vector);
  return 0;
}

int createDirectedGraph(igraph_t *star_digraph) {
  printf ("initializing vector\n");
  igraph_small(star_digraph, 0, IGRAPH_DIRECTED, 0, 10, 1, 0, 2, 11, 3, 0, 4, 12,
               5, 0, 6, 0, 7, 1, 8, 1, 9, 3, 10, 1, 11, 2, 12, 2, 13, 4, 14, 4,
               15, 2, 16, 2, 17, 5, 18, 5, 19, 2, 20, 5, 21, 4, 22, 3, 23, 6, 24, 3,
               25, 7, 26, 3, 27, 8, -1);
  printf("Graph created with %i nodes\n", igraph_vcount(star_digraph));
  return 0;
}
