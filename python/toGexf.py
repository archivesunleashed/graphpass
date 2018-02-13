import networkx as nx
import os #file operations
import sys

print(len(sys.argv))

if (len(sys.argv) > 1):
    path = sys.argv[1]
else:
    path = "../OUT/"

if (len(sys.argv) > 2):
    directory = sys.argv[2]
else:
    directory = "../GEXF"

try:
  for filename in os.listdir(path):
      if (filename.endswith(".graphml")):
          print ("Processing file: " + filename)
        #see the filenames available.
          graph = nx.read_graphml(path+filename)
          filename = filename[:-8] + ".gexf"
          if not os.path.exists(directory):
            os.makedirs(directory)
          nx.write_gexf(graph, directory+"/"+filename)
except Exception as e:
   print(e)
