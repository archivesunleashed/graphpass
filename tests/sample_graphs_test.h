igraph_t star_graph; // Graph contains a star, a triad and a bridge node.
igraph_vector_t star_graph_vector;

igraph_t star_digraph; // Graph contains a star, a triad and a bridge node.
igraph_vector_t star_digraph_vector;

/* Create Undirected graph */
igraph_vector_init(&star_graph_vector, 18);
VECTOR(star_graph_vector)[0]=0; VECTOR(star_graph_vector)[1]=8;
VECTOR(star_graph_vector)[2]=0; VECTOR(star_graph_vector)[3]=6;
VECTOR(star_graph_vector)[4]=0; VECTOR(star_graph_vector)[5]=7;
VECTOR(star_graph_vector)[6]=0; VECTOR(star_graph_vector)[7]=5;
VECTOR(star_graph_vector)[8]=1; VECTOR(star_graph_vector)[9]=0;
VECTOR(star_graph_vector)[10]=1; VECTOR(star_graph_vector)[11]=2;
VECTOR(star_graph_vector)[12]=2; VECTOR(star_graph_vector)[13]=3;
VECTOR(star_graph_vector)[14]=3; VECTOR(star_graph_vector)[15]=4;
VECTOR(star_graph_vector)[16]=4; VECTOR(star_graph_vector)[17]=2;
igraph_create(&star_graph, &star_graph_vector, 0, 0);

igraph_vector_init(&star_digraph_vector, 28);
VECTOR(star_digraph_vector)[0]=10; VECTOR(star_digraph_vector)[1]=0;
VECTOR(star_digraph_vector)[2]=11; VECTOR(star_digraph_vector)[3]=0;
VECTOR(star_digraph_vector)[4]=12; VECTOR(star_digraph_vector)[5]=0;
VECTOR(star_digraph_vector)[6]=0; VECTOR(star_digraph_vector)[7]=1;
VECTOR(star_digraph_vector)[8]=1; VECTOR(star_digraph_vector)[9]=3;
VECTOR(star_digraph_vector)[10]=1; VECTOR(star_digraph_vector)[11]=2;
VECTOR(star_digraph_vector)[12]=2; VECTOR(star_digraph_vector)[13]=4;
VECTOR(star_digraph_vector)[14]=4; VECTOR(star_digraph_vector)[15]=2;
VECTOR(star_digraph_vector)[16]=2; VECTOR(star_digraph_vector)[17]=5;
VECTOR(star_digraph_vector)[18]=5; VECTOR(star_digraph_vector)[19]=2;
VECTOR(star_digraph_vector)[20]=5; VECTOR(star_digraph_vector)[21]=4;
VECTOR(star_digraph_vector)[22]=3; VECTOR(star_digraph_vector)[23]=6;
VECTOR(star_digraph_vector)[24]=3; VECTOR(star_digraph_vector)[25]=7;
VECTOR(star_digraph_vector)[26]=3; VECTOR(star_digraph_vector)[27]=8;

igraph_create(&star_digraph, &star_digraph_vector, 0, 1);
