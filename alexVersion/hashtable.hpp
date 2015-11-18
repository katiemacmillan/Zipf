#ifndef __HASHTABLE_HPP__
#define __HASHTABLE_HPP__
/*
hashtable.hpp
Header file for frequency hashtable.
This hashtable is specialized for word frequency data. Negative numbers aren't supported.
Missing entries default to zero.

Author: Alex Iverson
*/

#include <string>
#include "myPair.hpp"

typedef myPair<int, std::string> tableEntry;

class Hashtable {
  tableEntry *table;
  int size;
  int capacity;
  //Find an already existing table entry.
  int Find(std::string &);
  //Chose an open slot to place a new table entry in.
  //This function manages the size variable and triggering rehashes.
  int Insert(std::string &);
  //If a given entry already exists, find it. Otherwise, create and insert an entry.
  int FindOrInsert(std::string &);
  //string hash functions courtesy of http://www.cse.yorku.ca/~oz/hash.html
  unsigned long hash(std::string &);
  unsigned long hash2(std::string &);
public:
  //default constructor
  Hashtable();
  //construct with capacity
  Hashtable(int);
  ~Hashtable();
  //Set an entry from a tableEntry. Returns the new value.
  int Put(tableEntry &);
  //Set the value associated with a given key, creating the entry if necessary. Returns the new Value.
  int Set(std::string&, int);
  //Increment the value associated with a given key, incrementing it if necessary. Returns the new Value.
  int Increment(std::string&);
  //Decrement the value associated with a given key, deleting it if the new value is zero. Returns the new Value.
  int Decrement(std::string&);
  //Unconditionally removes an entry, returning the value formerly associated with the given key.
  int Remove(std::string&);
  //Returns the value associated with the given key or zero if it isn't found.
  int Get(std::string&);
  //Ensures that the hashtable has enough slots to store cap elements.
  //This does not account for load factor, so user code should overestimate by 1/3 to avoid rehashing.
  void EnsureCapacity(int);
  //Delete every entry in the table.
  void Clear();
  //return size
  int Size();
  //return capacity
  int Capacity();
  //Iterator support. These iterators will only iterate through entries present in the table and not empty or deleted slots.
  //There are sentinel entries in the hashtable to prevent overstepping the bounds of valid memory.
  //To preserve speed and size, these iterators have no way to bounds check. This is the responsibility of the calling code. The sentinel entries ensure that the calling code has a chance to check before the iterator traverses into invalid memory.
  //Don't bother with pass by reference. The memory footprint of an iterator is a single pointer.
  class iterator {
    tableEntry* loc;
  public:
    iterator(tableEntry *loc);
    ~iterator();
    iterator& operator++();
    iterator& operator--();
    iterator operator++(int);
    iterator operator--(int);
    const tableEntry& operator*();
    bool operator==(iterator other) {return loc == other.loc;};
    bool operator<(iterator other) {return loc < other.loc;};
    bool operator>(iterator other) {return loc > other.loc;};
    bool operator>=(iterator other) {return loc >= other.loc;};
    bool operator<=(iterator other) {return loc <= other.loc;};
    bool operator!=(iterator other) {return loc != other.loc;};
  };
  iterator begin();
  iterator end();
};

#endif
