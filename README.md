# GraphPass
[![codecov](https://codecov.io/gh/archivesunleashed/graphpass/branch/main/graph/badge.svg)](https://codecov.io/gh/archivesunleashed/graphpass)
[![Contribution Guidelines](http://img.shields.io/badge/CONTRIBUTING-Guidelines-blue.svg)](./CONTRIBUTING.md)
[![LICENSE](https://img.shields.io/badge/license-Apache-blue.svg?style=flat-square)](./LICENSE)

![GraphPass in action](https://user-images.githubusercontent.com/3834704/35356808-87152cf0-011f-11e8-9264-9c411ca16b3e.png)

GraphPass is a utility to filter networks and provide a default
visualization output for [Gephi](https://gephi.org/) or [SigmaJS](https://sigmaja.org). It prevents the infamous "borg cube" result when entering large files into Gephi, allowing you to work with ready-made network layouts.

## Installation

### Dependencies

This utility requires the [C Igraph Library](http://igraph.org/c/) and
a C compiler, such as [gcc](https://gcc.gnu.org/).

#### For Linux (Ubuntu):

Install the igraph dependencies:

```
sudo apt-get install git gcc libxml2-dev build-essential
wget http://igraph.org/nightly/get/c/igraph-0.7.1.tar.gz
tar -xvzf igraph-0.7.1.tar.gz
cd igraph-0.7.1
./configure
make
sudo make install
sudo ldconfig
```

You can test that gcc and igraph have been installed properly by using:

```
gcc -ligraph
```

If you get `undefined reference to 'main'` that means Linux is looking for
graphpass and you are ready to go.

If you get `cannot find -ligraph` then something went wrong with the install.

Check through the logs to see what failed to install.

If desired remove the igraph directory:

```
cd ..
rm -rf igraph-0.7.1
```

#### For MacOS:

Using [brew](https://brew.sh/), the following commands will install dependencies:

```
brew install gcc
brew install igraph
```

Type

```
brew info igraph
```

and verify that the path displayed there matches the default `IGRAPH_PATH` value provided in the Makefile. By default this is `/usr/local/Cellar/igraph/0.7.1_6/` for MacOS.

### Building

Go to your preferred install directory, for example:

```
cd /Users/{USERNAME}/
```

Clone the repository with:

```
git clone https://github.com/archivesunleashed/graphpass
```

Then

```
cd graphpass
make
```

## Usage

Once compiled use the following command:

```
./graphpass {INPUT PATH} {OUTPUT PATH} {FLAGS}
```

The following flags are available:

* `--input {FILEPATH} or -i` - The filepath of the file to run GraphPass on. If not set, GraphPass will use
a default network in `src/resources`. This will override the value in `{INPUT PATH}`.
* `--output {FILEPATH} or -o` - The filepath for outputs, overriding `{OUTPUT PATH}`. If the output path contains a filename, GraphPass will use that, otherwise it will default to the filename provided in `{INPUT PATH}`. Unless the quickpass (`-q`) is selected, the filename will also be altered to show the percentage filtered from the graph and the method used.
* `--percent {PERCENT} or -p` - a percentage to remove from the file. By default this is 0.0.
* `--method {options} or -m` - a string of various methods through which to filter the
graph.
* `--quick or -q` - GraphPass will run a basic set of algorithms for visualization with no filtering. The filename will be the same as the input filename.
* `--gexf or -g` - GraphPass will return the graph output in gexf (good for SigmaJS) instead of graphml.
* `--max-nodes {Value}` - Change default maximum number of nodes that GraphPass will accept. By default this is 50,000. Values larger than 50k may cause GraphPass to use up a computer's memory.
* `--max-edges {Value}` - Change default maximum number of edges that GraphPass will accept. By default this is 500,000. Values larger than 500k are unlikely to cause significant delays in computation time, but could result in memory issue upon visualization in Gephi or SigmaJS.

These various methods are outlined below:

* `a` : authority
* `b` : betweenness
* `d` : simple degree
* `e` : eigenvector
* `h` : hub
* `i` : in-degree
* `o` : out-degree
* `p` : pagerank
* `r` : random

For example:

```
./graphpass /path/to/links-for-gephi.graphml --percent 10 --methods b /path/to/output_filename
```

Will remove 10% of the graph using betweenness as a cutting measure and lay the network out. It will find `links-for-gephi.graphml` file in `path/to/input` and output a new one to `/path/to/output_filename.graphml` (titled `output_filename10Betweenness.graphml`).

# Optional arguments

* `--report` or `-r` : create an output report showing the impact of filtering on graph features.
* `--no-save` or `-n` : does not save any filtered files (useful if you just want a report).

# Troubleshooting

It is possible that you can get a "error while loading shared libraries" error in Linux. If so, try running `sudo ldconfig` to set the libraries path for your local installation of igraph.

# License

Licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).

# Acknowledgments

This work is primarily supported by the [Andrew W. Mellon Foundation](https://uwaterloo.ca/arts/news/multidisciplinary-project-will-help-historians-unlock). Additional funding for the utility has come from the Social Sciences and Humanities Research Council of Canada, the Ontario Ministry of Research and Innovation's Early Researcher Award program, and the David R. Cheriton School of Computer Science at the University of Waterloo.

The author would also like to thank Drs. Ian Milligan & Jimmy Lin plus Nick Ruest and Samantha Fritz for their kind advice and support.
