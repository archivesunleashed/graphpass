# Borg Reducer

![borg reducer in action](https://user-images.githubusercontent.com/3834704/35356808-87152cf0-011f-11e8-9264-9c411ca16b3e.png)

Borg Reducer is a helper library to filter networks and provide a default
visualization output for [Gephi](https://gephi.org/). It prevents the infamous "borg cube" result
when entering large files into Gephi, allowing you to work with ready-made
network layouts.

## Installation

### Dependencies

This library requires the [C Igraph Library](http://igraph.org/c/) and
a C compiler, such as [gcc](https://gcc.gnu.org/).

For MacOS:

Using [brew](https://brew.sh/), the following commands will install dependencies:

```
brew install gcc
brew install igraph
```

### Building

Clone the repository with:

```
git clone https://github.com/archivesunleashed/borg-reducer
```

Type

```
brew info igraph
```

and verify that the path displayed there matches the default IGRAPH_PATH value provided in the Makefile. By default this is `/usr/local/Cellar/igraph/0.7.1_6/`.

Then

```
cd borg-reducer
make
```

## Usage

Once compiled use the following command:

```
./borgreducer --file {FILENAME} --percent {PERCENTAGE TO FILTER} --method {METHODS (see below)} --output {OUTPUT DIRECTORY}`
```

You will pass options using the `--method` flag. The options can be seen below:

* `a` : authority
* `b` : betweenness
* `c` : clustering
* `d` : simple degree
* `e` : eigenvector
* `h` : hub
* `i` : in-degree
* `o` : out-degree
* `p` : pagerank
* `r` : random
* `w` : weighted degree

For example:

```
./borgreducer --percent 10 --methods "b" --file links-for-gephi.graphml
```

Will filter the graph down by 10% and lay the network out using the betweenness function. It will find `links-for-gephi.graphml` file in `/assets` and output a new one to `/OUT` (titled `links-for-gephi.graphml10Betweenness.graphml`).

# Optional arguments

* `-r` : create an output report showing the impact of filtering on graph features.
* `-g` : output as a .gexf (for SigmaJS) instead of .graphml.

# GexF converter

Included in the package is a Graphml to Gexf converter (to support sigma.js outputs for instance).  

Use:

`python python/toGexf.py {path to graphmls} {path for Gexfs}`

The code will convert all the graphmls in the folder to gexf.

# License

Licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).

# Acknowledgments

This work is primarily supported by the [Andrew W. Mellon Foundation](https://uwaterloo.ca/arts/news/multidisciplinary-project-will-help-historians-unlock). Additional funding for the Toolkit has come from the U.S. National Science Foundation, Columbia University Library's Mellon-funded Web Archiving Incentive Award, the Natural Sciences and Engineering Research Council of Canada, the Social Sciences and Humanities Research Council of Canada, and the Ontario Ministry of Research and Innovation's Early Researcher Award program. Any opinions, findings, and conclusions or recommendations expressed are those of the researchers and do not necessarily reflect the views of the sponsors.
