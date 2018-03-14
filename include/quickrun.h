/* -*- Nutil -- Network Graph Utilities mode: C -*-  */
/*
 Copyright <2018> <Ryan Deschamps> <ryan.deschamps@gmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */


/** \file quickrun.h 
    \brief produces a lightweight vizualization for a graph.

     The quickrun utilty is intended for producing generic vizualisations for 
     network graphs, relatively quickly using basic algorithms.
     Node sizes are produced using basic degree.
     Node (rgb) colours are delineated using walktrap modularity.
     Node positions are 2d and set using the Fruchterman-Rheingold algorithm, 
     which emphasizes the spacing between nodes.
 */



int quickrunGraph() {
  igraph_t g2;
  igraph_vector_t size;
  igraph_vector_init(&size, NODESIZE);
  igraph_copy(&g2, &g);
  calc_degree(&g2, SIZE_DEFAULT_CHAR);
  calc_modularity(&g2);
  colors(&g2);
  VANV(&g2, SIZE_DEFAULT, &size);
  set_size(&g2, &size, 100);
  layout_graph(&g2, 'f');
  write_graph(&g2, "__");
  igraph_vector_destroy(&size);
  igraph_destroy(&g2);
  return 0;
}
