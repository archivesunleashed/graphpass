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

/** @file io.c
    @brief Basic input-output utilities (loading and writing graphs)
 **/

#include <graphpass.h>

/** \fn get_directory
    \brief gets the directory from a path (not using libgen)
    @param path - path to extract the directory
    @param result - empty string pointer to put the result.
 */
int get_directory (char *path, char **result) {
  if (strlen(path) > 499){
    if (!ug_TEST) {
      fprintf(stderr, "ERROR: Maximum characters in an outpath is 500.\n");
    }
    return(1);
  }
   static char temppath[500];
   char *temp;
   if (path[strlen(path)-1] ==  '/') {
     *result = path;
   } else {
     strncpy(temppath, path, strlen(path)+1);
     temp = strpbrk(temppath, "/") ? strrchr(temppath, '/') + 1 : NULL;
     if (temp) {
       *temp = '\0';
     }
     *result = temp ? temppath : "./";
   }
   return 0;
 }
 /** \fn get_filename
     \brief gets the filename from a path (not using libgen)
     @param path - path to extract the directory
     @param result - empty string pointer to place the result.
  */
int get_filename (char *path, char **result) {
  char *temp;
  if (path[strlen(path)-1] != '/') {
    temp = strpbrk(path, "/") ? strrchr(path, '/') + 1 : path;
    *result = temp;
  }
  return 0;
}

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
    if (!ug_TEST) {
      fprintf(stderr, ">>> FAILURE - Could not find graphML file at filepath location.\n");
    }
    return (-1);
  }
  igraph_read_graph_graphml(&g, fp, 0);
  NODESIZE = igraph_vcount(&g);
  EDGESIZE = igraph_ecount(&g);
  if (ug_verbose) {
    printf("Successfully ingested graph with %li nodes and %li edges.\n"
    , (long int)NODESIZE, (long int)EDGESIZE);
  }
  fclose(fp);
  return (0);
}


/** \fn int write_graph (igraph_t *graph)
    \brief Writes a graph file.

     Based on the ug_OUTPUT, FILENAME and methods
     writes a network graph to the appropriate location.
     If GFORMAT is set to "true" the file will output to GEXF,
     otherwise a graphml file will be produced.
     @param graph - the graph to write to the file.
 **/

extern int write_graph(igraph_t *graph, char *attr) {
  FILE *fp;
  char fn[strlen(ug_OUTFILE)+1];
  struct stat st = {0};
  if (stat(ug_OUTPATH, &st) == -1) {
    if (!ug_TEST) {
      fprintf(stderr, ">>> FAILURE - Could not create file at selected output location.\n");
      fprintf(stderr, ">>>         - Ensure that your assigned output folder exists.\n");
    }
    return (-1);
  }
  char path[250];
  char perc_as_string[3];
  int perc = (int)ug_percent;
  strncpy(fn, ug_OUTFILE, strlen(ug_OUTFILE)+1);
  if(strstr(fn, ".") != NULL) {
    strip_ext(fn);
  }
  snprintf(perc_as_string, 3, "%d", perc);
  strncpy(path, ug_OUTPATH, strlen(ug_OUTPATH)+1);
  strncat(path, fn, (strlen(fn)+1));
  if (ug_quickrun == false) {
    strncat(path, perc_as_string, 3);
    strncat(path, attr, strlen(attr));
  }
  if (ug_gformat){
    strncat(path, ".gexf", 5);
  } else {
    strncat(path, ".graphml", 8);
  }
  if (ug_save == true) {
    if (ug_verbose == true) {
      printf("Writing output to: %s\n", path);
    }
    fp = fopen(path, "w");
    if (fp) {
      if (ug_gformat) {
        igraph_write_graph_gexf(graph, fp, 1);
      } else {
        igraph_write_graph_graphml(graph, fp, 1);
      }
    } else {
      if (!ug_TEST) {
        fprintf (stderr, "\n ERROR: Output path %s could not be accessed. Graphpass", ug_OUTPATH);
        fprintf (stderr, "\n        cannot create more than one directory in your outpath.");
        fprintf (stderr, "\n        If you require additional directories, please create them");
        fprintf (stderr, "\n        before running graphpass.\n\n");
      }
      return(-1);
    }
    fclose(fp);
  }
  return 0;
}
