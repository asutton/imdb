// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include <imdb/actor_parser.hpp>

#include <iostream>


struct visitor
{
  void on_actor(const char* a) {
    // Nothing to do.
  }

  void on_row(const char* a, const char* p, const char* r) {
    std::cout << a << " :: " << p << '\n';
  }
};

int
main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    imdb::actor_parser<visitor> p(argv[i]);
    p.parse();
  }
}
