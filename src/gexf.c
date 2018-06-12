//
//  gexf.c
//  graphpass
//
//  Created by Ryan Deschamps on 2018-03-15.
//  Copyright © 2018 Ryan Deschamps. All rights reserved.
//

/** @file gexf.c 
 @brief Writes gexf files.
 */

#include <graphpass.h>

extern int errno;

/** Escapes invalid xml character (&, <, >, ") in attributes */
int igraph_i_xml_escape(char* src, char** dest) {
  long int destlen=0;
  char *s, *d;
  for (s=src; *s; s++, destlen++) {
    if (*s == '&') destlen += 4;
    else if (*s == '<') destlen += 3;
    else if (*s == '>') destlen += 3;
    else if (*s == '"') destlen += 5;
    else if (*s == '\'') destlen += 5;
  }
  *dest=igraph_Calloc(destlen+1, char);
  if (!*dest) IGRAPH_ERROR("Not enough memory", IGRAPH_ENOMEM);
  for (s=src, d=*dest; *s; s++, d++) {
    switch (*s) {
      case '&':
        strcpy(d, "&amp;"); d+=4; break;
      case '<':
        strcpy(d, "&lt;"); d+=3; break;
      case '>':
        strcpy(d, "&gt;"); d+=3; break;
      case '"':
        strcpy(d, "&quot;"); d+=5; break;
      case '\'':
        strcpy(d, "&apos;"); d+=5; break;
      default:
        *d = *s;
    }
  }
  *d=0;
  return 0;
}

/** Writes a GEXF file
 
 GEXF provides great support for visualizations and is therefore used by a number
 of light-weight visualization tools like SigmaJS and Gephi.
 
 @param graph - the graph to write to gexf
 @param outstream - a file object
 @param prefixattr - if "true" will add prefixes to the gexf output.
 */
