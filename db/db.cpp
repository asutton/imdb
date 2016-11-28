// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include "movies.hpp"
#include "actors.hpp"

#include <imdb/actor_parser.hpp>
#include <imdb/movie_parser.hpp>

#include <iostream>
#include <unordered_map>
#include <vector>


struct movie_visitor
{
  movie_visitor(movie_table& m, name_index& l)
    : movies(m), lookup(l)
  { }

  // Nothing to do.
  void on_movie(const char* m) { }

  // Save each movie.
  void on_row(const char* n, const char* y) {
    movie& m = movies.emplace(n, y);
    lookup.emplace(m.name.c_str(), movies.row(m));
  }

  movie_table& movies;
  name_index& lookup;
};


struct actor_visitor
{
  actor_visitor(actor_table& a, name_index& al, name_index& ml)
    : actors(a), actor_lookup(al), movie_lookup(ml)
  { }

  void on_actor(const char* n) {
    actor& a = actors.emplace(n);
    actor_lookup.emplace(a.name.c_str(), actors.row(a));
  }

  void on_row(const char* a, const char* p, const char* r) {
    // TODO: Connect actors to movies.
  }

  actor_table& actors;
  name_index& actor_lookup;
  name_index& movie_lookup;
};



int
main(int argc, char* argv[]) {

  constexpr int four_million = 4 << 20;

  // Storage for movies and actors.
  movie_table movies;
  actor_table actors;

  // Efficient lookup for movie and actor names.
  name_index movie_lookup;
  name_index actor_lookup;

  // Pre-allocate a bunch of storage for these things.
  movies.reserve(four_million);
  actors.reserve(four_million);
  movie_lookup.reserve(four_million);
  actor_lookup.reserve(four_million);

  // Actually parse the content.
  movie_visitor movie_vis(movies, movie_lookup);
  actor_visitor actor_vis(actors, actor_lookup, movie_lookup);

  imdb::movie_parser<movie_visitor> movie_parser("movies.list", movie_vis);
  imdb::actor_parser<actor_visitor> actor_parser("actors.list", actor_vis);
  imdb::actor_parser<actor_visitor> actress_parser("actresses.list", actor_vis);

  std::cout << "* loading movies\n"; 
  movie_parser.parse();
  std::cout << "* loaded " << movies.size() << " movies\n";

  std::cerr << "* loading actors\n";
  actor_parser.parse();
  std::cout << "* loading actresses\n";
  actress_parser.parse();
  std::cout << "* loaded " << actors.size() << " actors\n";

  // Get the target actor (Kevin Bacon).
  const char* kb = "Bacon, Kevin (I)";
  int target = actor_lookup.find(kb);
  std::cout << "* index of \"" << kb << "\": " << target << '\n';

  // Emulate a simple shell.
  std::string actor;
  std::cout << "actor> ";
  while (std::getline(std::cin, actor)) {
    if (!std::cin || actor == "exit")
      break;
    int source = actor_lookup.find(actor);
    std::cout << "* index of \"" << actor << "\": " << source << '\n';
  }

}
