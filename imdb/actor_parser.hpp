// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_ACTOR_PARSER_HPP
#define IMDB_ACTOR_PARSER_HPP

#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stdexcept>


namespace imdb {

  // Responsible for the parsing of contents from an actor file. The file
  // parser is parameterized by a visitor, which defines a set of functions
  // corresponding to parsing events.
  template<typename V>
  class actor_parser {
  public:
    actor_parser(const char*);
    actor_parser(const char*, V);
    
    void parse();

  private:
    static constexpr int max_line = 2048; // Maximum line length
    static constexpr int max_str = 256; // Maximum actor name length

    void get_line();
    bool check_line(const char*);
    void skip_preamble();
    char* skip_tabs(char* p);

    std::FILE* input; // The file being parsed.
    V vis; // The visitor.
    
    int line = 0; // Current line number.
    char buf[max_line]; // The current line.
    char actor[max_str]; // Stores the current actor.
    
  };


  // Construct a parser for the given file.
  template<typename V>
  actor_parser<V>::actor_parser(const char* path) 
    : actor_parser(path, V())
  { }

  // Construct a parser for the given file.
  template<typename V>
  actor_parser<V>::actor_parser(const char* path, V vis) 
    : input(::fopen(path, "r")), vis(vis)
  {
    if (!input)
      throw std::runtime_error("error: cannot open actor file");
  }

  template<typename V>
  void 
  actor_parser<V>::parse() {
      skip_preamble();

      while (true) {
        get_line();

        if (buf[0] == '\n')
          continue;

        // Match an entry. These have one of the following forms:
        //
        //    name <tab>+ role <newline>
        //    <tab>+      role <newline>
        //
        // If no tab is found, there are no more actors in the input.
        char* tab = std::strchr(buf, '\t');
        if (!tab)
          break;

        if (tab != buf) {
          // Preserve the actor name in a separate buffer since we're
          // going to be parsing roles in subsequent lines.
          assert(tab - buf < max_str);
          char* end = std::copy(buf, tab, actor);
          *end = 0;
          vis.on_actor(actor);
        }

        // All mappings have one of the following forms:
        //
        //    production role billing
        //
        // Each component is separated by a string of two spaces. A 
        // production gives unique identifying information about the
        // production. This has the form:
        //
        //    title (year) [subtitle] [kind] <space><space>
        //
        // The title appears in quotes when it is a serial production.
        // For series, the subtitle is given in braces. The kind qualifies
        // either (TV), (V), or (VG). However, this aspect is followed by
        // two spaces.
        //
        // Information about the actor's role in the production follows
        // the two spaces.
        char* product = skip_tabs(tab);
        char* ptr = product;
        while (*ptr != '\n') {
          if (*ptr == ' ' && *(ptr + 1) == ' ')
            break;
          ++ptr;
        }

        // Determine if and where the role information starts. Then,
        // null-terminate the production.
        char* role = ptr;
        if (*ptr != '\n')
          role += 2;
        *ptr = 0;

        // TODO: Extract the different components of the production by
        // walking backwards through the string. Note that it's easier
        // to match walking backwards than forwards.
        //
        // TODO: Support filters on different kinds of productions (e.g.,
        // only movies, movies + video games, etc).

        // Match the end of the role.
        //
        // TODO: This currently includes the billing, if present. We should
        // match that separately.
        if (*role) {
          char* end = strchr(role, '\n');
          *end = 0;
        }
        
        vis.on_row(actor, product, role);
      }
    }

  // Get the next line of input from the file.
  template<typename V>
  void 
  actor_parser<V>::get_line() {
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
  actor_parser<V>::check_line(const char* buf) {
    if (std::strlen(buf) == max_line - 1) {
      std::printf("%d: line overflow\n", line);
      return false;
    }
    return true;
  }

  // Consume all lines of text contributing to the preamble.
  template<typename V>
  void 
  actor_parser<V>::skip_preamble() {
    // Skip the preamble.
    do {
      get_line();        
    } while (buf[0] != '-');

    // Skip until we've reached the Name column divider.
    do {
      get_line();        
    } while (buf[0] != '-');
  }

  // Consume a sequence of consecutive tab characters.
  template<typename V>
  char* 
  actor_parser<V>::skip_tabs(char* p) {
    while (*p == '\t')
      ++p;
    return p;
  }

} // namespace imdb


#endif
