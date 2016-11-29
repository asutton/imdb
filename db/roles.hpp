// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_ROLE_HPP
#define IMDB_ROLE_HPP

#include "table.hpp"


// Represents an actor's role in a movie or production.
//
// TODO: Factor the role description into useful information.
struct role
{
  role(int a, int m, const char* i)
    : actor(a), movie(m), info(i)
  { }

  int actor;
  int movie;
  std::string info;
};

using role_table = table<role>;


#endif
