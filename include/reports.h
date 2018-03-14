/* -*- Nutil -- Network Graph Utilities mode: C -*-  */
/*
 Copyright <2018> <Ryan Deschamps> <ryan.deschamps@gmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */


struct Node {
  char* abbrev;
  igraph_real_t val;
  struct Node *next;
};

struct RankNode {
  int rankids[20];
  struct RankNode *next;
};

struct Node* asshead = NULL;
struct Node* edges = NULL;
struct Node* density = NULL;
struct Node* betcent = NULL;
struct Node* reciprocity = NULL;
struct Node* degcent = NULL;
struct Node* idegcent = NULL;
struct Node* odegcent = NULL;
struct Node* eigcent = NULL;
struct Node* pagecent = NULL;
struct Node* diameter = NULL;
struct Node* pathlength = NULL;
struct Node* clustering = NULL;
struct RankNode* ranks = NULL;

int push(struct Node** head_ref, igraph_real_t value, char* attr)
{
  struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
  new_node->val  = value;
  new_node->abbrev = attr;
  new_node->next = (*head_ref);
  (*head_ref) = new_node;
  return 0;
}

int pushRank (struct RankNode** head_ref, int rankids[20]) {
  struct RankNode* new_node = (struct RankNode*) malloc(sizeof(struct RankNode));
  for (int i=0; i<20; i++) {
    new_node->rankids[i] = rankids[i];
  }
  new_node->next = (*head_ref);
  (*head_ref) = new_node;
  return 0;
}

int write_report(igraph_t *graph) {
  if (QUICKRUN == true) {
    printf("No reports available for quickrun\n");
    exit(0);
  }
  printf("Write report ... \n");
  char dir[150];
  struct stat st = {0};
  strncpy(dir, OUTPUT, sizeof(OUTPUT)/sizeof(OUTPUT[0]));
  strncat(dir, "REPORT/", 7);
  char filepath[150];
  strncpy(filepath, dir, sizeof(dir)/sizeof(dir[0]));
  strncat(filepath, "report.txt", 10);
  if (stat(dir, &st) == -1) {
    mkdir(dir, 0700);
  }
  FILE *fs;
  time_t t;
  igraph_vector_t gtypes, vtypes, etypes;
  igraph_strvector_t gnames, vnames, enames;
  igraph_vector_init(&gtypes, 0);
  igraph_vector_init(&vtypes, 0);
  igraph_vector_init(&etypes, 0);
  igraph_strvector_init(&gnames, 0);
  igraph_strvector_init(&vnames, 0);
  igraph_strvector_init(&enames, 0);
  igraph_cattribute_list(&g, &gnames, &gtypes, &vnames, &vtypes,
                         &enames, &etypes);
  fs = fopen(filepath, "a");
  t = time(NULL);
  fprintf( fs, "REPORT: %s ", ctime(&t));
  fprintf( fs, "-------------------- \n\n");
  fprintf( fs, "ORIGINAL GRAPH: *%s.gexf*\n\n", FILENAME);
  for (int i=0; i<igraph_strvector_size(&gnames); i++) {
    fprintf(fs, "%s : %f \n", STR(gnames, i), GAN(&g, STR(gnames, i)));
  }
  /* print names (use asshead) */
  fprintf(fs, "TRAIT COMPARISON BY FILTERING METHOD \n");
  fprintf(fs, "------------------------------------ \n");
  fprintf(fs, "Percent Filtered: %-2f\n", PERCENT);
  fprintf(fs, "\n| Method          | Δ Edges   | Δ Assort | Δ Dens.  | Δ Recipr | Δ C(Deg.)|\n");
  fprintf(fs, "|-----------------|-----------|----------|----------|----------|----------|\n");
  while (asshead != NULL) {
    fprintf(fs,
            "| %-16s| %-10f|%-10f|%-10f|%-10f|%-10f|\n",
            asshead->abbrev, (GAN(&g, "EDGES") - edges->val), (GAN(&g, "ASSORTATIVITY") - asshead->val),
            (GAN(&g, "DENSITY") - density->val) , (GAN(&g, "RECIPROCITY") - reciprocity->val),
            (GAN(&g, "centralizationDegree") - degcent->val)
            );
    asshead = asshead->next;
    density = density->next;
    edges = edges->next;
    reciprocity = reciprocity->next;
    degcent = degcent->next;
  }
  fprintf (fs, "|                 |           |          |          |          |          |\n\n");
  
  fprintf(fs, "\n| Method          | Δ Diameter| Δ Pathlen| Δ Cluster| Δ C(Betw)| Δ C(Page)|\n");
  fprintf(fs, "|-----------------|-----------|----------|----------|----------|----------|\n");
  while (clustering != NULL) {
    fprintf(fs,
            "| %-16s| %-10f|%-10f|%-10f|%-10f|%-10f|\n",
            clustering->abbrev, (GAN(&g, "DIAMETER") - diameter->val), (GAN(&g, "AVG_PATH_LENGTH") - pathlength->val),
            (GAN(&g, "OVERALL_CLUSTERING") - clustering->val) , (GAN(&g, "centralizationBetweenness") - betcent->val),
            (GAN(&g, "centralizationPageRank") - pagecent->val)
            );
    clustering = clustering->next;
    diameter = diameter->next;
    pathlength = pathlength->next;
    betcent = betcent->next;
    pagecent = pagecent->next;
  }
  fprintf (fs, "|                 |           |          |          |          |          |\n\n");
  fclose(fs);
  igraph_vector_destroy(&gtypes);
  igraph_vector_destroy(&vtypes);
  igraph_vector_destroy(&etypes);
  igraph_strvector_destroy(&gnames);
  igraph_strvector_destroy(&vnames);
  igraph_strvector_destroy(&enames);
  return 0;
}
