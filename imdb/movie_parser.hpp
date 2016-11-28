// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_MOVIE_PARSER_HPP
#define IMDB_MOVIE_PARSER_HPP

#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stdexcept>


namespace imdb {

  // Responsible for the parsing of contents from the movie list file.
  template<typename V>
  class movie_parser {
  public:
    movie_parser(const char*);
    movie_parser(const char*, V);
    
    void parse();

  private:
    static constexpr int max_line = 2048; // Maximum line length

    void get_line();
    bool check_line(const char*);
    void skip_preamble();
    char* skip_tabs(char* p);

    std::FILE* input; // The file being parsed.
    V vis; // The visitor.
    
    int line = 0; // Current line number.
    char buf[max_line]; // The current line.
  };


  // Construct a parser for the given file.
  template<typename V>
  movie_parser<V>::movie_parser(const char* path) 
    : movie_parser(path, V())
  { }

  // Construct a parser for the given file.
  template<typename V>
  movie_parser<V>::movie_parser(const char* path, V vis) 
    : input(::fopen(path, "r")), vis(vis)
  {
    if (!input)
      throw std::runtime_error("error: cannot open actor file");
  }

  template<typename V>
  void 
  movie_parser<V>::parse() {
      skip_preamble();

      while (true) {
        get_line();

        if (buf[0] == '\n')
          continue;

        if (buf[0] == '-' && buf[1] == '-')
          break;

        // Get the movie name.
        //
        // TODO: A movie name has a lot of internal structure, and it's
        // shared with the actor files. We should parse that information
        // out so that we have full knowledge of the data structure.
        char* ptr = std::strchr(buf, '\t');
        *ptr = 0;
        vis.on_movie(buf);

        // Get year information. This can be a range of years if the
        // entry denotes a series.
        ptr = skip_tabs(ptr + 1);
        char* end = strchr(ptr + 1, '\n');
        *end = 0;
        vis.on_row(buf, ptr);
      }
    }

  // Get the next line of input from the file.
  template<typename V>
  void 
  movie_parser<V>::get_line() {
    char* str = std::fgets(buf, max_line, input);
    assert(check_line(str));
    ++line;
  }

  // Verify that the line fits in the allocated buffer.
  //
  // TODO: Consider dynamically allocating extra memory for "jumbo"
  // lines rather than having a single large buffer.
  template<typename V>
  bool
  movie_parser<V>::check_line(const char* buf) {
    if (std::strlen(buf) == max_line - 1) {
      std::printf("%d: line overflow\n", line);
      return false;
    }
    return true;
  }

  // Consume all lines of text contributing to the preamble. The preamble
  // for the move file is very short, so we're just going to look for
  // the '=' under the "Movie List" column. 
  template<typename V>
  void 
  movie_parser<V>::skip_preamble() {
    // Skip the preamble.
    do {
      get_line();        
    } while (buf[0] != '=');
  }

  // Consume a sequence of consecutive tab characters.
  template<typename V>
  char* 
  movie_parser<V>::skip_tabs(char* p) {
    while (*p == '\t')
      ++p;
    return p;
  }

} // namespace imdb


#endif
