// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include <imdb/movie_parser.hpp>

#include <iostream>


struct visitor
{
  void on_movie(const char* m) {
    // std::cout << m << '\n';
  }

  void on_row(const char* m, const char* y) {
    std::cout << m << " :: " << y << '\n';
  }
};

int
main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    imdb::movie_parser<visitor> p(argv[i]);
    p.parse();
  }
}