extern int igraph_write_graph_gexf(const igraph_t *graph, FILE *outstream,
                            igraph_bool_t prefixattr) {
  int ret;
  igraph_integer_t l, vc, ec;
  igraph_eit_t it;
  igraph_strvector_t gnames, vnames, enames, labels;
  igraph_vector_t gtypes, vtypes, etypes, size, r, g, b, x, y, weight;
  long int i;
  igraph_vector_t numv;
  igraph_strvector_t strv;
  igraph_vector_bool_t boolv;
  time_t t;
  t = time(NULL);
  const char *gprefix= prefixattr ? "g_" : "";
  const char *vprefix= prefixattr ? "v_" : "";
  const char *eprefix= prefixattr ? "e_" : "";
  
  ret=fprintf(outstream, "<?xml version=\"1.1\" encoding=\"UTF-8\"?>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<gexf xmlns=\"http://www.gexf.net/1.3draft\"\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         xmlns:viz=\"http://www.gexf.net/1.3draft/viz\"\n");
  ret=fprintf(outstream, "         xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         xsi:schemaLocation=\"http://www.gexf.net/1.3draft\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         http://www.gexf.net/1.3draft/gexf.xsd\"\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         version=\"1.3\">\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<!-- Created by igraph -->\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<meta lastmodifieddate=\"%.19s\">\n", ctime(&t));
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<creator>Graphpass filtering using Igraph by Archives Unleashed</creator>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<description> A Filtered Derivative Graph</description>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "</meta>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  /* dump the <key> elements if any */
  
  IGRAPH_VECTOR_INIT_FINALLY(&numv, 1);
  IGRAPH_STRVECTOR_INIT_FINALLY(&strv, 1);
  IGRAPH_VECTOR_BOOL_INIT_FINALLY(&boolv, 1);
  
  IGRAPH_STRVECTOR_INIT_FINALLY(&gnames, 0);
  IGRAPH_STRVECTOR_INIT_FINALLY(&vnames, 0);
  IGRAPH_STRVECTOR_INIT_FINALLY(&enames, 0);
  IGRAPH_VECTOR_INIT_FINALLY(&gtypes, 0);
  IGRAPH_VECTOR_INIT_FINALLY(&vtypes, 0);
  IGRAPH_VECTOR_INIT_FINALLY(&etypes, 0);
  igraph_i_attribute_get_info(graph,
                              &gnames, &gtypes,
                              &vnames, &vtypes,
                              &enames, &etypes);
  
  ret=fprintf(outstream, "  <graph id=\"G\" defaultedgetype=\"%s\">\n", (igraph_is_directed(graph)?"directed":"undirected"));
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  /* graph attributes */
  ret=fprintf(outstream, "  <attributes class=\"graph\">\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  for (i=0; i<igraph_vector_size(&gtypes); i++) {
    char *name, *name_escaped;
    igraph_strvector_get(&gnames, i, &name);
    IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
    if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"string\"/>\n", gprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"double\"/>\n", gprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" attr.title=\"%s\" type=\"boolean\"/>\n", gprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    }
    igraph_Free(name_escaped);
  }
  ret=fprintf(outstream, "  </attributes>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  /* vertex attributes */
  ret=fprintf(outstream, "  <attributes class=\"node\">\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  for (i=0; i<igraph_vector_size(&vtypes); i++) {
    char *name, *name_escaped;
    igraph_strvector_get(&vnames, i, &name);
    IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
    if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"string\"></attribute>\n", vprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"double\"></attribute>\n", vprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"boolean\"></attribute>\n", vprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    }
    igraph_Free(name_escaped);
  }
  ret=fprintf(outstream, "  </attributes>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  /* edge attributes */
  ret=fprintf(outstream, "  <attributes class=\"edge\">\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  for (i=0; i<igraph_vector_size(&etypes); i++) {
    char *name, *name_escaped;
    igraph_strvector_get(&enames, i, &name);
    IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
    if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"string\"/>\n", eprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"double\"/>\n", eprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"boolean\"/>\n", eprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    }
    igraph_Free(name_escaped);
  }
  ret=fprintf(outstream, "  </attributes>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  /* Do not include graph attvalues for now but can add them later */
  /*
   for (i=0; i<igraph_vector_size(&gtypes); i++) {
   char *name, *name_escaped;
   if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
   igraph_strvector_get(&gnames, i, &name);
   IGRAPH_CHECK(igraph_i_attribute_get_numeric_graph_attr(graph, name, &numv));
   if (!isnan(VECTOR(numv)[0])) {
   IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
   ret=fprintf(outstream, "    <data key=\"%s%s\">%g</data>\n",
   gprefix, name_escaped, VECTOR(numv)[0]);
   igraph_Free(name_escaped);
   if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
   }
   } else if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
   char *s, *s_escaped;
   igraph_strvector_get(&gnames, i, &name);
   IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
   ret=fprintf(outstream, "    <data key=\"%s%s\">", gprefix,
   name_escaped);
   igraph_Free(name_escaped);
   IGRAPH_CHECK(igraph_i_attribute_get_string_graph_attr(graph, name, &strv));
   igraph_strvector_get(&strv, 0, &s);
   IGRAPH_CHECK(igraph_i_xml_escape(s, &s_escaped));
   ret=fprintf(outstream, "%s", s_escaped);
   igraph_Free(s_escaped);
   if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
   ret=fprintf(outstream, "</data>\n");
   if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
   } else if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
   igraph_strvector_get(&gnames, i, &name);
   IGRAPH_CHECK(igraph_i_attribute_get_bool_graph_attr(graph, name, &boolv));
   IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
   ret=fprintf(outstream, "    <data key=\"%s%s\">%s</data>\n",
   gprefix, name_escaped, VECTOR(boolv)[0] ? "true" : "false");
   igraph_Free(name_escaped);
   if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
   }
   }*/
  
  /* Let's dump the nodes first */
  ret=fprintf(outstream, "  <nodes>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  vc=igraph_vcount(graph);
  ec=igraph_ecount(graph);
  igraph_strvector_init(&labels, vc);
  igraph_vector_init(&weight, ec);
  igraph_vector_init(&r, vc);
  igraph_vector_init(&g, vc);
  igraph_vector_init(&b, vc);
  igraph_vector_init(&y, vc);
  igraph_vector_init(&x, vc);
  igraph_vector_init(&size, vc);
  
  if (igraph_cattribute_has_attr(graph, IGRAPH_ATTRIBUTE_EDGE, "weight") == true) {
    EANV(graph, "weight", &weight);
  }
  if (igraph_cattribute_has_attr(graph, IGRAPH_ATTRIBUTE_VERTEX, "label") == true) {
    VASV(graph, "label", &labels);
  }
  else if (igraph_cattribute_has_attr(graph, IGRAPH_ATTRIBUTE_VERTEX, "name") == true){
    VASV(graph, "name", &labels);
  } else {
    printf ("No label information available on this graph.");
  }
  
  /*if ( VASV(graph, "label", &label) == 0 ){
   VASV(graph, "label", &label);
   } else if (VASV(graph, "name", &label) == 0) {
   VASV(graph, "name", &label);
   } else {
   printf("label was null trying name");
   }*/
  VANV(graph, "r", &r);
  VANV(graph, "g", &g);
  VANV(graph, "b", &b);
  VANV(graph, "size", &size);
  VANV(graph, "x", &x);
  VANV(graph, "y", &y);
  for (l=0; l<vc; l++) {
    char *name, *name_escaped, *label, *label_escaped;
    igraph_strvector_get(&labels, l, &label);
    IGRAPH_CHECK(igraph_i_xml_escape(label, &label_escaped));
    ret=fprintf(outstream, "    <node id=\"n%ld\" label=\"%s\">\n", (long)l, label_escaped ? label_escaped : "x");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "    <attvalues>\n");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    for (i=0; i<igraph_vector_size(&vtypes); i++) {
      if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
        igraph_strvector_get(&vnames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_numeric_vertex_attr(graph, name,
                                                                igraph_vss_1(l), &numv));
        if (!isnan(VECTOR(numv)[0])) {
          IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
          ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"%g\" />\n",
                      vprefix, name_escaped, VECTOR(numv)[0]);
          igraph_Free(name_escaped);
          if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
        }
      } else if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
        char *s, *s_escaped;
        igraph_strvector_get(&vnames, i, &name);
        IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
        ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"", vprefix,
                    name_escaped);
        igraph_Free(name_escaped);
        IGRAPH_CHECK(igraph_i_attribute_get_string_vertex_attr(graph, name,
                                                               igraph_vss_1(l), &strv));
        igraph_strvector_get(&strv, 0, &s);
        IGRAPH_CHECK(igraph_i_xml_escape(s, &s_escaped));
        ret=fprintf(outstream, "%s\" />\n", s_escaped);
        igraph_Free(s_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      } else if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
        igraph_strvector_get(&vnames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_bool_vertex_attr(graph, name,
                                                             igraph_vss_1(l), &boolv));
        IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
        ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"%s\" />\n",
                    vprefix, name_escaped, VECTOR(boolv)[0] ? "true" : "false");
        igraph_Free(name_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      }
    }
    ret=fprintf(outstream, "    </attvalues>\n");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <viz:color r=\"%i\" g=\"%i\" b=\"%i\"></viz:color>\n", (int)VECTOR(r)[l], (int)VECTOR(g)[l], (int)VECTOR(b)[l]);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <viz:size value=\"%f\"></viz:size>\n", VECTOR(size)[l]);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <viz:position y=\"%f\" x=\"%f\" z=\"0.0\"></viz:position>\n", VECTOR(y)[l], VECTOR(x)[l]);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "    </node>\n");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  }
  ret=fprintf(outstream, "  </nodes>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  /* Now the edges */
  ret=fprintf(outstream, "  <edges>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  IGRAPH_CHECK(igraph_eit_create(graph, igraph_ess_all(0), &it));
  IGRAPH_FINALLY(igraph_eit_destroy, &it);
  for (l=0; l<ec; l++) {
    igraph_integer_t from, to;
    char *name, *name_escaped;
    long int edge=IGRAPH_EIT_GET(it);
    igraph_edge(graph, (igraph_integer_t) edge, &from, &to);
    ret=fprintf(outstream, "    <edge id=\"%ld\" source=\"n%ld\" target=\"n%ld\" weight=\"%f\">\n",
                (long int)l, (long int)from, (long int)to, VECTOR(weight)[l] ? VECTOR(weight)[l] : 0.0);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <attvalues>\n");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    
    for (i=0; i<igraph_vector_size(&etypes); i++) {
      if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
        igraph_strvector_get(&enames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_numeric_edge_attr(graph, name,
                                                              igraph_ess_1((igraph_integer_t) edge), &numv));
        if (!isnan(VECTOR(numv)[0])) {
          IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
          ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"%g\"></attvalue>\n",
                      eprefix, name_escaped, VECTOR(numv)[0]);
          igraph_Free(name_escaped);
          if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
        }
      } else if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
        char *s, *s_escaped;
        igraph_strvector_get(&enames, i, &name);
        IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
        ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"", eprefix,
                    name_escaped);
        igraph_Free(name_escaped);
        IGRAPH_CHECK(igraph_i_attribute_get_string_edge_attr(graph, name,
                                                             igraph_ess_1((igraph_integer_t) edge), &strv));
        igraph_strvector_get(&strv, 0, &s);
        IGRAPH_CHECK(igraph_i_xml_escape(s, &s_escaped));
        ret=fprintf(outstream, "%s\"></attvalue>\n", s_escaped);
        igraph_Free(s_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      } else if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
        igraph_strvector_get(&enames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_bool_edge_attr(graph, name,
                                                           igraph_ess_1((igraph_integer_t) edge), &boolv));
        IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
        ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value\"%s\"></attvalue>\n",
                    eprefix, name_escaped, VECTOR(boolv)[0] ? "true" : "false");
        igraph_Free(name_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      }
    }
    ret=fprintf(outstream, "      </attvalues>\n");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "    </edge>\n");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    IGRAPH_EIT_NEXT(it);
  }
  ret=fprintf(outstream, "  </edges>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  igraph_eit_destroy(&it);
  IGRAPH_FINALLY_CLEAN(1);
  
  ret=fprintf(outstream, "  </graph>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  fprintf(outstream, "</gexf>\n");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  
  igraph_strvector_destroy(&gnames);
  igraph_strvector_destroy(&vnames);
  igraph_strvector_destroy(&enames);
  igraph_vector_destroy(&gtypes);
  igraph_vector_destroy(&vtypes);
  igraph_vector_destroy(&etypes);
  igraph_vector_destroy(&numv);
  igraph_strvector_destroy(&strv);
  igraph_vector_bool_destroy(&boolv);
  IGRAPH_FINALLY_CLEAN(9);
  
  return 0;
}
