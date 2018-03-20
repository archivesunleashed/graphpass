/* -*- Nutil -- Network Graph Utilities mode: C -*-  */
/*
 Copyright <2018> <Ryan Deschamps> <ryan.deschamps@gmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

/** @file io.c
    @brief Basic input-output utilities (loading and writing graphs)
 **/

#include <graphpass.h>


/** \fn strip_ext
    \brief strips the file extension from a filename
    @param fname - filename to strip the extension from.
 */
int strip_ext(char *fname) {
  char *end = fname + strlen(fname);
  while (end > fname && *end != '.' && *end != '\\' && *end != '/') {
    --end;
  }
  if (end > fname && *end == '.') {
    *end = '\0';
  }
  return 0;
}

/** \fn int load_graph
 Loads a graphml file.
 @param filename - name of the file to load.
 */
extern int load_graph (char* filename) {
  igraph_i_set_attribute_table(&igraph_cattribute_table);
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == 0) {
    return (1);
  }
  printf("Reading graphml file.\n");
  igraph_read_graph_graphml(&g, fp, 0);
  NODESIZE = igraph_vcount(&g);
  printf("Successfully ingested graph with %li nodes.\n", (long int)NODESIZE);
  fclose(fp);
  return (0);
}


/** \fn int write_graph (igraph_t *graph)
    \brief Writes a graph file.
 
     Based on the OUTPUT, FILENAME and METHODS constants
     writes a network graph to the appropriate location.
     If GFORMAT is set to "true" the file will output to GEXF, 
     otherwise a graphml file will be produced.
     @param graph - the graph to write to the file.
 **/

extern int write_graph(igraph_t *graph, char *attr) {
  FILE *fp;
  char fn[strlen(FILENAME)+1];
  struct stat st = {0};
  if (stat(OUTPUT, &st) == -1) {
    mkdir(OUTPUT, 0700);
  }
  char path[150];
  char perc_as_string[3];
  int perc = (int)PERCENT;
  strncpy(fn, FILENAME, strlen(FILENAME));
  strip_ext(fn);
  snprintf(perc_as_string, 3, "%d", perc);
  strncpy(path, OUTPUT, strlen(OUTPUT)+1);
  strncat(path, fn, (strlen(fn)+1));
  if (QUICKRUN == false) {
    strncat(path, perc_as_string, 3);
    strncat(path, attr, strlen(attr));
  }
  if (GFORMAT){
    strncat(path, ".gexf", 5);
  } else {
    strncat(path, ".graphml", 8);
  }
  if (SAVE == true) {
    printf("Writing output to: %s\n", path);
    fp = fopen(path, "w");
    if (GFORMAT) {
      igraph_write_graph_gexf(graph, fp, 1);
    } else {
      igraph_write_graph_graphml(graph, fp, 1);
    }
    fclose(fp);
  }
  
  return 0;
}

