#ifndef __HASHTABLE_HPP__
#define __HASHTABLE_HPP__

#include <utility>
#include <string>

typedef std::pair<int, std::string> tableEntry;

class Hashtable {
  tableEntry *table;
  int size;
  int capacity;
  int Find(std::string &);
  int Insert(std::string &);
  int FindOrInsert(std::string &);
  unsigned long hash(std::string &);
  unsigned long hash2(std::string &);
public:
  Hashtable();
  Hashtable(int);
  ~Hashtable();
  int Put(tableEntry &);
  int Set(std::string&, int);
  int Increment(std::string&);
  int Decrement(std::string&);
  int Remove(std::string&);
  void EnsureCapacity(int);
  int Size();
  int Capacity();
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
