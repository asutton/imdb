// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_MOVIES_HPP
#define IMDB_MOVIES_HPP

#include "table.hpp"

#include <unordered_map>
#include <vector>


// Represents a movie, tv episode, or video game.
struct movie
{
  movie(const char* n, const char* y)
    : name(n), year(y)
  { }
  
  std::string name;
  std::string year;
};


// Stores all movies in the database.
using movie_table = table<movie>;


#endif
