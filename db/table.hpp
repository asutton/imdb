// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef IMDB_TABLE_HPP
#define IMDB_TABLE_HPP

#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>


// A database-like table. This simply stores a sequence of objects (rows).
// The id of a row in the table can be computed.
template<typename T>
struct table
{
  // Pre-allocate storage for n elements.
  void reserve(int n) { rows.reserve(n); }

  // Returns the number of rows in the table.
  int size() const { return rows.size(); }

  // Returns the row index of the object t, which must be in the table.
  int row(const T& t) const { return &t - rows.data(); }

  // Row access
  const T& operator[](int n) const { return rows[n]; }
  T& operator[](int n) { return rows[n]; }

  template<typename... Args>
  int emplace(Args&&... args) { 
    rows.emplace_back(std::forward<Args>(args)...); 
    return rows.size() - 1;
  }

  std::vector<T> rows;
};


// Used as a hash function for C-strings. 
//
// This is the djb2 function, which seems to be adequate for a large number 
// of applications.
struct cstr_hash
{
  std::size_t operator()(const char* str) const {
    std::size_t h = 5381;
    while (*str) {
      h = ((h << 5) + h) + *str;
      ++str;
    }
    return h;
  }
};


// Used to compare two C-strings for equality.
struct cstr_eq
{
  bool operator()(const char* a, const char* b) const {
    return !std::strcmp(a, b);
  }
};


// Defines a mapping of names to rows in a table. Note that storing pointers 
// would be BAD (insertions into the table invalidate pointers).
//
// This stores C-strings in order to reduce the memory overhead required
// to store std::string objects.
struct name_index
{
  void reserve(int n) { map.reserve(n); }

  // Returns the number of keys in the index.
  int size() const { return map.size(); }

  int find(const std::string& n) const {
    auto iter = map.find(n.c_str());
    if (iter != map.end())
      return iter->second;
    else
      return -1;
  }

  template<typename... Args>
  void emplace(Args&&... args) {
    map.emplace(std::forward<Args>(args)...);
  }

  std::unordered_map<const char*, int, cstr_hash, cstr_eq> map;
};


#endif
