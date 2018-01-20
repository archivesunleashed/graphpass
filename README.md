# Borg::Reducer

The Borg reducer is a helper library to filter networks and provide a default
visualization output for Gephi.  It prevents the infamous "borg cube" result
when entering large files into gephi.


## Installation

```git clone https://github.com/archivesunleashed/borg-reducer
cd borg-reducer
make
```

## Usage

Once compiled use:
``` borgreducer --file {FILENAME} --percent {PERCENTAGE TO FILTER} --method {METHODS (see below)} --output {OUTPUT DIRECTORY}
```
--method is a string of characters representing the algorithms to apply to the filtering.

b : betweenness
c : clustering
d : simple degree
e : eigenvector
i : in-degree
o : out-degree
p : pagerank
r : random
w : weighted degree

In future, a feature to create a validity report will be produced for each method applied.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## Code of Conduct

Everyone interacting in the Borg::Reducer project’s codebases, issue trackers, chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/archivesunleashed/borg-reducer/blob/master/CODE_OF_CONDUCT.md).
