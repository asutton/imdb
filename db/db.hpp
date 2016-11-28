// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include <unordered_map>
#include <vector>


// Represents a movie, tv episode, or video game.
struct movie
{
  std::string name;
  std::string year;
};


// The movie table stores all of movies.
struct movie_table : std::vector<movie>
{
  using std::vector<movie>::vector;

  int row(const movie&);
};

// Returns the row in which the movie m is stored.
int
movie_table::row(const movie& m)
{
  return &m - data();
}


// Represents an actor in a movie.
struct actor
{
  std::string name;
};


// Stores all actors in the database.
using std::vector<actor> actor_table;


// Defines a mapping of actor names to actors.



struct actor_visitor
{
  void on_actor(const char* a) {
    std::cout << a << '\n';
  }

  void on_row(const char* a, const char* p, const char* r) {
    // Nothing to do.
  }
};


struct movie_visitor
{
  movie_visitor

  void on_movie(const char* m) {
    std::cout << m << '\n';
  }

  void on_row(const char* m, const char* y) {
    // std::cout << p << '\n';
  }
};


int
main(int argc, char* argv[]) {

  std::set<

  imdb::movie_parser<visitor> movie_parser(argv[i]);
  p.parse();
  imdb::parse_moves
}
