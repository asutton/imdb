// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_ACTORS_HPP
#define IMDB_ACTORS_HPP

#include "table.hpp"

#include <unordered_map>
#include <vector>


// Represents an actor in a movie.
struct actor
{
  actor(const char* a)
    : name(a)
  { }

  std::string name;
};


// Stores all actors in the database.
using actor_table = table<actor>;


#endif