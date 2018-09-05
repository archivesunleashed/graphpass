/*
 * GraphPass:
 * A utility to filter networks and provide a default visualization output
 * for Gephi or SigmaJS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../headers/graphpass.h"

/** @file viz.c
 @brief Tools for presetting visualization elements: color, position & size.
 */


/** creates default colors for a graph based on walktrap modularity

  Colors sets the rgb values based on a preset color scheme.

  @param - the graph for which to provide colors.
 */
extern int colors (igraph_t *graph) {
  /* Has modularity been set? */
  char* attr = "WalkTrapModularity";
  igraph_vector_t r, g, b;
  int gsize = (long int)igraph_vcount(graph);
  igraph_vector_init(&r, gsize);
  igraph_vector_init(&g, gsize);
  igraph_vector_init(&b, gsize);
  /* Set RGB values based on WalkTrapModularity membership */
  for (long int i=0; i<gsize; i++) {
    switch ((int)VAN(graph, attr, i)) {
      case 0 :
        VECTOR(r)[i] = 35;
        VECTOR(g)[i] = 217;
        VECTOR(b)[i] = 211;
        break;
      case 1 :
        VECTOR(r)[i] = 217;
        VECTOR(g)[i] = 80;
        VECTOR(b)[i] = 35;
        break;
      case 2 :
        VECTOR(r)[i] = 35;
        VECTOR(g)[i] = 217;
        VECTOR(b)[i] = 211;
        break;
      case 3 :
        VECTOR(r)[i] = 123;
        VECTOR(g)[i] = 35;
        VECTOR(b)[i] = 217;
        break;
      case 4 :
        VECTOR(r)[i] = 219;
        VECTOR(g)[i] = 59;
        VECTOR(b)[i] = 147;
        break;
      case 5 :
        VECTOR(r)[i] = 59;
        VECTOR(g)[i] = 217;
        VECTOR(b)[i] = 209;
        break;
      case 6 :
        VECTOR(r)[i] = 67;
        VECTOR(g)[i] = 217;
        VECTOR(b)[i] = 59;
        break;
      case 7 :
        VECTOR(r)[i] = 217;
        VECTOR(g)[i] = 120;
        VECTOR(b)[i] = 189;
        break;
      case 8 :
        VECTOR(r)[i] = 120;
        VECTOR(g)[i] = 219;
        VECTOR(b)[i] = 180;
        break;
      case 9 :
        VECTOR(r)[i] = 120;
        VECTOR(g)[i] = 189;
        VECTOR(b)[i] = 219;
        break;
      case 10 :
        VECTOR(r)[i] = 217;
        VECTOR(g)[i] = 150;
        VECTOR(b)[i] = 120;
        break;
        /* Can add more here if desired, but assume that the first 10 collected
         membership groups will be the biggest */
      default:
        VECTOR(r)[i] = 217;
        VECTOR(g)[i] = 219;
        VECTOR(b)[i] = 219;
    }
  }
  SETVANV(graph, "r", &r);
  SETVANV(graph, "g", &g);
  SETVANV(graph, "b", &b);
  igraph_vector_destroy(&r);
  igraph_vector_destroy(&g);
  igraph_vector_destroy(&b);
  return 0;
}

/** Lays out a graph using a standard algorithm

 @param graph - the graph to layout
 @param layout - a char for the layout 'k' for kamada_kawai,
   'f' for fruchterman rheingold or lgl if char is not recognized.
 **/
extern int layout_graph(igraph_t *graph, char layout) {
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
      1000, gsize/0.5, 10, 0.98,gsize, 0,&min,&max,&min, &max);
      break;
    case 'f' : igraph_layout_fruchterman_reingold(graph, &matrix,
      500, gsize, 10 * gsize^2, 1.5,10 * gsize^3,0,NULL, &min, &max, &min, &max);
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

/** sets the size of nodes based on vector scores provide by v

 @param graph - graph to set size values
 @param v - vector used to set size values, an attribute for degree, for instance
 @param max - the maximum desired size setting.

 **/
extern int set_size(igraph_t *graph, igraph_vector_t *v, int max) {
  long int gsize = (long int)igraph_vcount(graph);
  igraph_vector_t v2;
  igraph_vector_t logOf;
  igraph_vector_init(&logOf, gsize);
  double scale;
  igraph_vector_copy(&v2, v);
  for (long int i=0; i<gsize; i++){
    double val = log(VECTOR(v2)[i] + 1);
    double minimum = log(igraph_vector_min(&v2) + 1);
    double maximum = log(igraph_vector_max(&v2) + 1);
    VECTOR(logOf)[i] = maximum == minimum ? (VECTOR(v2)[i] + 1) : (max * (val - minimum) / (maximum - minimum));
  }
  SETVANV(graph, "size", &logOf);
  igraph_vector_destroy(&logOf);
  igraph_vector_destroy(&v2);
  return 0;
}
