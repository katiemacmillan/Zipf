/*
hashtable.cpp
Implementation of the hashtable for the Zipf's law program.
It is specialized to store word frequency data, so negative numbers aren't supported.
Missing entries default to zero.

Author: Alex Iverson
*/

#include "hashtable.hpp"
#include <iostream>

#define PROBE_LINEAR 1
#define PROBE_QUADRATIC 2
#define PROBE_DOUBLEHASH 3
#ifndef PROBE_SEQUENCE
#define PROBE_SEQUENCE PROBE_LINEAR
#endif

using namespace std;

bool checkPrime(int val) {
  int n = 2;
  while (n * n <= val) {
    if (val % n == 0) {
      return false;
    }
    ++n;
  }
  return true;
}

//credit goes to http://www.cse.yorku.ca/~oz/hash.html djb2
unsigned long Hashtable::hash(string& str) {
  unsigned long hash = 5381;
  for (auto c : str) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

//credit goes to http://www.cse.yorku.ca/~oz/hash.html sdbm
unsigned long Hashtable::hash2(string& str) {
  unsigned long hash = 0;
  for (auto c : str) {
    hash = c + (hash << 6) + (hash  << 16) - hash;
  }
  return hash;
}

//default constructor
Hashtable::Hashtable(): table(new tableEntry[10011] + 1), size(0), capacity(1009) {
  //sentinel entries to guarantee that the iterators don't go into invalid memory before being checked
  table[-1].first = 1;
  table[capacity].first = 1;
};

//construct with capacity
Hashtable::Hashtable(int capacity): table(nullptr), size(0), capacity(capacity) {
  while (!checkPrime(capacity)) {++capacity;}
  table = new tableEntry[capacity + 2] + 1;
  //sentinel entries
  table[-1].first = 1;
  table[capacity].first = 1;
  this->capacity = capacity;
}

Hashtable::~Hashtable() {
  delete[] (table - 1);
}

//Find an already existing table entry.
int Hashtable::Find(string& key) {
  int index = hash(key) % capacity;
#if PROBE_SEQUENCE == PROBE_QUADRATIC
  int delta = -1;
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
  int delta = hash2(key) % (capacity - 1) + 1; //guarantee nonzero
#endif
  while (table[index].first && table[index].second != key) {
#if PROBE_SEQUENCE == PROBE_LINEAR
    ++index;
#elif PROBE_SEQUENCE == PROBE_QUADRATIC
    index += (delta += 2);
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
    index += delta;
#endif
    if (index >= capacity) {
      index -= capacity;
    }
  }
  if (table[index].second != key) {
    index = -1;
  }
  return index;
}

//Chose an open slot to place a new table entry in.
//This function manages the size variable and triggering rehashes.
int Hashtable::Insert(string& key) {
  ++size;
  if (size*4 > capacity*3) { //Check 0.75 load factor
    EnsureCapacity(capacity*2);
  }
  int index = hash(key) % capacity;
#if PROBE_SEQUENCE == PROBE_QUADRATIC
  int delta = -1;
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
  int delta = hash2(key) % (capacity - 1) + 1; //guarantee nonzero
#endif
  while (table[index].first > 0) {
#if PROBE_SEQUENCE == PROBE_LINEAR
    ++index;
#elif PROBE_SEQUENCE == PROBE_QUADRATIC
    index += (delta += 2);
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
    index += delta;
#endif
    if (index >= capacity) {
      index -= capacity;
    }
  }
  table[index].second = key;
  table[index].first = 0;
  return index;
}

//If a given entry already exists, find it.
//Otherwise, create insert an entry.
int Hashtable::FindOrInsert(string& key) {
  int index = Find(key);
  if (index == -1) {
    index = Insert(key);
  }
  return index;
}

//Set an entry from the values in a tableEntry. Returns the new value.
int Hashtable::Put(tableEntry& entry) {
  int index = FindOrInsert(entry.second);
  table[index].first = entry.first ? entry.first : -1;
  return entry.first;
}

//Set the value associated with a given key, creating the entry if necessary. Returns the new value.
int Hashtable::Set(string& key, int val) {
  int index = FindOrInsert(key);
  table[index].first = val ? val : -1;
  return val;
}

//Increment the value associated with a given key, incrementing it if necessary. Returns the new value.
int Hashtable::Increment(string& key) {
  int index = FindOrInsert(key);
  return ++table[index].first;
}

//Decrement the value associated with a given key, deleting it if the new value is zero. Returns the new value.
int Hashtable::Decrement(string& key) {
  int index = Find(key);
  int val = 0;
  if (index != -1) {
    val = --table[index].first;
    if (!table[index].first) {
      table[index].first = -1;
      --size;
    }
  }
  return val;
}

//Unconditionally removes an entry, returning the value formerly associated with the given key.
int Hashtable::Remove(string& key) {
  int index = Find(key);
  int val = 0;
  if (index != -1) {
    val = table[index].first;
    table[index].first = -1;
  }
  --size;
  return val;
}

//Returns the value associated with the given key or zero if it isn't found.
int Hashtable::Get(string& key) {
  int index = Find(key);
  return index == -1 ? 0 : table[index].first;
}

//Ensures that the hashtable has enough slots to store cap elements.
//This does not account for load factor, so user code should overestimate by 1/3 to avoid rehashing.
void Hashtable::EnsureCapacity(int cap) {
  if (cap <= capacity) {return;}
  while (!checkPrime(cap)) {++cap;}
#ifndef SUPRESS_REHASH_OUTPUT
  cout << "rehashing to size " << cap << endl;
#endif
  auto tmp = table;
  table = new tableEntry[cap + 2] + 1;
  table[-1].first = 1;
  table[cap].first = 1;
  auto tmpEnd = &tmp[capacity];
  capacity = cap;
  for (auto ent = tmp; ent < tmpEnd; ++ent) {
    if (ent->first > 0) {
      Put(*ent);
    }
  }
  delete[] (tmp - 1);
}

void Hashtable::Clear() {
  for (auto ent = table; ent < &table[capacity]; ++ent) {
    ent->first = 0;
    ent->second = string();
  }
}

int Hashtable::Size() {
  return size;
}

int Hashtable::Capacity() {
  return capacity;
}

//iterator support.
Hashtable::iterator::iterator(tableEntry *loc): loc(loc) {};

Hashtable::iterator::~iterator() {};

//The sentinel entries will ensure that these loops will terminate at the ends of the hashmap before reaching invalid memory.
//The controlling loops will therefore be able to check termination conditions.
//Note: it is unsafe to attempt to increment or decrement more than once past the first or last elements of the table.
Hashtable::iterator Hashtable::iterator::operator++(int ignore) {
  Hashtable::iterator tmp = *this;
  do {
    ++this->loc;
  } while (this->loc->first <= 0);
  return tmp;
}

Hashtable::iterator& Hashtable::iterator::operator++() {
  do {
    ++this->loc;
  } while (this->loc->first <= 0);
  return *this;
}

Hashtable::iterator Hashtable::iterator::operator--(int ignore) {
  Hashtable::iterator tmp = *this;
  do {
    --this->loc;
  } while (this->loc->first <= 0);
  return tmp;
}

Hashtable::iterator& Hashtable::iterator::operator--() {
  do {
    --this->loc;
  } while (this->loc->first <= 0);
  return *this;
}

//Dereference operator
const tableEntry& Hashtable::iterator::operator*() {
  return *loc;
}

//support range based for loops and iterators.
Hashtable::iterator Hashtable::begin() {
  return ++Hashtable::iterator(table - 1);
}

Hashtable::iterator Hashtable::end() {
  return Hashtable::iterator(&table[capacity]);
}
