// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_ACTORS_HPP
#define IMDB_ACTORS_HPP

#include "table.hpp"


// Represents an actor in a movie.
struct actor
{
  actor(const char* a)
    : name(a)
  { }

  // Add the index of a role to the actor's filmography.
  void add_role(int r) { roles.push_back(r); }

  std::string name;
  std::vector<int> roles;
};


// Stores all actors in the database.
using actor_table = table<actor>;


#endif