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

/** @file gexf.c
 @brief Writes gexf files.
 */

#include <graphpass.h>

extern int errno;

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

  ret=fprintf(outstream, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<gexf xmlns=\"http://www.gexf.net/1.2draft\"\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         xmlns:viz=\"http://www.gexf.net/1.2draft/viz\"\x0A");
  ret=fprintf(outstream, "         xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         xsi:schemaLocation=\"http://www.gexf.net/1.2draft\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         http://www.gexf.net/1.2draft/gexf.xsd\"\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "         version=\"1.2\">\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<!-- Created by igraph -->\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<meta lastmodifieddate=\"%.19s\">\x0A", ctime(&t));
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<creator>Graphpass filtering using Igraph by Archives Unleashed</creator>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "<description> A Filtered Derivative Graph</description>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  ret=fprintf(outstream, "</meta>\x0A");
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

  ret=fprintf(outstream, "  <graph id=\"G\" defaultedgetype=\"%s\">\x0A", (igraph_is_directed(graph)?"directed":"undirected"));
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);

  /* vertex attributes */
  ret=fprintf(outstream, "  <attributes class=\"node\">\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);

  for (i=0; i<igraph_vector_size(&vtypes); i++) {
    char *name, *name_escaped;
    igraph_strvector_get(&vnames, i, &name);
    IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
    if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"string\"></attribute>\x0A", vprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"double\"></attribute>\x0A", vprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"boolean\"></attribute>\x0A", vprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    }
    igraph_Free(name_escaped);
  }
  ret=fprintf(outstream, "  </attributes>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);

  /* edge attributes */
  ret=fprintf(outstream, "  <attributes class=\"edge\">\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  for (i=0; i<igraph_vector_size(&etypes); i++) {
    char *name, *name_escaped;
    igraph_strvector_get(&enames, i, &name);
    IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
    if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_STRING) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"string\"/>\x0A", eprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"double\"/>\x0A", eprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    } else if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
      ret=fprintf(outstream, "  <attribute id=\"%s%s\" title=\"%s\" type=\"boolean\"/>\x0A", eprefix, name_escaped, name_escaped);
      if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    }
    igraph_Free(name_escaped);
  }
  ret=fprintf(outstream, "  </attributes>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);

  /* Let's dump the nodes first */
  ret=fprintf(outstream, "  <nodes>\x0A");
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
    ret=fprintf(outstream, "    <node id=\"n%ld\" label=\"%s\">\x0A", (long)l, label_escaped ? label_escaped : "x");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "    <attvalues>\x0A");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    for (i=0; i<igraph_vector_size(&vtypes); i++) {
      if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
        igraph_strvector_get(&vnames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_numeric_vertex_attr(graph, name,
                                                                igraph_vss_1(l), &numv));
        if (!isnan(VECTOR(numv)[0])) {
          IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
          ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"%g\" />\x0A",
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
        ret=fprintf(outstream, "%s\" />\x0A", s_escaped);
        igraph_Free(s_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      } else if (VECTOR(vtypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
        igraph_strvector_get(&vnames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_bool_vertex_attr(graph, name,
                                                             igraph_vss_1(l), &boolv));
        IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
        ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"%s\" />\x0A",
                    vprefix, name_escaped, VECTOR(boolv)[0] ? "true" : "false");
        igraph_Free(name_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      }
    }
    ret=fprintf(outstream, "    </attvalues>\x0A");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <viz:color r=\"%i\" g=\"%i\" b=\"%i\"></viz:color>\x0A", (int)VECTOR(r)[l], (int)VECTOR(g)[l], (int)VECTOR(b)[l]);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <viz:size value=\"%f\"></viz:size>\x0A", VECTOR(size)[l]);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <viz:position y=\"%f\" x=\"%f\" z=\"0.0\"></viz:position>\x0A", VECTOR(y)[l], VECTOR(x)[l]);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "    </node>\x0A");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  }
  ret=fprintf(outstream, "  </nodes>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);

  /* Now the edges */
  ret=fprintf(outstream, "  <edges>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  IGRAPH_CHECK(igraph_eit_create(graph, igraph_ess_all(0), &it));
  IGRAPH_FINALLY(igraph_eit_destroy, &it);
  for (l=0; l<ec; l++) {
    igraph_integer_t from, to;
    char *name, *name_escaped;
    long int edge=IGRAPH_EIT_GET(it);
    igraph_edge(graph, (igraph_integer_t) edge, &from, &to);
    ret=fprintf(outstream, "    <edge id=\"%ld\" source=\"n%ld\" target=\"n%ld\" weight=\"%f\">\x0A",
                (long int)l, (long int)from, (long int)to, VECTOR(weight)[l] ? VECTOR(weight)[l] : 0.0);
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "      <attvalues>\x0A");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);

    for (i=0; i<igraph_vector_size(&etypes); i++) {
      if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
        igraph_strvector_get(&enames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_numeric_edge_attr(graph, name,
                                                              igraph_ess_1((igraph_integer_t) edge), &numv));
        if (!isnan(VECTOR(numv)[0])) {
          IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
          ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value=\"%g\"></attvalue>\x0A",
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
        ret=fprintf(outstream, "%s\"></attvalue>\x0A", s_escaped);
        igraph_Free(s_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      } else if (VECTOR(etypes)[i] == IGRAPH_ATTRIBUTE_BOOLEAN) {
        igraph_strvector_get(&enames, i, &name);
        IGRAPH_CHECK(igraph_i_attribute_get_bool_edge_attr(graph, name,
                                                           igraph_ess_1((igraph_integer_t) edge), &boolv));
        IGRAPH_CHECK(igraph_i_xml_escape(name, &name_escaped));
        ret=fprintf(outstream, "      <attvalue for=\"%s%s\" value\"%s\"></attvalue>\x0A",
                    eprefix, name_escaped, VECTOR(boolv)[0] ? "true" : "false");
        igraph_Free(name_escaped);
        if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
      }
    }
    ret=fprintf(outstream, "      </attvalues>\x0A");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    ret=fprintf(outstream, "    </edge>\x0A");
    if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
    IGRAPH_EIT_NEXT(it);
  }
  ret=fprintf(outstream, "  </edges>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  igraph_eit_destroy(&it);
  IGRAPH_FINALLY_CLEAN(1);

  ret=fprintf(outstream, "  </graph>\x0A");
  if (ret<0) IGRAPH_ERROR("Write failed", IGRAPH_EFILE);
  fprintf(outstream, "</gexf>\x0A");
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
