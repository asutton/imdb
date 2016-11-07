# An IMDb parser

This project contains a C++ implementation of a parser for IMDb data files,
which can be downloaded from http://www.imdb.com/interfaces. Be sure to read
the copyright and license information on that site, as there are limitations
on its usage.

This is a work in progress. It is not complete, and it is almost certainly
not (entirely) correct.

Currently, this project supports parsers for the following files:

- actors.list
- actresses.list

The actor file parser is designed to produce row sets of parsed data. This
can be consumed to produce an actual database, a graph, filters, etc.