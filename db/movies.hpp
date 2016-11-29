// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_MOVIES_HPP
#define IMDB_MOVIES_HPP

#include "table.hpp"


// Represents a movie, tv episode, or video game.
struct movie
{
  movie(const char* n, const char* y)
    : name(n), year(y)
  { }

  // Add the index of an actor's role in a movie to the cast.
  void add_role(int r) { roles.push_back(r); }
  
  std::string name;
  std::string year;
  std::vector<int> roles;
};


// Stores all movies in the database.
using movie_table = table<movie>;


#endif
