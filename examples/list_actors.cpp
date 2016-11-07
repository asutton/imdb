// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include <imdb/actor_parser.hpp>

#include <iostream>


struct visitor
{
  void on_actor(const char* a) {
    std::cout << a << '\n';
  }

  void on_row(const char* a, const char* p, const char* r) {
    // Nothing to do.
  }
};

int
main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    imdb::actor_parser<visitor> p(argv[i]);
    p.parse();
  }
}
