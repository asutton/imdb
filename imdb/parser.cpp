// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stdexcept>

namespace imdb {

  // A string view is a pair of pointers into an underlying buffer.
  struct string_view {
    const char* first = nullptr;
    const char* limit = nullptr;
  };

  // Responsible for the parsing of contents from an actor file.
  struct actor_file_parser {
    // The maximum line length. This is established empirically. This
    // 
    static constexpr int max_line = 2048;
    static constexpr int max_str = 256;

    actor_file_parser(const char* path) : input(::fopen(path, "r")) {
      if (!input)
        throw std::runtime_error("error: cannot open actor file");
    }

    void parse() {
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
        // walking backwards through the string (i.e., middle out compression).

        if (*role) {
          char* end = strchr(role, '\n');
          *end = 0;
        }
        
        std::printf("%s :: [[%s]] :: [[%s]]\n", actor, product, role);
      }
    }

    // Get the next line of input from the file.
    void get_line() {
      std::fgets(buf, max_line, input);
      assert(check_line(buf));
      ++line;
    }

    // Verify that the line fits in the allocated buffer.
    //
    // TODO: Consider dynamically allocating extra memory for "jumbo"
    // lines rather than having a single large buffer.
    bool check_line(const char* buf) {
      if (std::strlen(buf) == max_line - 1) {
        std::printf("%d: line overflow\n", line);
        return false;
      }
      return true;
    }

    void skip_preamble() {
      // Skip the preamble.
      do {
        get_line();        
      } while (buf[0] != '-');

      // Skip until we've reached the Name column divider.
      do {
        get_line();        
      } while (buf[0] != '-');
    }

    char* skip_tabs(char* p) {
      while (*p == '\t')
        ++p;
      return p;
    }


    std::FILE* input; // The file being parsed.
    
    char buf[max_line]; // The current line.
    char actor[max_str]; // Stores the current actor.
    
    int line = 0; // Current line number.
    int sec = 0; // The current section count.
  };


} // namespace imdb

int main(int argc, char* argv[]) {
  imdb::actor_file_parser p(argv[1]);
  p.parse();
}
