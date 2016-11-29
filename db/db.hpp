// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_DB_HPP
#define IMDB_DB_HPP

#include "movies.hpp"
#include "actors.hpp"
#include "roles.hpp"

struct database 
{
  database();
  
  int add_movie(const char* name, const char* year);
  int find_movie(const char* name);

  int add_actor(const char* name);
  int find_actor(const char* name);

  int find_actor(const std::string& name);
  int add_role(const char* act, const char* mov, const char* info);
  
  // Storage for movies and actors.
  movie_table movies;
  actor_table actors;
  role_table roles;

  // Efficient lookup for movie and actor names.
  name_index movie_lookup;
  name_index actor_lookup;

  int movie_lookup_errors = 0;
};


#endif