/* -*- Graphpass -- Network Graph Utilities mode: C -*-  */
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


/** @file quickrun.c
    @brief produces a lightweight vizualization for a graph.

     The quickrun utilty is intended for producing generic vizualisations for
     network graphs, relatively quickly using basic algorithms.
     Node sizes are produced using basic degree.
     Node (rgb) colours are delineated using walktrap modularity.
     Node positions are 2d and set using the Fruchterman-Rheingold algorithm,
     which emphasizes the spacing between nodes.
 */

#include <graphpass.h>

extern int quickrunGraph() {
  igraph_vector_t size;
  igraph_vector_init(&size, igraph_vcount(&g));
  if (ug_verbose == true)
    printf ("Calculating Degree... \n");
  calc_degree(&g, 'd');
  if (ug_verbose == true)
    printf ("Calculating Modularity... \n");
  calc_modularity(&g);
  if (ug_verbose == true)
    printf ("Assigning Colors... \n");
  colors(&g);
  if (ug_verbose == true)
    printf("Assigning Degree values... \n");
  VANV(&g, "Degree", &size);
  if (ug_verbose == true)
    printf("Scaling graph with degree values... \n");
  set_size(&g, &size, 100);
  if (ug_verbose == true)
    printf("Producing layout details... \n");
  layout_graph(&g, 'f');
  write_graph(&g, "-");
  igraph_vector_destroy(&size);
  return 0;
}
